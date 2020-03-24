//
//  CDoc.h
//
//  Created by Colin on 2020-03-05.
//  Copyright (c) 2020 Sumscope. All rights reserved.
//
#pragma once
#include "lava_base.h"
#include "CCalcEngine.h"
#include "cdh_client.h"
#include <string>
#include <memory>

using namespace ATL;

typedef struct acc
{
    CString   m_user;
    CString   m_password;

    acc() : m_user(_T("")),
            m_password(_T("")) {}
} accoutInfo;

class CDoc
{
public:
	CDoc();
	~CDoc();

	void Initialzie();
	inline void SetLoginResult(bool is_success)
	{
		m_is_login = is_success;
	}
	inline bool GetLoginResult()
	{
		return m_is_login;
	}

	ATL::CComVariant SingleCalc(char* func_name, CCalcEngine::CodeType code_type, long& param_count, ATL::CComVariant** params);

	inline void OnCalculate()
	{
		m_CalcEngine->OnCalculate();
	}
	inline void CDoc::OnAfterCalculate()
	{
		m_CalcEngine->OnAfterCalculate();
	}

	BOOL OpenWorkBook(BSTR file_name, _Workbook**);
	CComPtr<_Worksheet> AddNewSheet();
	CComPtr<_Worksheet> AddNewSheet(__lv_in CString name);

public:
    bool GetUserAccount(accoutInfo &acInfo);
    bool WriteUserAccount(const accoutInfo &acInfo);
    bool ClearUserAccount();
    bool LoginCDH(const accoutInfo &acInfo);
    CDHClient* GetCDHClient();

private:
	CString RegularSheetName(CString org_name);

private:
	bool            m_is_login;
    CDHClient*      m_cliProxy;
	std::shared_ptr<CCalcEngine>	m_CalcEngine;
};

