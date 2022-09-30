//
//  subscribe_manager.h
//
//  Created by Lei.wang on 2019-08-26.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once 
#include <string>
#include <lava/lava_box/lava_box_api.h>
#include <sfa/dds/dds_listener.h>
#include <sfa/sfa_macro.h>
#include "sfa_utils/sfa_base.h"
#include "dds_proxy_impl.h"
#include "sfa_utils/sfa_container.h"
#include "sfa_utils/thread_pool.h"
#include "sfa_utils/sfa_log.h"

#define sfa_subscribe_manager		"sfa.subscribe_manager"
#define ceda_template_topic			"sdn.cdh.rdc.template"
#define ceda_template_svrid			"SDN_CDH_RDC"
#define sequence_number				20288
#define template_subscribe			0
#define common_subscribe			1

namespace sfa {
namespace dds {

class subscribe_manager;
//lava event listener
class lava_event_listener : public lava::comm::i_event_listener {
public:
	inline lava_event_listener(subscribe_manager *sub_mgr) : sub_mgr_(sub_mgr) {
		if (nullptr == sub_mgr_) {
			sfa_log_error(sfa_subscribe_manager, "[lava_event_listener]: get subscribe manager failed!");
		}
	}
	virtual inline ~lava_event_listener() {
		sub_mgr_ = nullptr;
	}

private:
	virtual void on_event(int32_t event_code, void* event_data) override;
	//no need implement
	virtual void on_exception(int32_t error_code, const char* what) override {}

private:
	subscribe_manager		*sub_mgr_;
	i_object_impl(std::atomic<int32_t>)
};

//lava message listener
class lava_msg_listener : public lava::comm::i_message_listener {
public:
	inline lava_msg_listener(subscribe_manager *sub_mgr, sfa::msg::IMessageContext* msg_context) 
		: sub_mgr_(sub_mgr), msg_context_(msg_context) {
		if (nullptr == sub_mgr_) {
			sfa_log_error(sfa_subscribe_manager, "[lava_msg_listener]: get subscribe manager failed!");
		}
		if (nullptr == msg_context_) {
			sfa_log_error(sfa_subscribe_manager, "[lava_msg_listener]: get message context failed!");
		}
	}
	virtual inline ~lava_msg_listener() {
		sub_mgr_ = nullptr;
		msg_context_ = nullptr;
	}

private:
	virtual void on_reply(__lv_in lava::comm::i_message* msg, __lv_in const char* topic) override;
	//no need implement
	virtual void on_message(__lv_in lava::comm::i_message* msg) override {}

private:
	bool parse_ack_ceda_msg(_sfa_in_ const lava::comm::i_message *ceda_msg, _sfa_out_ msg::IMessage *sdn_msg, 
		_sfa_out_ std::string &ic);

private:
	subscribe_manager			*sub_mgr_;
	sfa::msg::IMessageContext	*msg_context_;
	i_object_impl(std::atomic<int32_t>)
};

//subscribe type to dds server
enum class SubscribeType : uint8_t {
	snapshoot = 0,			//only subscribe all data one time
	snapshoot_update = 1,	//subscribe all data and update
	unscribe = 2			//unsubscribe
};

typedef struct icInformation {
	icInformation() : ic_(""), listener_addr_(0) {
		fids_set_.clear();
	}
	std::string				ic_;
	std::set<int>			fids_set_;
	long					listener_addr_;
} ICInfo;

typedef struct dispatchMsgInfo : public std::mutex {
	dispatchMsgInfo() : dispatch_flg_(false), sequence_id_("") {}
	bool					dispatch_flg_;
	std::string				sequence_id_;
} dispatchInfo;

class subscriber;
typedef sfa::utils::map_lv<std::string, std::set<int> >				ic_fids_map;
typedef sfa::utils::map_lv<std::string, int>						ic_ref_count_map;
typedef sfa::utils::map_lv<std::string, subscriber*>				subscriber_map;
typedef sfa::utils::map_lv<std::string, std::set<std::string> >		ic_sub_keys_map;

class subscriber {
public:
	subscriber(std::string &ic, std::set<int> *fids, sfa::base::i_auto_ptr<IMsgListener> listener, 
		sfa::utils::threadpool *thread_pool);
	~subscriber();

public:
	void set_message(_sfa_in_ const std::string &ic, _sfa_in_ const msg::IMessage *msg);

private:
	std::string								ic_;
	std::set<int>							*fids_;
	sfa::base::i_auto_ptr<IMsgListener>		listener_;
	sfa::utils::threadpool					*thread_pool_;
	dispatchInfo							*dispatch_info_;
};

//Interfaces of the subscribe manager
struct i_subscribe_manager {
	virtual void set_session(_sfa_in_ const ISession *session, _sfa_in_ const sfa::msg::IMessageContext *context) = 0;
	virtual bool register_listener(_sfa_in_ const std::string &ic, _sfa_in_ const std::vector<int> &filter_fields, 
		_sfa_in_ IMsgListener* listener) = 0;
	virtual bool unregister_listener(_sfa_in_ const std::string &ic, _sfa_in_ const std::vector<int> &filter_fields, 
		_sfa_in_ IMsgListener* listener) = 0;
};

class subscribe_manager : public i_subscribe_manager {
public:
	subscribe_manager();
	~subscribe_manager();

public:
	virtual void set_session(_sfa_in_ const ISession *session, _sfa_in_ const sfa::msg::IMessageContext *context) override;
	virtual bool register_listener(_sfa_in_ const std::string &ic, _sfa_in_ const std::vector<int> &filter_fields, 
		_sfa_in_ IMsgListener* listener) override;
	virtual bool unregister_listener(_sfa_in_ const std::string &ic, _sfa_in_ const std::vector<int> &filter_fields, 
		_sfa_in_ IMsgListener* listener) override;

public:
	void re_subscribe();
	void set_lava_sub_flg(_sfa_in_ bool enable);
	bool get_lava_sub_flg();
	void dispatch_sdn_msg(_sfa_in_ const std::string &ic, _sfa_in_ msg::IMessage *sdn_msg);

private:
	std::string get_subscriber_key(_sfa_in_ const ICInfo &icinfo);
	void subscribe(_sfa_in_ const std::string &ic, _sfa_in_ const std::set<int> &fid_set, 
		_sfa_in_ const SubscribeType &sub_type, _sfa_in_ const int sub_flg);
	void unsubscribe(_sfa_in_ const std::string &ic, _sfa_in_ const std::set<int> &fid_set, _sfa_in_ const int sub_flg);
	msg::IMessage *get_sdnmsg_common_sub(_sfa_in_ const std::string &ic, _sfa_in_ const std::set<int> &fid_set, 
		_sfa_in_ const SubscribeType &sub_type);
	lava::comm::i_message *get_req_ceda_msg(_sfa_in_ const msg::IMessage* sdnmsg_req, _sfa_in_ const int sub_flg);
	void unsubscribe_all();

private:
	ISession					*lava_session_;
	sfa::msg::IMessageContext	*msg_context_;
	bool						lava_session_flg_;
	bool						lava_subscribe_enable_;
	sfa::utils::threadpool		*executor_;
	ic_fids_map					ic_fids_dict_;			//find the max union filter fields using ic
	ic_ref_count_map			ic_ref_count_;			//the reference count of ic
	ic_sub_keys_map				ic_subscriber_keys_;	//find the the keys set using ic
	subscriber_map				obj_subscribers_;		//maintain all subscribers object
	lava::base::i_auto_ptr<lava::box::i_proxy>				lava_proxy_;
	lava::base::i_auto_ptr<lava::comm::i_event_listener>	lava_event_listener_;
	lava::base::i_auto_ptr<lava::comm::i_message_listener>	lava_msg_listener_;
};

}
}
