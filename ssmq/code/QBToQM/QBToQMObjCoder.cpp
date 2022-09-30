#include "StdAfx.h"
#include "QBToQMObjCoder.h"
#include "BaseStringFunc.h"

CQBToQMObjCoder::CQBToQMObjCoder(void)
{
}

CQBToQMObjCoder::~CQBToQMObjCoder(void)
{
}

void CQBToQMObjCoder::Encode_LstStr(std::list<std::string>& lst)
{
	for (std::list<std::string>::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Encode_Str(*iter);
	}
}

void CQBToQMObjCoder::Decode_LstStr(std::list<std::string>& lst)
{
	for (std::list<std::string>::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Decode_Str(*iter);
	}
}

void CQBToQMObjCoder::Encode_SetStr(SetString& st)
{
	//VS2010开始set的iterator类型变为const的引用类型
	//为保证set内部数据结构的正确,采用临时变量处理
	SetString setTmp = st;
	st.clear();
    for (SetString::iterator iter = setTmp.begin(); iter != setTmp.end(); ++iter)
    {
		std::string sItem = *iter;
        Encode_Str(sItem);
		st.insert(sItem);
    }
}

void CQBToQMObjCoder::Decode_SetStr(SetString& st)
{
	//VS2010开始set的iterator类型变为const的引用类型
	//为保证set内部数据结构的正确,采用临时变量处理
	SetString setTmp = st;
	st.clear();
	for (SetString::iterator iter = setTmp.begin(); iter != setTmp.end(); ++iter)
	{
		std::string sItem = *iter;
		Decode_Str(sItem);
		st.insert(sItem);
	}
}

void CQBToQMObjCoder::Encode_Str(std::string& str)
{
	str = SS_BASE::GBK2Utf8A(str);
}

void CQBToQMObjCoder::Decode_Str(std::string& str)
{
	str = SS_BASE::Utf8ToGBKA(str);
}

void CQBToQMObjCoder::Encode_LstMultilanguageStr(LstMultilanguageStr& lst)
{
	for (LstMultilanguageStr::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Encode_MultilanguageStr(*iter);
	}
}

void CQBToQMObjCoder::Decode_LstMultilanguageStr(LstMultilanguageStr& lst)
{
	for (LstMultilanguageStr::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Decode_MultilanguageStr(*iter);
	}
}

void CQBToQMObjCoder::Encode_MultilanguageStr(MultilanguageStr& Item)
{
	Encode_Str(Item.sDisplay);
	Encode_Str(Item.sEnumVal);
}

void CQBToQMObjCoder::Decode_MultilanguageStr(MultilanguageStr& Item)
{
	Decode_Str(Item.sDisplay);
	Decode_Str(Item.sEnumVal);
}

void CQBToQMObjCoder::Encode_KeyValues(KeyValues& kvs)
{
	for (KeyValues::iterator iter = kvs.begin(); iter != kvs.end(); ++iter)
	{
		Encode_Str(iter->second);
	}
}

void CQBToQMObjCoder::Decode_KeyValues(KeyValues& kvs)
{
	for (KeyValues::iterator iter = kvs.begin(); iter != kvs.end(); ++iter)
	{
		Decode_Str(iter->second);
	}
}

void CQBToQMObjCoder::Encode_LstTermInfo(LstTermInfo& lst)
{
	for (LstTermInfo::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Encode_TermInfo(*iter);
	}
}

void CQBToQMObjCoder::Decode_LstTermInfo(LstTermInfo& lst)
{
	for (LstTermInfo::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Decode_TermInfo(*iter);
	}
}

void CQBToQMObjCoder::Encode_TermInfo(TermInfo& Item)
{
	Encode_Str(Item.sDisplay);
	Encode_LstStr(Item.LstEnumVal);
}

void CQBToQMObjCoder::Decode_TermInfo(TermInfo& Item)
{
	Decode_Str(Item.sDisplay);
	Decode_LstStr(Item.LstEnumVal);
}

