#pragma once
#include "TestComponentBase.h"
#include "IQMToQB.h"

class CTestComponentCommon : public CTestComponentBase,
    public QB_QM::IQMToQBCallback
{
public:
    CTestComponentCommon(void);
    ~CTestComponentCommon(void);

    //Interface of QB_QM::IQMToQBCallback
    bool SelContacts(const std::string& sSelID, const std::string& sWndID, int nLimit, bool bShowGrp);
    bool EditContacts(const std::string& sEditID, const LstContact& Contacts, const std::string& sWndID, int nLimit, bool bShowGrp);
    bool QueryContactInfo(const LstContact& lstIn, LstPersonInfo& lstPerson, LstGroupInfo& lstGrp, LstContact& lstFailed);
    void QuitQM();
    bool OpenMainPanel(const std::string& sWndID);
    bool OpenDialogue(const Contact& ct, const std::string& sSendMsg, const std::string& sWndID, const bool bDialogue = true);
    unsigned long long GetCurUsrID();
    bool QueryCurUsrFriendIDs();
    bool QueryCurUsrGroupIDs();
    void QBStatus(int nStatus);
    bool SendMsg(const LstContact& Contacts, const std::string& sMsg, const std::string& sWndID);
    bool QueryQMWndPos(const std::string& sWndID, int& nTop, int& nBottom, int& nLeft, int& nRight);
    bool QueryQMWndHandle(const std::string& sWndID, unsigned long long& hWnd);
    bool QuerySharedGroup(Contact& ct);
    bool SearchQMContact(const ContactSearch& Input, LstSearchUserInfo& Result);
    void QueryQMIDByQBAccount(const std::string& sMsgID, const SetString& setQBAccount);
    bool OpenNewsBulletin(const std::string& sWndID);
    bool QueryCustomService(Contact& ct);
    void QBModifyPwd(const std::string& sNewPwd);
    void ReqJoinGroup(const std::string& sMsgID, const unsigned long long &llGroup);
    virtual void ReqOpenNews(const NewsInfo &info);
    virtual void SharePic(const PicInfo &info, const LstContact& Contacts);

    //�������Ÿ�ָ����ϵ��
    virtual void ShareNewsDetail(const NewsInfoV2 &info, const LstContact& Contacts, bool bDisablePDF = true);
    //����QB userID �򿪶Ի���
    virtual void OpenDialogueByUserID(const std::string &userID);
    //��ѯQM����״̬
    virtual void QueryQMStatus();
    //����QM�����̴��ڽӿ�
    virtual void ReqOpenSellsWnd(const std::string& sWndID);
    //��ѯQM�Ǻ���������ϵ�˽ӿ�
    virtual void ReqQueryNoInstituteContact(LstNoInstituteContact &contacts, int &status);
    //����QM�Ǻ���������ϵ�˽ӿ�
    virtual void ReqSetNoInstituteContact(const std::list<std::string> &QBAccounts);
    //����QM�����̽����еĵ������󵯳��Ĳ��Ҵ���
    virtual void ReqOpenSellsSearchWnd(const std::string& sWndID);
    //QB�û��������,֪ͨQM
    virtual void ReqOpenDialogueFromAd(const std::string &userID);

    //Test
    void QueryHWnd();

public:
    void InitData();

protected:
    virtual void InitCallback();
};
