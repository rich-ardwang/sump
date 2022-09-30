#include "StdAfx.h"
#include "TestComponentBase.h"

CTestComponentBase::CTestComponentBase(void)
:m_pIFQMToQB(NULL)
,m_pUIOperate(NULL)
{
}

CTestComponentBase::~CTestComponentBase(void)
{
}

void CTestComponentBase::Init(IQMToQB* pQMToQB)
{
    m_pIFQMToQB = pQMToQB;
    InitCallback();
}

void CTestComponentBase::InitCallback()
{

}