void CQBToQMObjCoder::Encode_LstQuotation(LstQuotation& lst)
{
	for (LstQuotation::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Encode_Quotation(*iter);
	}
}

void CQBToQMObjCoder::Decode_LstQuotation(LstQuotation& lst)
{
	for (LstQuotation::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Decode_Quotation(*iter);
	}
}

void CQBToQMObjCoder::Encode_Quotation(Quotation& Item)
{
	Encode_Str(Item.sQuotationID);
	Encode_MultilanguageStr(Item.sDirect);
	Encode_MultilanguageStr(Item.sAssetsType);
	Encode_LstTermInfo(Item.lstTerms);
	Encode_MultilanguageStr(Item.sCount);
	Encode_MultilanguageStr(Item.sPrice);
	Encode_LstMultilanguageStr(Item.lstTags);
	Encode_Str(Item.sMemo);
}

void CQBToQMObjCoder::Decode_Quotation(Quotation& Item)
{
	Decode_Str(Item.sQuotationID);
	Decode_MultilanguageStr(Item.sDirect);
	Decode_MultilanguageStr(Item.sAssetsType);
	Decode_LstTermInfo(Item.lstTerms);
	Decode_MultilanguageStr(Item.sCount);
	Decode_MultilanguageStr(Item.sPrice);
	Decode_LstMultilanguageStr(Item.lstTags);
	Decode_Str(Item.sMemo);
}

void CQBToQMObjCoder::Encode_LstQuotationMngInfo(LstQuotationMngInfo& lst)
{
	for (LstQuotationMngInfo::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Encode_QuotationMngInfo(*iter);
	}
}

void CQBToQMObjCoder::Decode_LstQuotationMngInfo(LstQuotationMngInfo& lst)
{
	for (LstQuotationMngInfo::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Decode_QuotationMngInfo(*iter);
	}
}

void CQBToQMObjCoder::Encode_QuotationMngInfo(QuotationMngInfo& Item)
{
	Encode_Quotation(Item.Quot);
}

void CQBToQMObjCoder::Decode_QuotationMngInfo(QuotationMngInfo& Item)
{
	Decode_Quotation(Item.Quot);
}

void CQBToQMObjCoder::Encode_LstQuotSendInfo(LstQuotSendInfo& lst)
{
	for (LstQuotSendInfo::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Encode_QuotSendInfo(*iter);
	}
}

void CQBToQMObjCoder::Decode_LstQuotSendInfo(LstQuotSendInfo& lst)
{
	for (LstQuotSendInfo::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Decode_QuotSendInfo(*iter);
	}
}

void CQBToQMObjCoder::Encode_QuotSendInfo(QuotSendInfo& Item)
{
	Encode_LstQuotation(Item.lstQuots);
}

void CQBToQMObjCoder::Decode_QuotSendInfo(QuotSendInfo& Item)
{
	Decode_LstQuotation(Item.lstQuots);
}

void CQBToQMObjCoder::Encode_LstQuotRecvInfo(LstQuotRecvInfo& lst)
{
	for (LstQuotRecvInfo::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Encode_QuotRecvInfo(*iter);
	}
}

void CQBToQMObjCoder::Decode_LstQuotRecvInfo(LstQuotRecvInfo& lst)
{
	for (LstQuotRecvInfo::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Decode_QuotRecvInfo(*iter);
	}
}

void CQBToQMObjCoder::Encode_QuotRecvInfo(QuotRecvInfo& Item)
{
	Encode_LstQuotation(Item.lstQuots);
}

void CQBToQMObjCoder::Decode_QuotRecvInfo(QuotRecvInfo& Item)
{
	Decode_LstQuotation(Item.lstQuots);
}

void CQBToQMObjCoder::Encode_LstQuotReplyInfo(LstQuotReplyInfo& lst)
{
	for (LstQuotReplyInfo::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Encode_QuotReplyInfo(*iter);
	}
}

void CQBToQMObjCoder::Decode_LstQuotReplyInfo(LstQuotReplyInfo& lst)
{
	for (LstQuotReplyInfo::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Decode_QuotReplyInfo(*iter);
	}
}

