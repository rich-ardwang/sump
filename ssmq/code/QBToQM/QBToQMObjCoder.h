#pragma once
#include "QB_QMDefine.h"

using namespace QB_QM;

class CQBToQMObjCoder
{
public:
	CQBToQMObjCoder(void);
	~CQBToQMObjCoder(void);

	static void Encode_LstStr(std::list<std::string>& lst);
	static void Decode_LstStr(std::list<std::string>& lst);
    static void Encode_SetStr(SetString& st);
    static void Decode_SetStr(SetString& st);
	static void Encode_Str(std::string& str);
	static void Decode_Str(std::string& str);

	static void Encode_LstMultilanguageStr(LstMultilanguageStr& lst);
	static void Decode_LstMultilanguageStr(LstMultilanguageStr& lst);
	static void Encode_MultilanguageStr(MultilanguageStr& Item);
	static void Decode_MultilanguageStr(MultilanguageStr& Item);

	static void Encode_KeyValues(KeyValues& kvs);
	static void Decode_KeyValues(KeyValues& kvs);

	static void Encode_LstTermInfo(LstTermInfo& lst);
	static void Decode_LstTermInfo(LstTermInfo& lst);
	static void Encode_TermInfo(TermInfo& Item);
	static void Decode_TermInfo(TermInfo& Item);

	static void Encode_LstQuotation(LstQuotation& lst);
	static void Decode_LstQuotation(LstQuotation& lst);
	static void Encode_Quotation(Quotation& Item);
	static void Decode_Quotation(Quotation& Item);

	static void Encode_LstQuotationMngInfo(LstQuotationMngInfo& lst);
	static void Decode_LstQuotationMngInfo(LstQuotationMngInfo& lst);
	static void Encode_QuotationMngInfo(QuotationMngInfo& Item);
	static void Decode_QuotationMngInfo(QuotationMngInfo& Item);

	static void Encode_LstQuotSendInfo(LstQuotSendInfo& lst);
	static void Decode_LstQuotSendInfo(LstQuotSendInfo& lst);
	static void Encode_QuotSendInfo(QuotSendInfo& Item);
	static void Decode_QuotSendInfo(QuotSendInfo& Item);

	static void Encode_LstQuotRecvInfo(LstQuotRecvInfo& lst);
	static void Decode_LstQuotRecvInfo(LstQuotRecvInfo& lst);
	static void Encode_QuotRecvInfo(QuotRecvInfo& Item);
	static void Decode_QuotRecvInfo(QuotRecvInfo& Item);

	static void Encode_LstQuotReplyInfo(LstQuotReplyInfo& lst);
	static void Decode_LstQuotReplyInfo(LstQuotReplyInfo& lst);
	static void Encode_QuotReplyInfo(QuotReplyInfo& Item);
	static void Decode_QuotReplyInfo(QuotReplyInfo& Item);

	static void Encode_LstPersonInfo(LstPersonInfo& lst);
	static void Decode_LstPersonInfo(LstPersonInfo& lst);
	static void Encode_PersonInfo(PersonInfo& Item);
	static void Decode_PersonInfo(PersonInfo& Item);

	static void Encode_LstGroupInfo(LstGroupInfo& lst);
	static void Decode_LstGroupInfo(LstGroupInfo& lst);
	static void Encode_GroupInfo(GroupInfo& Item);
	static void Decode_GroupInfo(GroupInfo& Item);

	static void Encode_LstBondItem(LstBondItem& lst);
	static void Decode_LstBondItem(LstBondItem& lst);
	static void Encode_BondItem(BondItem& Item);
	static void Decode_BondItem(BondItem& Item);

	static void Encode_LstMsgInfo(LstMsgInfo& lst);
	static void Decode_LstMsgInfo(LstMsgInfo& lst);
	static void Encode_MsgInfo(MsgInfo& Item);
	static void Decode_MsgInfo(MsgInfo& Item); 

	static void Encode_LstBondQuotInfo(LstBondQuotInfo& lst);
	static void Decode_LstBondQuotInfo(LstBondQuotInfo& lst);
	static void Encode_BondQuotInfo(BondQuotInfo& Item);
	static void Decode_BondQuotInfo(BondQuotInfo& Item);

	static void Encode_LstBondQuotByCode(LstBondQuotByCode& lst);
	static void Decode_LstBondQuotByCode(LstBondQuotByCode& lst);
	static void Encode_BondQuotByCode(BondQuotByCode& Item);
	static void Decode_BondQuotByCode(BondQuotByCode& Item); 

	static void Encode_LstBondQuotByCodeSendInfo(LstBondQuotByCodeSendInfo& lst);
	static void Decode_LstBondQuotByCodeSendInfo(LstBondQuotByCodeSendInfo& lst);
	static void Encode_BondQuotByCodeSendInfo(BondQuotByCodeSendInfo& Item);
	static void Decode_BondQuotByCodeSendInfo(BondQuotByCodeSendInfo& Item); 

