//
//  CDoc.cpp
//
//  Created by Colin on 2020-03-05.
//  Copyright (c) 2020 Sumscope. All rights reserved.
//
#include "stdafx.h"
#include <fstream>
#include "CDoc.h"
#include "SumsAddin.h"
#include "lava_crt.h"
#include "lava_utils_api.h"

using namespace ATL;

CDoc::CDoc() : m_is_login(false)
{
	m_CalcEngine = std::make_shared<CCalcEngine>();
}

CDoc::~CDoc()
{
	m_CalcEngine->OnStopCalculate();
	m_CalcEngine = nullptr;
}

void CDoc::Initialzie()
{
	std::wstring xla_file = CSumsAddin::GetAddin()->GetModulePath();
	xla_file += L"sumscope.xla";

	CComPtr<_Workbook> spWorkbook;
	BOOL ret = OpenWorkBook(CComBSTR(xla_file.c_str()), &spWorkbook);
	if (ret == FALSE)
		log_error(helios_module, "open sumscope.xla file failed.");

    //init cdh client proxy
    m_cliProxy = CDHClient::getInstance();
    if (nullptr == m_cliProxy)
        log_error(helios_module, "CDH client init failed!");

    //auto login CDH
    if (!m_is_login)
    {
        accoutInfo ac;
        bool ret = GetUserAccount(ac);
        if (ret)
        {
            ret = LoginCDH(ac);
            if (ret)
                m_is_login = true;
            else
                log_error(helios_module, "login CDH failed!");
        }
    }
}

CComVariant CDoc::SingleCalc(char* func_name, CCalcEngine::CodeType code_type, long& param_count, CComVariant** params)
{
	return m_CalcEngine->SingleCalc(func_name, code_type, param_count, params);
}

BOOL CDoc::OpenWorkBook(BSTR file_name, _Workbook** workbook)
{
	CComPtr<Workbooks> spWorkbooks;
	CSumsAddin::GetAddin()->GetApp()->get_Application()->get_Workbooks(&spWorkbooks);
	HRESULT hr = spWorkbooks->raw_Open(file_name, CComVariant(0), CComVariant(VARIANT_FALSE), CComVariant(1),
		CComVariant(""), CComVariant(""), CComVariant(VARIANT_TRUE), CComVariant(MSExcel::xlWindows), CComVariant(6),
		CComVariant(VARIANT_FALSE), CComVariant(VARIANT_FALSE), CComVariant(0), CComVariant(VARIANT_FALSE), CComVariant(VARIANT_FALSE),
		CComVariant(MSExcel::xlNormalLoad), 0, workbook);

	return SUCCEEDED(hr);
}

CComPtr<_Worksheet> CDoc::AddNewSheet()
{
	CComPtr<_Workbook> spWorkbook;
	CComPtr<_Application> spApp = CSumsAddin::GetAddin()->GetApp()->get_Application();
	spApp->get_ActiveWorkbook(&spWorkbook);
	if ( nullptr == spWorkbook )
	{
		CComPtr<Workbooks> spWorkbooks;
		spApp->get_Workbooks(&spWorkbooks);
		spWorkbooks->raw_Add(vtMissing, 0, &spWorkbook);
	}

	if ( spWorkbook )
	{
		CComPtr<Sheets> spSheets;
		if ( SUCCEEDED(spWorkbook->get_Sheets(&spSheets)) )
		{
			CComPtr<IDispatch> spAfter = spSheets->GetItem(CComVariant(spSheets->GetCount()));
			CComPtr<IDispatch> spNew = spSheets->Add(vtMissing, CComVariant(spAfter), CComVariant(1), CComVariant(xlWorksheet));
			if ( spNew )
			{
				CComQIPtr<_Worksheet> spWorksheet( spNew );
				return spWorksheet;
			}
			log_error(helios_module, "Add new sheet failed.");
		}
		else
			log_error(helios_module, "Get sheets failed.");
	}
	return nullptr;
}