void CQBToQMObjCoder::Encode_QuotReplyInfo(QuotReplyInfo& Item)
{
	Encode_Quotation(Item.Quot);
}

void CQBToQMObjCoder::Decode_QuotReplyInfo(QuotReplyInfo& Item)
{
	Decode_Quotation(Item.Quot);
}

void CQBToQMObjCoder::Encode_LstPersonInfo(LstPersonInfo& lst)
{
	for (LstPersonInfo::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Encode_PersonInfo(*iter);
	}
}

void CQBToQMObjCoder::Decode_LstPersonInfo(LstPersonInfo& lst)
{
	for (LstPersonInfo::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Decode_PersonInfo(*iter);
	}
}

void CQBToQMObjCoder::Encode_PersonInfo(PersonInfo& Item)
{
	Encode_Str(Item.sDisplayName);
	Encode_Str(Item.sTel);
	Encode_Str(Item.sMobile);
	Encode_Str(Item.sInstitution);
	Encode_Str(Item.sInstitutionType);
    Encode_Str(Item.sQBAccount);
}

void CQBToQMObjCoder::Decode_PersonInfo(PersonInfo& Item)
{
	Decode_Str(Item.sDisplayName);
	Decode_Str(Item.sTel);
	Decode_Str(Item.sMobile);
	Decode_Str(Item.sInstitution);
	Decode_Str(Item.sInstitutionType);
    Decode_Str(Item.sQBAccount);
}

void CQBToQMObjCoder::Encode_LstGroupInfo(LstGroupInfo& lst)
{
	for (LstGroupInfo::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Encode_GroupInfo(*iter);
	}
}

void CQBToQMObjCoder::Decode_LstGroupInfo(LstGroupInfo& lst)
{
	for (LstGroupInfo::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Decode_GroupInfo(*iter);
	}
}

void CQBToQMObjCoder::Encode_GroupInfo(GroupInfo& Item)
{
	Encode_Str(Item.sDisplayName);
}

void CQBToQMObjCoder::Decode_GroupInfo(GroupInfo& Item)
{
	Decode_Str(Item.sDisplayName);
}

void CQBToQMObjCoder::Encode_LstBondItem(LstBondItem& lst)
{
	for (LstBondItem::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Encode_BondItem(*iter);
	}
}

void CQBToQMObjCoder::Decode_LstBondItem(LstBondItem& lst)
{
	for (LstBondItem::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Decode_BondItem(*iter);
	}
}

void CQBToQMObjCoder::Encode_BondItem(BondItem& Item)
{
	//Encode_Str(Item.m_sBondKey);							
	//Encode_Str(Item.m_sBondID);							
	//Encode_Str(Item.m_sListedMarket);					
	//Encode_Str(Item.m_sCombBondKey);						
	//Encode_Str(Item.m_sCombBondID);						
	Encode_Str(Item.m_sFullName);						
	Encode_Str(Item.m_sShortName);						
	//Encode_Str(Item.m_sFullNameEn);						
	//Encode_Str(Item.m_sShortNameEn);						
	//Encode_Str(Item.m_sJianpin);							
	//Encode_Str(Item.m_sQuanpin);							
	//Encode_Str(Item.m_sMarketType);						
	//Encode_Str(Item.m_sBondSubType);						
	//Encode_Str(Item.m_sIssuerRating);					
	//Encode_Str(Item.m_sBondRating);						
	//Encode_Str(Item.m_sListDate);						
	//Encode_Str(Item.m_sInterestStartDate);				
	//Encode_Str(Item.m_sMaturityDate);					
	//Encode_Str(Item.m_sPaymentDate);						
	//Encode_Str(Item.m_sOptionDate);						
	//Encode_Str(Item.m_sRateType);						
	//Encode_Str(Item.m_sOptionType);						
	//Encode_Str(Item.m_sMaturityTerm);					
	//Encode_Str(Item.m_sTermToMaturity);					
	//Encode_Str(Item.m_sFRNIndexID);						

	//Encode_Str(Item.m_sIssuerCode);						
	//Encode_Str(Item.m_sInstitutionType);					
	//Encode_Str(Item.m_sYieldCurveType);					
	//Encode_Str(Item.m_sEstcleanprice);					
	//Encode_Str(Item.m_sEstPrice);						

	//Encode_Str(Item.m_sAuctionDateStart);				
	//Encode_Str(Item.m_sAuctionDateEnd);					
	//Encode_Str(Item.m_sAnnounceDate);					
	//Encode_Str(Item.m_sIssueStartDate);					
	//Encode_Str(Item.m_sIssueEndDate);					
	//Encode_Str(Item.m_sIsUnderwriter);					
	//
	//Encode_Str(Item.m_sMortgate);						
	//Encode_Str(Item.m_sCrossmarket);						
	//Encode_Str(Item.m_sIsMunicipal);						
	Encode_Str(Item.m_sRemainDate);	
}

