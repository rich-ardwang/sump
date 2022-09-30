//
//  subscribe_manager.cpp
//
//  Created by lei.wang on 2019-08-26.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#include <algorithm>
#include <cstring>
#include <sfa/msg/value_kv_array.h>
#include "lava_comm_api.h"
#include "dds_session.h"
#include "dds_utils.h"
#include "subscribe_manager.h"
#include "sfa_utils/sfa_utils.h"

namespace sfa {
namespace dds {

void filter_send_msg(_sfa_in_ const std::string &ic, _sfa_in_ const std::set<int> *fids, _sfa_in_ const msg::IMessage *msg,
	_sfa_in_ sfa::base::i_auto_ptr<IMsgListener> listener, _sfa_inout_ dispatchInfo *dispatch) {
	//check params
	if ((ic.empty()) || (nullptr == fids) || (nullptr == msg) 
		|| (nullptr == listener.get())) {
		return;
	}

	//if dispatch_flag is true, need to filter the messages those the listener had received
	if (dispatch->dispatch_flg_) {
		//parse sdn message to get the sequence number
		sfa::msg::IFieldValue *fidValue = msg->GetBody()->GetField(sequence_number);
		if (nullptr == fidValue) {
			sfa_log_error(sfa_subscribe_manager, "[filter_send_msg]: get field value failed!");
			return;
		}
		std::string seq_no = fidValue->ToString();	//the value is "[5103.SSEC.20190830.112647]" for example
		if (seq_no.empty()) {
			sfa_log_error(sfa_subscribe_manager, "[filter_send_msg]: sequence number is empty!");
			return;
		}

		//parse sequence id
		std::string::size_type ln = seq_no.length();
		std::string::size_type pos = seq_no.find_last_of(".");
		std::string s_tmp = seq_no.substr(0, pos);
		pos = s_tmp.find_last_of(".");
		//for example: "[5103.SSEC.20190830.112647]" convert to "20190830.112647"
		std::string seq_id = seq_no.substr(pos + 1, ln - (pos + 1) - 1);

		//filter sdn message according to sequence id
		if (dispatch->sequence_id_ >= seq_id) {
			return;
		}

		//update sequence number
		{
			sfa::utils::sfa_c_autolock guard(*dispatch);
			dispatch->sequence_id_ = seq_id;
		}
	}

	//cut the unuse fids in subscribe message
	std::vector<int> all_fids = msg->GetBody()->GetAllFieldId();
	for (auto &it : all_fids) {
		auto it_set = fids->find(it);
		if (it_set == fids->end()) {
			msg->GetBody()->DelField(it);
		}
	}

	//dispatch msg to the listener
	listener->OnReply(ic.c_str(), msg);
}

void lava_event_listener::on_event(int32_t event_code, void* event_data) {
	switch (event_code) {
	//receive dds disconnect
	case i_event_listener::exception_conn_failed:
	case i_event_listener::exception_login_failed:
		if (sub_mgr_->get_lava_sub_flg()) {
			sub_mgr_->set_lava_sub_flg(false);
		}
		break;

	//receive lava connect login success
	case i_event_listener::event_login_success:
		if (!sub_mgr_->get_lava_sub_flg()) {
			sub_mgr_->set_lava_sub_flg(true);
			sub_mgr_->re_subscribe();
		}
		break;

	default:
		break;
	}
}

void lava_msg_listener::on_reply(__lv_in lava::comm::i_message* msg, __lv_in const char* topic) {
	//check
	if ((nullptr == topic) || (nullptr == msg)) {
		return;
	}

	//parse ceda message
	std::string s_ic;
	msg::IMessage *sdn_msg = nullptr;
	bool ret = parse_ack_ceda_msg(msg, sdn_msg, s_ic);
	if (!ret) {
		return;
	}

	//dispatch sdn msg
	sub_mgr_->dispatch_sdn_msg(s_ic, sdn_msg);
}

bool lava_msg_listener::parse_ack_ceda_msg(_sfa_in_ const lava::comm::i_message *ceda_msg, _sfa_out_ msg::IMessage *sdn_msg,
	_sfa_out_ std::string &ic) {
	lava::comm::i_message *cedamsg = const_cast<lava::comm::i_message*>(ceda_msg);
	//check
	if (nullptr == cedamsg) {
		return false;
	}

	//get ceda message body
	lava::comm::i_message_body *body = cedamsg->get_message_body();
	if ((body->has_field(3) != lava::comm::i_message_body::BYTES) || 
		(body->has_field(6) != lava::comm::i_message_body::STRING)) {
		sfa_log_warn(sfa_subscribe_manager, "[parse_ack_ceda_msg]: get ceda message body failed!");
		return false;
	}

	//decode ceda msg to sdn msg
	char* buf = nullptr;
	uint16_t buf_len = body->get_bytes(3, buf);
	msg::IMessage *sdnmsg = nullptr;
	bool ret = msg::DecodeMessage((uint8_t*)buf, buf_len, msg_context_, sdnmsg);
	if (!ret) {
		sfa_log_warn(sfa_subscribe_manager, "[parse_ack_ceda_msg]: decode ceda message failed!");
		return false;
	}
	sdn_msg = sdnmsg;

	//get ic from ceda message
	ic.clear();
	ic = body->get_string(6);
	if (ic.empty()) {
		sfa_log_warn(sfa_subscribe_manager, "[parse_ack_ceda_msg]: get ic from ceda message failed!");
		return false;
	}

	return true;
}

subscriber::subscriber(std::string &ic, std::set<int> *fids, sfa::base::i_auto_ptr<IMsgListener> listener, 
	sfa::utils::threadpool *thread_pool) : ic_(ic), fids_(fids), listener_(listener), thread_pool_(thread_pool), 
	dispatch_info_(nullptr) {
	dispatch_info_ = new dispatchInfo();
}

subscriber::~subscriber() {
	if (fids_) {
		if (!fids_->empty()) {
			fids_->clear();
		}
		delete fids_;
		fids_ = nullptr;
	}

	thread_pool_ = nullptr;

	if (dispatch_info_) {
		delete dispatch_info_;
		dispatch_info_ = nullptr;
	}
}

void subscriber::set_message(_sfa_in_ const std::string &ic, _sfa_in_ const msg::IMessage *msg) {
	//check msg
	if (nullptr == msg) {
		return;
	}

	//set dispatch flag
	if (!dispatch_info_->dispatch_flg_) {
		dispatch_info_->dispatch_flg_ = true;
	}

	//send a task to the thread pool
	if ((thread_pool_) && (ic == this->ic_)) {
		sfa::msg::IMessage* sndmsg = ((sfa::msg::IMessage*)msg)->Clone();
		thread_pool_->commit(filter_send_msg, this->ic_, this->fids_, sndmsg, this->listener_, dispatch_info_);
	}
}

subscribe_manager::subscribe_manager() : lava_session_(nullptr), msg_context_(nullptr), lava_session_flg_(true), 
	lava_subscribe_enable_(true), executor_(nullptr), lava_proxy_(nullptr), lava_event_listener_(nullptr), 
	lava_msg_listener_(nullptr) {
	//init containers
	ic_fids_dict_.clear();
	ic_ref_count_.clear();
	obj_subscribers_.clear();
	ic_subscriber_keys_.clear();

	//create thread pool
	executor_ = new sfa::utils::threadpool(50);
	if (nullptr == executor_) {
		sfa_log_error(sfa_subscribe_manager, "[subscribe_manager]: can not create thread pool!");
	}
}

subscribe_manager::~subscribe_manager() {
	//unsubscribe all ics
	if (lava_subscribe_enable_) {
		unsubscribe_all();
	}

	//unregister event listener to lava
	lava_proxy_->cancel_event(lava_event_listener_.get());

	//release resources
	if (executor_) {
		delete executor_;
		executor_ = nullptr;
	}
	lava_session_ = nullptr;
	msg_context_ = nullptr;
	lava_proxy_ = nullptr;
	lava_event_listener_ = nullptr;
	lava_msg_listener_ = nullptr;
}

void subscribe_manager::set_session(_sfa_in_ const ISession *session, _sfa_in_ const sfa::msg::IMessageContext *context) {
	//check session pointer
	if (nullptr == session) {
		sfa_log_error(sfa_subscribe_manager, "[set_session]: lava session is null!");
		return;
	}
	//check context pointer
	if (nullptr == context) {
		sfa_log_error(sfa_subscribe_manager, "[set_session]: message context is null!");
		return;
	}
	//valid set_lava_session only set one time
	if (lava_session_flg_) {
		//create lava event listener
		lava_event_listener_ = new(std::nothrow) lava_event_listener(this);

		//set session and context
		lava_session_ = const_cast<ISession*>(session);
		msg_context_ = const_cast<sfa::msg::IMessageContext*>(context);
		lava_proxy_ = ((DdsSession*)lava_session_)->CreateProxy();
		if (lava_proxy_) {
			//register event listener to lava
			lava_proxy_->interest_event(lava_event_listener_.get());
		}

		//create lava message listener
		lava_msg_listener_ = new(std::nothrow) lava_msg_listener(this, msg_context_);

		lava_session_flg_ = false;
	}
}

bool subscribe_manager::register_listener(_sfa_in_ const std::string &ic, _sfa_in_ const std::vector<int> &filter_fields, 
	_sfa_in_ IMsgListener* listener) {
	//check params
	if (ic.empty()) {
		sfa_log_warn(sfa_subscribe_manager, "[register_listener]: ic is empty!");
		return false;
	}

	if (filter_fields.empty()) {
		sfa_log_warn(sfa_subscribe_manager, "[register_listener]: filter_fields is empty!");
		return false;
	}

	if (nullptr == listener) {
		sfa_log_warn(sfa_subscribe_manager, "[register_listener]: listener is nullptr!");
		return false;
	}

	//get subscriber key
	ICInfo icinfo;
	icinfo.ic_ = ic;
	for (auto &it : filter_fields) {
		icinfo.fids_set_.insert(it);
	}
	icinfo.listener_addr_ = reinterpret_cast<long>(listener);
	std::string subKey = get_subscriber_key(icinfo);

	//create subscriber or not
	{
		sfa::utils::sfa_c_autolock guard(obj_subscribers_);
		auto it = obj_subscribers_.find(subKey);
		if (it == obj_subscribers_.end()) {
			std::set<int> *pFids = new std::set<int>(icinfo.fids_set_);
			if (nullptr == pFids) {
				sfa_log_error(sfa_subscribe_manager, "[register_listener]: new fids set failed!");
				return false;
			}
			subscriber *pSub = new subscriber(icinfo.ic_, pFids, listener, executor_);
			if (nullptr == pSub) {
				if (pFids) {
					pFids->clear();
					delete pFids;
					pFids = nullptr;
				}
				sfa_log_error(sfa_subscribe_manager, "[register_listener]: new subscriber failed!");
				return false;
			}
			obj_subscribers_.insert(std::pair<std::string, subscriber*>(subKey, pSub));
		}
		else {
			return true;
		}
	}

	//update ic subscriber keys map
	{
		sfa::utils::sfa_c_autolock guard(ic_subscriber_keys_);
		auto it = ic_subscriber_keys_.find(icinfo.ic_);
		if (it != ic_subscriber_keys_.end()) {
			it->second.insert(subKey);
		}
		else {
			std::set<std::string> key_set;
			key_set.insert(subKey);
			ic_subscriber_keys_.insert(std::pair<std::string, std::set<std::string> >(icinfo.ic_, key_set));
		}
	}

	//update ic reference count
	{
		sfa::utils::sfa_c_autolock guard(ic_ref_count_);
		auto it = ic_ref_count_.find(icinfo.ic_);
		if (it == ic_ref_count_.end()) {
			ic_ref_count_.insert(std::pair<std::string, int>(icinfo.ic_, 1));
		}
		else {
			it->second++;
		}
	}

	//because add new subscriber, need to subscribe or not
	{
		sfa::utils::sfa_c_autolock guard(ic_fids_dict_);
		auto it = ic_fids_dict_.find(icinfo.ic_);
		if (it != ic_fids_dict_.end()) {
			//calc the max union of fids
			size_t fids_old_size = it->second.size();
			for (auto &iter : icinfo.fids_set_) {
				it->second.insert(iter);
			}
			size_t fids_new_size = it->second.size();
			if (fids_old_size != fids_new_size) {
				subscribe(icinfo.ic_, it->second, SubscribeType::snapshoot_update, common_subscribe);
			}
		}
		else {
			ic_fids_dict_.insert(std::pair<std::string, std::set<int> >(icinfo.ic_, icinfo.fids_set_));
			subscribe(icinfo.ic_, icinfo.fids_set_, SubscribeType::snapshoot_update, common_subscribe);
		}
	}

	return true;
}

bool subscribe_manager::unregister_listener(_sfa_in_ const std::string &ic, _sfa_in_ const std::vector<int> &filter_fields, 
	_sfa_in_ IMsgListener* listener) {
	//check params
	if (ic.empty()) {
		sfa_log_warn(sfa_subscribe_manager, "[unregister_listener]: ic is empty!");
		return false;
	}

	if (filter_fields.empty()) {
		sfa_log_warn(sfa_subscribe_manager, "[unregister_listener]: filter_fields is empty!");
		return false;
	}

	if (nullptr == listener) {
		sfa_log_warn(sfa_subscribe_manager, "[unregister_listener]: listener is nullptr!");
		return false;
	}

	//get subscriber key
	ICInfo icinfo;
	icinfo.ic_ = ic;
	for (auto &it : filter_fields) {
		icinfo.fids_set_.insert(it);
	}
	icinfo.listener_addr_ = reinterpret_cast<long>(listener);
	std::string subKey = get_subscriber_key(icinfo);

	//remove subscriber object
	{
		sfa::utils::sfa_c_autolock guard(obj_subscribers_);
		auto it = obj_subscribers_.find(subKey);
		if (it != obj_subscribers_.end()) {
			if (it->second) {
				//delete it->second;
				it->second = nullptr;
			}
			it = obj_subscribers_.erase(it);
		}
	}

	//update ic subscriber keys map
	{
		sfa::utils::sfa_c_autolock guard(ic_subscriber_keys_);
		auto it = ic_subscriber_keys_.find(icinfo.ic_);
		if (it != ic_subscriber_keys_.end()) {
			it->second.erase(subKey);
			//if subscriber keys set is empty, remove the key
			if (it->second.empty()) {
				it = ic_subscriber_keys_.erase(it);
			}
		}
	}

	//update ic reference count and unsubscribe if it is necessary
	{
		sfa::utils::sfa_c_autolock guard(ic_ref_count_);
		auto it = ic_ref_count_.find(icinfo.ic_);
		if (it != ic_ref_count_.end()) {
			it->second--;
			if (it->second <= 0) {
				//if ic reference is zero, remove the key
				it = ic_ref_count_.erase(it);
				//only unsubscribe one time when ic reference count reduce to zero
				unsubscribe(icinfo.ic_, icinfo.fids_set_, common_subscribe);
			}
		}
	}

	//update ic fids dict
	{
		sfa::utils::sfa_c_autolock guard(ic_fids_dict_);
		auto it = ic_fids_dict_.find(icinfo.ic_);
		if (it != ic_fids_dict_.end()) {
			//because ic is not referenced, remove the key
			it->second.clear();
			it = ic_fids_dict_.erase(it);
		}
	}

	return true;
}

void subscribe_manager::re_subscribe() {
	sfa::utils::sfa_c_autolock guard(ic_fids_dict_);
	for (auto &it : ic_fids_dict_) {
		subscribe(it.first, it.second, SubscribeType::snapshoot_update, common_subscribe);
	}
}

void subscribe_manager::set_lava_sub_flg(_sfa_in_ bool enable) {
	lava_subscribe_enable_ = enable;
}

bool subscribe_manager::get_lava_sub_flg() {
	return lava_subscribe_enable_;
}

void subscribe_manager::dispatch_sdn_msg(_sfa_in_ const std::string &ic, _sfa_in_ msg::IMessage *sdn_msg) {
	//dispatch sdn message
	sfa::utils::sfa_c_autolock guard(ic_subscriber_keys_);
	auto it = ic_subscriber_keys_.find(ic);
	if (it != ic_subscriber_keys_.end()) {
		for (auto &it_set : it->second) {
			{
				sfa::utils::sfa_c_autolock guard_tmp(obj_subscribers_);
				auto it_map = obj_subscribers_.find(it_set);
				if (it_map != obj_subscribers_.end()) {
					it_map->second->set_message(it->first, sdn_msg);
				}
			}
		}
	}

	//release sdn msg
	sfa::msg::DeleteMessage(&sdn_msg);
}

std::string subscribe_manager::get_subscriber_key(_sfa_in_ const ICInfo &icinfo) {
	std::string key;
	if ((icinfo.ic_.empty()) || (icinfo.fids_set_.empty()) || (icinfo.listener_addr_ <= 0)) {
		return key;
	}
	key = icinfo.ic_;
	key += "_";
	for (auto &it : icinfo.fids_set_) {
		key += std::to_string(it);
		key += ",";
	}
	key.replace(key.length() - 1, key.length(), "_");
	key += std::to_string(icinfo.listener_addr_);
	return key;
}

void subscribe_manager::subscribe(_sfa_in_ const std::string &ic, _sfa_in_ const std::set<int> &fid_set, 
	_sfa_in_ const SubscribeType &sub_type, _sfa_in_ const int sub_flg) {
	//check
	if (!lava_subscribe_enable_) {
		sfa_log_warn(sfa_subscribe_manager, "[subscribe]: lava_subscribe_enable_ false!");
		return;
	}
	if ((ic.empty()) || (fid_set.empty())) {
		sfa_log_warn(sfa_subscribe_manager, "[subscribe]: ic or fid_set empty!");
		return;
	}

	//get sdn message
	msg::IMessage *sdn_msg = get_sdnmsg_common_sub(ic, fid_set, sub_type);
	if (nullptr == sdn_msg) {
		sfa_log_warn(sfa_subscribe_manager, "[subscribe]: sdn_msg is nullptr!");
		return;
	}

	//get ceda message
	lava::comm::i_message *lava_msg = get_req_ceda_msg(sdn_msg, 1);
	if (nullptr == lava_msg) {
		sfa_log_warn(sfa_subscribe_manager, "[subscribe]: lava_msg is nullptr!");
		return;
	}

	//commit to lava
	const char *topic = nullptr;
	if (template_subscribe == sub_flg) {
		topic = ceda_template_topic;
	}
	else if (common_subscribe == sub_flg) {
		topic = TOPIC_API_STREAM_SUB;
	}
	else {
		sfa_log_warn(sfa_subscribe_manager, "[subscribe]: sub_flg : %d failed!", sub_flg);
		return;
	}
	lava_proxy_->subscribe_with_filter(const_cast<char*>(topic), lava_msg, lava_msg_listener_.get());

	//release sdn msg
	sfa::msg::DeleteMessage(&sdn_msg);
}

void subscribe_manager::unsubscribe(_sfa_in_ const std::string &ic, _sfa_in_ const std::set<int> &fid_set, 
	_sfa_in_ const int sub_flg) {
	//check
	if (!lava_subscribe_enable_) {
		sfa_log_warn(sfa_subscribe_manager, "[subscribe]: lava_subscribe_enable_ false!");
		return;
	}
	if ((ic.empty()) || (fid_set.empty())) {
		sfa_log_warn(sfa_subscribe_manager, "[subscribe]: ic or fid_set empty!");
		return;
	}

	//get sdn message
	msg::IMessage *sdn_msg = get_sdnmsg_common_sub(ic, fid_set, SubscribeType::unscribe);
	if (nullptr == sdn_msg) {
		sfa_log_warn(sfa_subscribe_manager, "[subscribe]: sdn_msg is nullptr!");
		return;
	}

	//get ceda message
	lava::comm::i_message *lava_msg = get_req_ceda_msg(sdn_msg, 1);
	if (nullptr == lava_msg) {
		sfa_log_warn(sfa_subscribe_manager, "[subscribe]: lava_msg is nullptr!");
		return;
	}

	//commit to lava
	const char *topic = nullptr;
	if (template_subscribe == sub_flg) {
		topic = ceda_template_topic;
	}
	else if (common_subscribe == sub_flg) {
		topic = TOPIC_API_STREAM_SUB;
	}
	else {
		sfa_log_warn(sfa_subscribe_manager, "[subscribe]: sub_flg : %d failed!", sub_flg);
		return;
	}
	lava_proxy_->unsubscribe_with_filter(const_cast<char*>(topic), lava_msg, lava_msg_listener_.get());

	//release sdn msg
	sfa::msg::DeleteMessage(&sdn_msg);
}

msg::IMessage *subscribe_manager::get_sdnmsg_common_sub(_sfa_in_ const std::string &ic, _sfa_in_ const std::set<int> &fid_set, 
	_sfa_in_ const SubscribeType &sub_type) {
	//create sdn message for common subscribe and fill it
	msg::IMessage* sdnmsg = sfa::msg::NewMessage();
	sdnmsg->GetHeader()->SetMessageType(sfa::msg::IMessageHeader::MessageType::FULL);
	sdnmsg->GetBody()->AddField(1, &sfa::msg::AsciiValue(/*"CN190210.IBBP=SSCP"*/ic));			//ic
	sdnmsg->GetBody()->AddField(2, &sfa::msg::IntegerValue(static_cast<int64_t>(sub_type)));	//subscribe type
	//set filter fields
	sfa::msg::KVArrayValue kv_array;
	kv_array.SetColType(4, sfa::msg::FieldType::INTEGER);
	for (auto &it : fid_set) {
		kv_array.AddRow()->SetValue(4, &sfa::msg::IntegerValue(it));
	}
	sdnmsg->GetBody()->AddField(3, &sfa::msg::KVArrayValue(kv_array));
	//return sdnmsg
	if (sdnmsg) {
		return sdnmsg;
	}
	else {
		return nullptr;
	}
}

lava::comm::i_message *subscribe_manager::get_req_ceda_msg(_sfa_in_ const sfa::msg::IMessage* sdnmsg_req, 
	_sfa_in_ const int sub_flg) {
	if (nullptr == sdnmsg_req) {
		return nullptr;
	}

	//encode sdn message
	uint8_t* buf = nullptr;
	uint32_t buf_len = 0;
	bool encode_succeed = sfa::msg::EncodeMessage(sdnmsg_req, msg_context_, buf, buf_len);
	if (!encode_succeed) {
		return nullptr;
	}

	//create ceda message
	lava::comm::i_message *cedamsg_req = get_message(MSG_TYPE_DATA_REQ);
	if (nullptr == cedamsg_req) {
		return nullptr;
	}

	//fill ceda message and return it
	const char *svr_id = nullptr;
	const char *topic = nullptr;
	if (template_subscribe == sub_flg) {
		svr_id = ceda_template_svrid;
		topic = ceda_template_topic;
		cedamsg_req->set_svr_id(svr_id, strlen(svr_id));
	}
	else if (common_subscribe == sub_flg) {
		topic = TOPIC_API_STREAM_SUB;
	}
	else {
		sfa_log_warn(sfa_subscribe_manager, "[get_req_ceda_msg]: sub_flg : %d failed!", sub_flg);
		return nullptr;
	}
	cedamsg_req->set_destination(topic, strlen(topic));
	cedamsg_req->get_message_body()->add_bytes(3, (char*)buf, buf_len);
	return cedamsg_req;
}

void subscribe_manager::unsubscribe_all() {
	sfa::utils::sfa_c_autolock guard(ic_fids_dict_);
	for (auto &it : ic_fids_dict_) {
		unsubscribe(it.first, it.second, common_subscribe);
	}
}

}
}