	static void Encode_LstCondition(LstCondition& lst);
	static void Decode_LstCondition(LstCondition& lst);
	static void Encode_Condition(Condition& Item);
	static void Decode_Condition(Condition& Item); 

	static void Encode_LstBondQuotByCondition(LstBondQuotByCondition& lst);
	static void Decode_LstBondQuotByCondition(LstBondQuotByCondition& lst);
	static void Encode_BondQuotByCondition(BondQuotByCondition& Item);
	static void Decode_BondQuotByCondition(BondQuotByCondition& Item); 

	static void Encode_LstBondQuotByConditionSendInfo(LstBondQuotByConditionSendInfo& lst);
	static void Decode_LstBondQuotByConditionSendInfo(LstBondQuotByConditionSendInfo& lst);
	static void Encode_BondQuotByConditionSendInfo(BondQuotByConditionSendInfo& Item);
	static void Decode_BondQuotByConditionSendInfo(BondQuotByConditionSendInfo& Item); 

    static void Encode_ContactSearch(ContactSearch& Item);
    static void Decode_ContactSearch(ContactSearch& Item);

    static void Encode_LstSearchUserInfo(LstSearchUserInfo& lst);
    static void Decode_LstSearchUserInfo(LstSearchUserInfo& lst);
    static void Encode_SearchUserInfo(SearchUserInfo& Item);
    static void Decode_SearchUserInfo(SearchUserInfo& Item);

    static void Encode_LstMMQuotSendInfoV2(LstMMQuotSendInfoV2& lst);
    static void Decode_LstMMQuotSendInfoV2(LstMMQuotSendInfoV2& lst);
    static void Encode_QuotSendInfoV2_MM(QuotSendInfoV2_MM& Item);
    static void Decode_QuotSendInfoV2_MM(QuotSendInfoV2_MM& Item);

    static void Encode_LstBondQuotByCodeSendInfoV2(LstBondQuotByCodeSendInfoV2& lst);
    static void Decode_LstBondQuotByCodeSendInfoV2(LstBondQuotByCodeSendInfoV2& lst);
    static void Encode_QuotSendInfoV2_BondByCode(QuotSendInfoV2_BondByCode& Item);
    static void Decode_QuotSendInfoV2_BondByCode(QuotSendInfoV2_BondByCode& Item);

    static void Encode_BondQuotByCodeReplyInfo(BondQuotByCodeReplyInfo& Item);
    static void Decode_BondQuotByCodeReplyInfo(BondQuotByCodeReplyInfo& Item);

    static void Encode_PurchaseInfo(PurchaseInfo& Item);
    static void Decode_PurchaseInfo(PurchaseInfo& Item);

    static void Encode_LstQBAccount2QMID(LstQBAccount2QMID& lst);
    static void Decode_LstQBAccount2QMID(LstQBAccount2QMID& lst);
    static void Encode_QBAccount2QMID(QBAccount2QMID& Item);
    static void Decode_QBAccount2QMID(QBAccount2QMID& Item);

    static void Encode_LstQMRKey(LstQMRKey& lst);
    static void Decode_LstQMRKey(LstQMRKey& lst);
    static void Encode_QMRemindKeyItem(QMRemindKeyItem& Item);
    static void Decode_QMRemindKeyItem(QMRemindKeyItem& Item);

    static void Encode_LstQMRFriend(LstQMRFriend& lst);
    static void Decode_LstQMRFriend(LstQMRFriend& lst);
    static void Encode_QMRemindFriendItem(QMRemindFriendItem& Item);
    static void Decode_QMRemindFriendItem(QMRemindFriendItem& Item);

    static void Encode_QMRemindConfig(QMRemindConfig& Item);
    static void Decode_QMRemindConfig(QMRemindConfig& Item);

    static void Encode_LstBondShare(LstBondShare& lst);
    static void Decode_LstBondShare(LstBondShare& lst);
    static void Encode_BondShareItem(BondShareItem& Item);
    static void Decode_BondShareItem(BondShareItem& Item);

	static void Encode_NewsInfo(NewsInfo &info);
	static void Decode_NewsInfo(NewsInfo &info);

    static void Encode_NewsInfoV2(NewsInfoV2 &info);
    static void Decode_NewsInfoV2(NewsInfoV2 &info);

    static void Encode_PicInfo(PicInfo &info);
    static void Decode_PiceInfo(PicInfo &info);

    static void Encode_NoInstituteContact(NoInstituteContact &item);
    static void Decode_NoInstituteContact(NoInstituteContact &item);

};