void CQBToQMObjCoder::Decode_BondItem(BondItem& Item)
{
	//Decode_Str(Item.m_sBondKey);							
	//Decode_Str(Item.m_sBondID);							
	//Decode_Str(Item.m_sListedMarket);					
	//Decode_Str(Item.m_sCombBondKey);						
	//Decode_Str(Item.m_sCombBondID);						
	Decode_Str(Item.m_sFullName);						
	Decode_Str(Item.m_sShortName);						
	//Decode_Str(Item.m_sFullNameEn);						
	//Decode_Str(Item.m_sShortNameEn);						
	//Decode_Str(Item.m_sJianpin);							
	//Decode_Str(Item.m_sQuanpin);							
	//Decode_Str(Item.m_sMarketType);						
	//Decode_Str(Item.m_sBondSubType);						
	//Decode_Str(Item.m_sIssuerRating);					
	//Decode_Str(Item.m_sBondRating);						
	//Decode_Str(Item.m_sListDate);						
	//Decode_Str(Item.m_sInterestStartDate);				
	//Decode_Str(Item.m_sMaturityDate);					
	//Decode_Str(Item.m_sPaymentDate);						
	//Decode_Str(Item.m_sOptionDate);						
	//Decode_Str(Item.m_sRateType);						
	//Decode_Str(Item.m_sOptionType);						
	//Decode_Str(Item.m_sMaturityTerm);					
	//Decode_Str(Item.m_sTermToMaturity);					
	//Decode_Str(Item.m_sFRNIndexID);						

	//Decode_Str(Item.m_sIssuerCode);						
	//Decode_Str(Item.m_sInstitutionType);					
	//Decode_Str(Item.m_sYieldCurveType);					
	//Decode_Str(Item.m_sEstcleanprice);					
	//Decode_Str(Item.m_sEstPrice);						

	//Decode_Str(Item.m_sAuctionDateStart);				
	//Decode_Str(Item.m_sAuctionDateEnd);					
	//Decode_Str(Item.m_sAnnounceDate);					
	//Decode_Str(Item.m_sIssueStartDate);					
	//Decode_Str(Item.m_sIssueEndDate);					
	//Decode_Str(Item.m_sIsUnderwriter);					

	//Decode_Str(Item.m_sMortgate);						
	//Decode_Str(Item.m_sCrossmarket);						
	//Decode_Str(Item.m_sIsMunicipal);						
	Decode_Str(Item.m_sRemainDate);		
}

void CQBToQMObjCoder::Encode_LstMsgInfo(LstMsgInfo& lst)
{
	for (LstMsgInfo::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Encode_MsgInfo(*iter);
	}
}

void CQBToQMObjCoder::Decode_LstMsgInfo(LstMsgInfo& lst)
{
	for (LstMsgInfo::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Decode_MsgInfo(*iter);
	}
}

void CQBToQMObjCoder::Encode_MsgInfo(MsgInfo& Item)
{
	Encode_Str(Item.sMsgDisplay);
}

