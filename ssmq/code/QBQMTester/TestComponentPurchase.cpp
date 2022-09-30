#include "StdAfx.h"
#include "TestComponentPurchase.h"

CTestComponentPurchase::CTestComponentPurchase(void)
:m_pIFPurchase(NULL)
,m_bTempIsEditPurchaseValid(false)
{
}

CTestComponentPurchase::~CTestComponentPurchase(void)
{
}

bool CTestComponentPurchase::Purchase(const Contact& ct, const PurchaseInfo& PInfo, const std::string& sWndID)
{
    return false;
}

void CTestComponentPurchase::ContactIsDealerRet(const std::string& sReqID, bool bRet)
{

}

void CTestComponentPurchase::GetAllDealerRet(const std::string& sReqID, const SetUInt64& setID)
{
    m_setDealers = setID;
}

void CTestComponentPurchase::QueryPurchaseStatusRet(const std::string& sReqID, const MapPurchaseStatus& mapPStatus)
{
    for (MapPurchaseStatus::const_iterator iter = mapPStatus.begin(); iter != mapPStatus.end(); ++iter)
    {
        PurchaseUkey uk = iter->first;
        int nStatus = iter->second;
    }
}

void CTestComponentPurchase::ConfirmPurchaseRet(const std::string& sReqID, int nStatus)
{

}

void CTestComponentPurchase::IsEditPurchaseValidRet(const std::string& sReqID, bool bRet)
{
    m_bTempIsEditPurchaseValid = bRet;
    m_Notify_IsEditPurchaseValidInfo.Notify();
}

void CTestComponentPurchase::PurchaseStatusPush(const MapPurchaseStatus& mapPStatus)
{

}

void CTestComponentPurchase::ContactIsDealerPush(const Contact& ct, bool bRet)
{

}

void CTestComponentPurchase::IsEditPurchaseValidPush(const Contact& ct, bool bRet)
{

}

void CTestComponentPurchase::TestEditPurchase()
{
    if (!m_pIFPurchase)
    {
        return;
    }

    Contact ct;
    ct.nContactID = 10022091;
    ct.nContactType = 0;
     m_pIFPurchase->EditPurchase(ct);

    //402880f034219aed0134219e10b00727

    /*
    for (SetUInt64::iterator iter = m_setDealers.begin(); iter != m_setDealers.end(); ++iter)
    {
        Contact ct;
        ct.nContactID = *iter;
        ct.nContactType = 0;
        m_pIFPurchase->IsEditPurchaseValid(ct);
        if (m_Notify_IsEditPurchaseValidInfo.Wait(-1))
        {
            m_cacheIsEditPurchaseValid[ct] = m_bTempIsEditPurchaseValid;
            m_bTempIsEditPurchaseValid = false;
        }
    }

    for (MapContactPerm::iterator iter = m_cacheIsEditPurchaseValid.begin(); iter != m_cacheIsEditPurchaseValid.end(); ++iter)
    {
        if (iter->second)
        {
            m_pIFPurchase->EditPurchase(iter->first);
            return;
        }
    }
    */
}

void CTestComponentPurchase::QueryPurchaseStatus()
{
    if (!m_pIFPurchase)
    {
        return;
    }

    LstPurchaseUkey lstQuery;
    PurchaseUkey uk("1122", time_t(0));
    lstQuery.push_back(uk);

    m_pIFPurchase->QueryPurchaseStatus(lstQuery);
}

void CTestComponentPurchase::ConfirmPurchase()
{
    if (!m_pIFPurchase)
    {
        return;
    }

    m_pIFPurchase->ConfirmPurchase("1122", time_t(0));
}

void CTestComponentPurchase::InitData()
{
    if (m_pIFQMToQB)
    {
        m_pIFPurchase = m_pIFQMToQB->GetSubInterface_Purchase();
    }

    if (!m_pIFPurchase)
    {
        return;
    }

    std::string sReqID = m_pIFPurchase->GetAllDealer();
}

void CTestComponentPurchase::InitCallback()
{
    if (m_pIFQMToQB)
    {
        m_pIFQMToQB->GetSubInterface_Purchase()->RegCallback(this);
    }
}