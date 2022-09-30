#pragma once

namespace QB_QM
{
    class IQMToQB;
}

class CTestComponentCommon;
class CTestComponentPurchase;
class CMsgHandler;
class CTestFrame
{
public:
    CTestFrame(void);
    ~CTestFrame(void);

public:
    bool Init(const std::string& sUsr, const std::string& sPwd);
    void Release();

    //Test
    void TestEditPurchase();
    void QueryPurchaseStatus();
    void ConfirmPurchase();
    void QueryWndHandle();

protected:
    void InitTestComponent();
    void ClearTestComponnet();
    void InitData();

private:
    QB_QM::IQMToQB* m_pQMToQB;
    CTestComponentCommon* m_pTestCommon;
    CTestComponentPurchase* m_pTestPurchase;
    CMsgHandler *m_pMsgHandler;
};