void CQBToQMObjCoder::Decode_MsgInfo(MsgInfo& Item)
{
	Decode_Str(Item.sMsgDisplay);
}

void CQBToQMObjCoder::Encode_LstBondQuotInfo(LstBondQuotInfo& lst)
{
	for (LstBondQuotInfo::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Encode_BondQuotInfo(*iter);
	}
}

void CQBToQMObjCoder::Decode_LstBondQuotInfo(LstBondQuotInfo& lst)
{
	for (LstBondQuotInfo::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Decode_BondQuotInfo(*iter);
	}
}

void CQBToQMObjCoder::Encode_BondQuotInfo(BondQuotInfo& Item)
{
	Encode_Str(Item.sCombBondKey);
	Encode_Str(Item.sBid);
	Encode_Str(Item.sVolBid);
	Encode_Str(Item.sOfr);
	Encode_Str(Item.sVolOfr);
}

void CQBToQMObjCoder::Decode_BondQuotInfo(BondQuotInfo& Item)
{
	Decode_Str(Item.sCombBondKey);
	Decode_Str(Item.sBid);
	Decode_Str(Item.sVolBid);
	Decode_Str(Item.sOfr);
	Decode_Str(Item.sVolOfr);
}

void CQBToQMObjCoder::Encode_LstBondQuotByCode(LstBondQuotByCode& lst)
{
	for (LstBondQuotByCode::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Encode_BondQuotByCode(*iter);
	}
}

void CQBToQMObjCoder::Decode_LstBondQuotByCode(LstBondQuotByCode& lst)
{
	for (LstBondQuotByCode::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Decode_BondQuotByCode(*iter);
	}
}

void CQBToQMObjCoder::Encode_BondQuotByCode(BondQuotByCode& Item)
{
	Encode_Str(Item.sQuotID);
	Encode_BondQuotInfo(Item.BQInfo);
	Encode_LstMultilanguageStr(Item.lstTags);
	Encode_Str(Item.sMemo);
}

void CQBToQMObjCoder::Decode_BondQuotByCode(BondQuotByCode& Item)
{
	Decode_Str(Item.sQuotID);
	Decode_BondQuotInfo(Item.BQInfo);
	Decode_LstMultilanguageStr(Item.lstTags);
	Decode_Str(Item.sMemo);
}

void CQBToQMObjCoder::Encode_LstBondQuotByCodeSendInfo(LstBondQuotByCodeSendInfo& lst)
{
	for (LstBondQuotByCodeSendInfo::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Encode_BondQuotByCodeSendInfo(*iter);
	}
}

void CQBToQMObjCoder::Decode_LstBondQuotByCodeSendInfo(LstBondQuotByCodeSendInfo& lst)
{
	for (LstBondQuotByCodeSendInfo::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Decode_BondQuotByCodeSendInfo(*iter);
	}
}

void CQBToQMObjCoder::Encode_BondQuotByCodeSendInfo(BondQuotByCodeSendInfo& Item)
{
	Encode_LstBondQuotByCode(Item.lstQuots);
}

void CQBToQMObjCoder::Decode_BondQuotByCodeSendInfo(BondQuotByCodeSendInfo& Item)
{
	Decode_LstBondQuotByCode(Item.lstQuots);
}

void CQBToQMObjCoder::Encode_LstCondition(LstCondition& lst)
{
	for (LstCondition::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Encode_Condition(*iter);
	}
}

void CQBToQMObjCoder::Decode_LstCondition(LstCondition& lst)
{
	for (LstCondition::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Decode_Condition(*iter);
	}
}

void CQBToQMObjCoder::Encode_Condition(Condition& Item)
{
	Encode_MultilanguageStr(Item.sName);
	Encode_KeyValues(Item.ConditionInfo);
}

void CQBToQMObjCoder::Decode_Condition(Condition& Item)
{
	Decode_MultilanguageStr(Item.sName);
	Decode_KeyValues(Item.ConditionInfo);
}

