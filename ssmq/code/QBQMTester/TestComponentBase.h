#pragma once
#include "IQMToQB.h"

class IUIOperate;

using namespace QB_QM;

class CTestComponentBase
{
public:
    CTestComponentBase(void);
    virtual ~CTestComponentBase(void);

    void Init(IQMToQB* pQMToQB);

protected:
    virtual void InitCallback();

protected:
    IQMToQB* m_pIFQMToQB;
    IUIOperate* m_pUIOperate;
};
