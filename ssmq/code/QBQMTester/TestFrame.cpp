#include "StdAfx.h"
#include "TestFrame.h"
#include "IQMToQB.h"
#include "TestComponentPurchase.h"
#include "TestComponentCommon.h"
#include "SdbusMsgHandler.h"
#include "ISSMQMng.h"

CTestFrame::CTestFrame(void)
:m_pQMToQB(NULL)
,m_pTestCommon(NULL)
,m_pTestPurchase(NULL)
,m_pMsgHandler(NULL)
{
}

CTestFrame::~CTestFrame(void)
{
    ClearTestComponnet();
}

bool CTestFrame::Init(const std::string& sUsr, const std::string& sPwd)
{
    m_pQMToQB = GetQMToQB();
    if (!m_pQMToQB)
    {
        return false;
    }

    m_pQMToQB->Init(sUsr, sPwd);


    InitTestComponent();
    InitData();
    GetSSMQMng()->RegObs(m_pMsgHandler);

    return true;
}

void CTestFrame::Release()
{
    if (!m_pQMToQB)
    {
        return;
    }
    m_pQMToQB->Release();
    m_pQMToQB = NULL;
}


void CTestFrame::TestEditPurchase()
{
    m_pTestPurchase->TestEditPurchase();
}

void CTestFrame::QueryPurchaseStatus()
{
    m_pTestPurchase->QueryPurchaseStatus();
}

void CTestFrame::ConfirmPurchase()
{
    m_pTestPurchase->ConfirmPurchase();
}

void CTestFrame::QueryWndHandle()
{
    m_pTestCommon->QueryHWnd();
}

void CTestFrame::InitTestComponent()
{
    m_pTestCommon = new CTestComponentCommon();
    m_pTestCommon->Init(m_pQMToQB);

    m_pTestPurchase = new CTestComponentPurchase();
    m_pTestPurchase->Init(m_pQMToQB);

    m_pMsgHandler = new CMsgHandler();


    m_pQMToQB->SetStatus(1);
}

void CTestFrame::ClearTestComponnet()
{
    if (m_pTestCommon)
    {
        delete m_pTestCommon;
        m_pTestCommon = NULL;
    }

    if (m_pTestPurchase)
    {
        delete m_pTestPurchase;
        m_pTestPurchase = NULL;
    }
    if (m_pMsgHandler)
    {
        delete m_pMsgHandler;
        m_pMsgHandler = NULL;
    }
}

void CTestFrame::InitData()
{
    if (m_pTestCommon)
    {
        m_pTestCommon->InitData();
    }
    if (m_pTestPurchase)
    {
        m_pTestPurchase->InitData();
    }
}