void CQBToQMObjCoder::Encode_LstBondQuotByCondition(LstBondQuotByCondition& lst)
{
	for (LstBondQuotByCondition::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Encode_BondQuotByCondition(*iter);
	}
}

void CQBToQMObjCoder::Decode_LstBondQuotByCondition(LstBondQuotByCondition& lst)
{
	for (LstBondQuotByCondition::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Decode_BondQuotByCondition(*iter);
	}
}

void CQBToQMObjCoder::Encode_BondQuotByCondition(BondQuotByCondition& Item)
{
	Encode_Str(Item.sQuotID);
	Encode_MultilanguageStr(Item.sDirect);
	Encode_Str(Item.sCount);
	Encode_LstCondition(Item.lstConditions);
	Encode_LstMultilanguageStr(Item.lstTags);
	Encode_Str(Item.sMemo);
}

void CQBToQMObjCoder::Decode_BondQuotByCondition(BondQuotByCondition& Item)
{
	Decode_Str(Item.sQuotID);
	Decode_MultilanguageStr(Item.sDirect);
	Decode_Str(Item.sCount);
	Decode_LstCondition(Item.lstConditions);
	Decode_LstMultilanguageStr(Item.lstTags);
	Decode_Str(Item.sMemo);
}

void CQBToQMObjCoder::Encode_LstBondQuotByConditionSendInfo(LstBondQuotByConditionSendInfo& lst)
{
	for (LstBondQuotByConditionSendInfo::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Encode_BondQuotByConditionSendInfo(*iter);
	}
}

void CQBToQMObjCoder::Decode_LstBondQuotByConditionSendInfo(LstBondQuotByConditionSendInfo& lst)
{
	for (LstBondQuotByConditionSendInfo::iterator iter = lst.begin(); iter != lst.end(); ++iter)
	{
		Decode_BondQuotByConditionSendInfo(*iter);
	}
}

void CQBToQMObjCoder::Encode_BondQuotByConditionSendInfo(BondQuotByConditionSendInfo& Item)
{
	Encode_LstBondQuotByCondition(Item.lstQuots);
}

void CQBToQMObjCoder::Decode_BondQuotByConditionSendInfo(BondQuotByConditionSendInfo& Item)
{
	Decode_LstBondQuotByCondition(Item.lstQuots);
}

void CQBToQMObjCoder::Encode_ContactSearch(ContactSearch& Item)
{
    Encode_Str(Item.sKey);
}

void CQBToQMObjCoder::Decode_ContactSearch(ContactSearch& Item)
{
    Decode_Str(Item.sKey);
}

void CQBToQMObjCoder::Encode_LstSearchUserInfo(LstSearchUserInfo& lst)
{
    for (LstSearchUserInfo::iterator iter = lst.begin(); iter != lst.end(); ++iter)
    {
        Encode_SearchUserInfo(*iter);
    }
}

void CQBToQMObjCoder::Decode_LstSearchUserInfo(LstSearchUserInfo& lst)
{
    for (LstSearchUserInfo::iterator iter = lst.begin(); iter != lst.end(); ++iter)
    {
        Decode_SearchUserInfo(*iter);
    }
}

void CQBToQMObjCoder::Encode_SearchUserInfo(SearchUserInfo& Item)
{
    Encode_Str(Item.sUserName);
    Encode_Str(Item.sCompanyName);
}

void CQBToQMObjCoder::Decode_SearchUserInfo(SearchUserInfo& Item)
{
    Decode_Str(Item.sUserName);
    Decode_Str(Item.sCompanyName);
}

void CQBToQMObjCoder::Encode_LstMMQuotSendInfoV2(LstMMQuotSendInfoV2& lst)
{
    for (LstMMQuotSendInfoV2::iterator iter = lst.begin(); iter != lst.end(); ++iter)
    {
        Encode_QuotSendInfoV2_MM(*iter);
    }
}

