// LoginDlg.h : interface of the CLoginDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <GdiPlus.h>
#include <atlimage.h>
#include "SumsAddin.h"

#pragma comment(lib, "GdiPlus.lib")

#define HELIOS_MAX_UNAME_LEN            32
#define HELIOS_MAX_PASSWORD_LEN         33

class CLoginDlg : public ATL::CDialogImpl<CLoginDlg>
{
public:
    CLoginDlg() : m_pImage(nullptr), m_login_result(false){}
    ~CLoginDlg()
    {
        if (m_pImage)
        {
            delete m_pImage;
            m_pImage = nullptr;
        }
    }

    enum { IDD = IDD_DLG_LOGIN };

    BEGIN_MSG_MAP(CLoginDlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_PAINT, OnPaint)
        MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
        COMMAND_ID_HANDLER(IDOK, OnOK)
        COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
    END_MSG_MAP()

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnCtlColorStatic(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    void CloseDialog(int nVal);

    inline bool GetLoginResult()
    {
        return m_login_result;
    }

private:
    std::string Conver2Md5(const CString &in);

private:
    Gdiplus::Image*     m_pImage;
    ULONG_PTR           m_GdiToken;
    std::wstring        m_ModulePath;
    bool                m_login_result;
};
