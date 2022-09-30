#pragma once
#include "TestComponentBase.h"
#include "IQMToQB_Purchase.h"
#include "MyNotifier.h"

typedef std::map<Contact, bool> MapContactPerm;

class CTestComponentPurchase : public CTestComponentBase, 
    public QB_QM::IQMToQBCallback_Purchase
{
public:
    CTestComponentPurchase(void);
    ~CTestComponentPurchase(void);

    //Interface of QB_QM::IQMToQBCallback_Purchase
    bool Purchase(const Contact& ct, const PurchaseInfo& PInfo, const std::string& sWndID);
    void ContactIsDealerRet(const std::string& sReqID, bool bRet);
    void GetAllDealerRet(const std::string& sReqID, const SetUInt64& setID);
    void QueryPurchaseStatusRet(const std::string& sReqID, const MapPurchaseStatus& mapPStatus); 
    void ConfirmPurchaseRet(const std::string& sReqID, int nStatus);
    void IsEditPurchaseValidRet(const std::string& sReqID, bool bRet);
    void PurchaseStatusPush(const MapPurchaseStatus& mapPStatus);
    void ContactIsDealerPush(const Contact& ct, bool bRet);
    void IsEditPurchaseValidPush(const Contact& ct, bool bRet);

    //Test
    void TestEditPurchase();
    void QueryPurchaseStatus();
    void ConfirmPurchase();

public:
    void InitData();

protected:
    virtual void InitCallback();
   
private:
    IQMToQB_Purchase* m_pIFPurchase;
    SetUInt64 m_setDealers;

    MyNotifier m_Notify_IsEditPurchaseValidInfo;
    MapContactPerm m_cacheIsEditPurchaseValid;
    bool m_bTempIsEditPurchaseValid;

};