void CQBToQMObjCoder::Decode_LstMMQuotSendInfoV2(LstMMQuotSendInfoV2& lst)
{
    for (LstMMQuotSendInfoV2::iterator iter = lst.begin(); iter != lst.end(); ++iter)
    {
        Decode_QuotSendInfoV2_MM(*iter);
    }
}

void CQBToQMObjCoder::Encode_QuotSendInfoV2_MM(QuotSendInfoV2_MM& Item)
{
	Encode_LstQuotation(Item.lstQuots);
}

void CQBToQMObjCoder::Decode_QuotSendInfoV2_MM(QuotSendInfoV2_MM& Item)
{
    Decode_LstQuotation(Item.lstQuots);
}

void CQBToQMObjCoder::Encode_LstBondQuotByCodeSendInfoV2(LstBondQuotByCodeSendInfoV2& lst)
{
    for (LstBondQuotByCodeSendInfoV2::iterator iter = lst.begin(); iter != lst.end(); ++iter)
    {
        Encode_QuotSendInfoV2_BondByCode(*iter);
    }
}

void CQBToQMObjCoder::Decode_LstBondQuotByCodeSendInfoV2(LstBondQuotByCodeSendInfoV2& lst)
{
    for (LstBondQuotByCodeSendInfoV2::iterator iter = lst.begin(); iter != lst.end(); ++iter)
    {
        Decode_QuotSendInfoV2_BondByCode(*iter);
    }
}

void CQBToQMObjCoder::Encode_QuotSendInfoV2_BondByCode(QuotSendInfoV2_BondByCode& Item)
{
    Encode_LstBondQuotByCode(Item.lstQuots);
}

void CQBToQMObjCoder::Decode_QuotSendInfoV2_BondByCode(QuotSendInfoV2_BondByCode& Item)
{
    Decode_LstBondQuotByCode(Item.lstQuots);
}

void CQBToQMObjCoder::Encode_BondQuotByCodeReplyInfo(BondQuotByCodeReplyInfo& Item)
{
    Encode_BondQuotByCode(Item.Quot);
}

void CQBToQMObjCoder::Decode_BondQuotByCodeReplyInfo(BondQuotByCodeReplyInfo& Item)
{
    Decode_BondQuotByCode(Item.Quot);
}

void CQBToQMObjCoder::Encode_PurchaseInfo(PurchaseInfo& Item)
{
    Encode_Str(Item.sMsgDisplayP1);
    Encode_Str(Item.sMsgDisplayP2);
}

void CQBToQMObjCoder::Decode_PurchaseInfo(PurchaseInfo& Item)
{
    Decode_Str(Item.sMsgDisplayP1);
    Decode_Str(Item.sMsgDisplayP2);
}

void CQBToQMObjCoder::Encode_LstQBAccount2QMID(LstQBAccount2QMID& lst)
{
    for (LstQBAccount2QMID::iterator iter = lst.begin(); iter != lst.end(); ++iter)
    {
        Encode_QBAccount2QMID(*iter);
    }
}

void CQBToQMObjCoder::Decode_LstQBAccount2QMID(LstQBAccount2QMID& lst)
{
    for (LstQBAccount2QMID::iterator iter = lst.begin(); iter != lst.end(); ++iter)
    {
        Decode_QBAccount2QMID(*iter);
    }
}

void CQBToQMObjCoder::Encode_QBAccount2QMID(QBAccount2QMID& Item)
{
    Encode_Str(Item.sQBAccount);
}

void CQBToQMObjCoder::Decode_QBAccount2QMID(QBAccount2QMID& Item)
{
    Decode_Str(Item.sQBAccount);
}

void CQBToQMObjCoder::Encode_LstQMRKey(LstQMRKey& lst)
{
    for (LstQMRKey::iterator iter = lst.begin(); iter != lst.end(); ++iter)
    {
        Encode_QMRemindKeyItem(*iter);
    }
}
void CQBToQMObjCoder::Decode_LstQMRKey(LstQMRKey& lst)
{
    for (LstQMRKey::iterator iter = lst.begin(); iter != lst.end(); ++iter)
    {
        Decode_QMRemindKeyItem(*iter);
    }
}

