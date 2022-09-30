#include "StdAfx.h"
#include "TestComponentCommon.h"

CTestComponentCommon::CTestComponentCommon(void)
{
}

CTestComponentCommon::~CTestComponentCommon(void)
{
}

bool CTestComponentCommon::SelContacts(const std::string& sSelID, const std::string& sWndID, int nLimit, bool bShowGrp)
{
    return false;
}

bool CTestComponentCommon::EditContacts(const std::string& sEditID, const LstContact& Contacts, const std::string& sWndID, int nLimit, bool bShowGrp)
{
    return false;
}

bool CTestComponentCommon::QueryContactInfo(const LstContact& lstIn, LstPersonInfo& lstPerson, LstGroupInfo& lstGrp, LstContact& lstFailed)
{
    return false;
}

void CTestComponentCommon::QuitQM()
{

}

bool CTestComponentCommon::OpenMainPanel(const std::string& sWndID)
{
    return false;
}

bool CTestComponentCommon::OpenDialogue(const Contact& ct, const std::string& sSendMsg, const std::string& sWndID, const bool bDialogue/* = true*/)
{
    return false;
}

unsigned long long CTestComponentCommon::GetCurUsrID()
{
    return false;
}

bool CTestComponentCommon::QueryCurUsrFriendIDs()
{
    return false;
}

bool CTestComponentCommon::QueryCurUsrGroupIDs()
{
    return false;
}

void CTestComponentCommon::QBStatus(int nStatus)
{

}

bool CTestComponentCommon::SendMsg(const LstContact& Contacts, const std::string& sMsg, const std::string& sWndID)
{
    return false;
}

bool CTestComponentCommon::QueryQMWndPos(const std::string& sWndID, int& nTop, int& nBottom, int& nLeft, int& nRight)
{
    return false;
}

bool CTestComponentCommon::QueryQMWndHandle(const std::string& sWndID, unsigned long long& hWnd)
{
    hWnd = 123445;
    return false;
}

bool CTestComponentCommon::QuerySharedGroup(Contact& ct)
{
    return false;
}

bool CTestComponentCommon::SearchQMContact(const ContactSearch& Input, LstSearchUserInfo& Result)
{
    return false;
}

void CTestComponentCommon::QueryQMIDByQBAccount(const std::string& sMsgID, const SetString& setQBAccount)
{

}

bool CTestComponentCommon::OpenNewsBulletin(const std::string& sWndID)
{
    return false;
}

bool CTestComponentCommon::QueryCustomService(Contact& ct)
{
    return false;
}

void CTestComponentCommon::QBModifyPwd(const std::string& sNewPwd)
{

}

void CTestComponentCommon::QueryHWnd()
{
    unsigned long long hRet;
    m_pIFQMToQB->QueryQBWndHandle("", hRet);
    HWND hWnd = (HWND)hRet;
    
    WINDOWPLACEMENT winplacement;
    memset(&winplacement, 0, sizeof(winplacement));
    winplacement.length = sizeof(WINDOWPLACEMENT);
    ::GetWindowPlacement(hWnd, &winplacement);
    winplacement.showCmd = SW_SHOWMINIMIZED;
    ::SetWindowPlacement(hWnd, &winplacement);
}

void CTestComponentCommon::InitData()
{

}

void CTestComponentCommon::InitCallback()
{
    if (m_pIFQMToQB)
    {
        m_pIFQMToQB->RegCallback(this);
    }
}

void CTestComponentCommon::ReqJoinGroup( const std::string& sMsgID, const unsigned long long &llGroup )
{
    if (m_pIFQMToQB)
    {
        m_pIFQMToQB->ReqJoinGroupRet(sMsgID, true);
    }
}

void CTestComponentCommon::ReqOpenNews( const NewsInfo &info )
{

}

void CTestComponentCommon::SharePic( const PicInfo &info, const LstContact& Contacts )
{
    if (m_pIFQMToQB)
    {
        NewsInfoV2 news;
        m_pIFQMToQB->ReqOpenNewsDetail(news, false);
    }
}

void CTestComponentCommon::ShareNewsDetail( const NewsInfoV2 &info, const LstContact& Contacts, bool bDisablePDF /*= true*/ )
{
    if (m_pIFQMToQB)
    {
        int i = 0;
        i--;
    }
}

void CTestComponentCommon::OpenDialogueByUserID( const std::string &userID )
{
    if (m_pIFQMToQB)
    {
        NewsInfoV2 info;
        info.sTitle = "baidu";
        info.sURL = "www.baidu.com";
        info.sPicUUID = "wwwwwwww";
        m_pIFQMToQB->ReqOpenNewsDetail(info, false);
    }
}

void CTestComponentCommon::QueryQMStatus()
{
    if (m_pIFQMToQB)
    {
        m_pIFQMToQB->QueryQMStatusRet(0);
    }
}

void CTestComponentCommon::ReqOpenSellsWnd( const std::string& sWndID )
{
    int i = 0;
    i--;
    i++;
}

void CTestComponentCommon::ReqQueryNoInstituteContact( LstNoInstituteContact &contacts, int &status )
{
    NoInstituteContact item;
    item.sQBAccount = "qwertyuiop";
    item.sInstituteName = "超级机构";
    item.sName = "名字";
    item.ullQMId = 1234567890;
    contacts.push_back(item);

    status = 1;
}

void CTestComponentCommon::ReqSetNoInstituteContact( const std::list<std::string> &QBAccounts )
{
    LstNoInstituteContact contacts;
    NoInstituteContact item;
    item.sQBAccount = "qwertyuiop";
    item.sInstituteName = "超级机构";
    item.sName = "名字";
    item.ullQMId = 1234567890;
    contacts.push_back(item);

    if (m_pIFQMToQB)
    {
        m_pIFQMToQB->ModifyNoInstituteContactRet(contacts);
    }
}

void CTestComponentCommon::ReqOpenSellsSearchWnd( const std::string& sWndID )
{
    int i = 0;
    i--;
    i++;
}

void CTestComponentCommon::ReqOpenDialogueFromAd( const std::string &userID )
{
    int i = 0;
    i--;
    i++;
}