CComPtr<_Worksheet> CDoc::AddNewSheet(__lv_in CString name)
{
	name = RegularSheetName(name);
	CComPtr<_Worksheet> spWorksheet = AddNewSheet();
	if ( spWorksheet )
	{
		CComPtr<IDispatch> spDisp;
		spWorksheet->get_Parent(&spDisp);
		CComQIPtr<_Workbook> spWorkbook(spDisp);

		CComPtr<Sheets> spSheets;
		spWorkbook->get_Sheets(&spSheets);

		for ( int i = 0; ;i++ )
		{
			CString sheet_name;
			if (0 == i)
				sheet_name = name;
			else
				sheet_name.Format(_T("%s%d"), name.Left(30), i); // name too length lead to crash.

			try
			{
				spDisp = nullptr;
				spSheets->get_Item(CComVariant(sheet_name), &spDisp);
				if (spDisp)
					continue;
			}
			catch (...)
			{
				log_error(helios_module, "exception occurs when add named sheet.");
			}
			spWorksheet->put_Name(CComBSTR(sheet_name));
			break;
		}
		return spWorksheet;
	}
	return nullptr;
}

CString CDoc::RegularSheetName(CString org_name)
{
	CString name;
	org_name.Replace(_T("*"), _T("_"));

	CString token(_T(":\\/?[]"));
	int cur_pos = 0;
	CString res = org_name.Tokenize(token, cur_pos);
	while (res != _T(""))
	{
		name += res;
		res = org_name.Tokenize(token, cur_pos);
	}
	return (name.GetLength() > 30) ? name.Left(30) : name;
}

bool CDoc::GetUserAccount(accoutInfo &acInfo)
{
    bool ret = false;
    std::wstring filePath = CSumsAddin::GetAddin()->GetModulePath() + L"DT";
    std::ifstream in;
    in.open(filePath);
    if (!in.is_open())
        return ret;

    //read from file
    std::string b64User;
    std::string b64Pwd;
    in >> b64User;
    in >> b64Pwd;
    in.close();

    //parse the info
    size_t lenUser = 0;
    size_t lenPwd = 0;
    char *cUser = nullptr;
    char *cPwd = nullptr;
    cUser = (char *)base64_decode((uint8_t *)b64User.c_str(), lenUser);
    cPwd = (char *)base64_decode((uint8_t *)b64Pwd.c_str(), lenPwd);
    if ((lenUser > 0) && (lenPwd > 0))
    {
        std::string user(cUser, lenUser);
        std::string pwdMd5(cPwd, lenPwd);
        acInfo.m_user = CA2T(user.c_str());
        acInfo.m_password = CA2T(pwdMd5.c_str());
        ret = true;
    }

    //release buf
    if (cUser)
    {
        delete[] cUser;
        cUser = nullptr;
    }
    if (cPwd)
    {
        delete[] cPwd;
        cPwd = nullptr;
    }

    return ret;
}

bool CDoc::WriteUserAccount(const accoutInfo &acInfo)
{
    std::string u = CT2A(acInfo.m_user.GetString());
    std::string p = CT2A(acInfo.m_password.GetString());
    std::string usr = lava::utils::trim(u);
    std::string pd = lava::utils::trim(p);

    //base64 encryption
    uint8_t *cypUser = base64_encode((uint8_t *)usr.c_str(), usr.length());
    uint8_t *cypPwd = base64_encode((uint8_t *)pd.c_str(), pd.length());

    std::wstring filePath = CSumsAddin::GetAddin()->GetModulePath() + L"DT";
    std::ofstream out(filePath);
    if (!out)
        return false;
    out << cypUser << " " << cypPwd << std::endl;
    out.close();
    return true;
}

bool CDoc::ClearUserAccount()
{
    std::wstring filePath = CSumsAddin::GetAddin()->GetModulePath() + L"DT";
    std::ofstream out(filePath);
    if (!out)
        return false;
    out << "" << std::endl;
    out.close();
    return true;
}

bool CDoc::LoginCDH(const accoutInfo &acInfo)
{
    bool ret = false;
    if (nullptr == m_cliProxy)
        return ret;

    std::string ip = "172.16.74.144";
    int port = 20480;
    ret = m_cliProxy->Connect(ip, port);
    if (!ret)
        return ret;

    std::string usr = CT2A(acInfo.m_user.GetString());
    std::string pwd = CT2A(acInfo.m_password.GetString());
    ret = m_cliProxy->Login(usr, pwd);
    if (!ret)
        return ret;

    m_cliProxy->getAuth();
    return ret;
}

CDHClient* CDoc::GetCDHClient()
{
    return m_cliProxy;
}