void CQBToQMObjCoder::Encode_QMRemindKeyItem(QMRemindKeyItem& Item)
{
    Encode_Str(Item.sKeyWord);
}

void CQBToQMObjCoder::Decode_QMRemindKeyItem(QMRemindKeyItem& Item)
{
    Decode_Str(Item.sKeyWord);
}

void CQBToQMObjCoder::Encode_LstQMRFriend(LstQMRFriend& lst)
{
    for (LstQMRFriend::iterator iter = lst.begin(); iter != lst.end(); ++iter)
    {
        Encode_QMRemindFriendItem(*iter);
    }
}

void CQBToQMObjCoder::Decode_LstQMRFriend(LstQMRFriend& lst)
{
    for (LstQMRFriend::iterator iter = lst.begin(); iter != lst.end(); ++iter)
    {
        Decode_QMRemindFriendItem(*iter);
    }
}

void CQBToQMObjCoder::Encode_QMRemindFriendItem(QMRemindFriendItem& Item)
{
    Encode_Str(Item.sDisplayName);
}

void CQBToQMObjCoder::Decode_QMRemindFriendItem(QMRemindFriendItem& Item)
{
    Decode_Str(Item.sDisplayName);
}

void CQBToQMObjCoder::Encode_QMRemindConfig(QMRemindConfig& Item)
{
    Encode_LstQMRKey(Item.KeyConfig);
    Encode_LstQMRFriend(Item.FriendConfig);
}

void CQBToQMObjCoder::Decode_QMRemindConfig(QMRemindConfig& Item)
{
    Decode_LstQMRKey(Item.KeyConfig);
    Decode_LstQMRFriend(Item.FriendConfig);
}

void CQBToQMObjCoder::Encode_LstBondShare(LstBondShare& lst)
{
    for (LstBondShare::iterator iter = lst.begin(); iter != lst.end(); ++iter)
    {
        Encode_BondShareItem(*iter);
    }
}

void CQBToQMObjCoder::Decode_LstBondShare(LstBondShare& lst)
{
    for (LstBondShare::iterator iter = lst.begin(); iter != lst.end(); ++iter)
    {
        Decode_BondShareItem(*iter);
    }
}

void CQBToQMObjCoder::Encode_BondShareItem(BondShareItem& Item)
{
    Encode_Str(Item.sBondShortName);
    Encode_Str(Item.sMemo);
}

void CQBToQMObjCoder::Decode_BondShareItem(BondShareItem& Item)
{
    Decode_Str(Item.sBondShortName);
    Decode_Str(Item.sMemo);
}

void CQBToQMObjCoder::Encode_NewsInfo( NewsInfo &info )
{
	Encode_Str(info.sTitle);
	Encode_Str(info.sURL);
}

void CQBToQMObjCoder::Decode_NewsInfo( NewsInfo &info )
{
	Decode_Str(info.sTitle);
	Decode_Str(info.sURL);
}

void CQBToQMObjCoder::Encode_NewsInfoV2( NewsInfoV2 &info )
{
    Encode_Str(info.sTitle);
    Encode_Str(info.sURL);
    Encode_Str(info.sPicUUID);
}

void CQBToQMObjCoder::Decode_NewsInfoV2( NewsInfoV2 &info )
{
    Decode_Str(info.sTitle);
    Decode_Str(info.sURL);
    Decode_Str(info.sPicUUID);
}

void CQBToQMObjCoder::Encode_PicInfo( PicInfo &info )
{

}

void CQBToQMObjCoder::Decode_PiceInfo( PicInfo &info )
{

}

void CQBToQMObjCoder::Encode_NoInstituteContact( NoInstituteContact &item )
{
    Encode_Str(item.sInstituteName);
    Encode_Str(item.sName);
}

void CQBToQMObjCoder::Decode_NoInstituteContact( NoInstituteContact &item )
{
    Decode_Str(item.sInstituteName);
    Decode_Str(item.sName);
}
