#include "stdafx.h"
#include <iomanip>
#include "CDoc.h"
#include "lava_crt.h"
#include "LoginDlg.h"

LRESULT CLoginDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    // center the dialog on the screen
    CenterWindow();

    // set icons
    HICON hIcon = AtlLoadIconImage(IDI_ICON_MAIN, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
    SetIcon(hIcon, TRUE);
    HICON hIconSmall = AtlLoadIconImage(IDI_ICON_MAIN, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
    SetIcon(hIconSmall, FALSE);

    // start GDI plus
    Gdiplus::GdiplusStartupInput gdiInput = { 0 };
    GdiplusStartup(&m_GdiToken, &gdiInput, NULL);

    m_ModulePath = CSumsAddin::GetAddin()->GetModulePath();
    std::wstring imagePath = m_ModulePath + L"sumscope.jpg";
    m_pImage = new Gdiplus::Image(imagePath.c_str());

    // get user account info
    accoutInfo ac;
    bool ret = CSumsAddin::GetAddin()->GetDoc()->GetUserAccount(ac);
    if (ret)
    {
        SetDlgItemText(IDC_EDIT_USER, ac.m_user);
        SetDlgItemText(IDC_EDIT_PSWD, ac.m_password);
        ((CButton)GetDlgItem(IDC_CHECK_ACCOUNT_STORE)).SetCheck(TRUE);
    }
    else
        ((CButton)GetDlgItem(IDC_CHECK_ACCOUNT_STORE)).SetCheck(FALSE);

    return TRUE;
}

LRESULT CLoginDlg::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    CPaintDC dc(m_hWnd);
    RECT rect;
    GetClientRect(&rect);
    HDC hDC = dc.m_hDC;
    Gdiplus::Graphics gph(hDC);
    gph.DrawImage(m_pImage, 0, 0, rect.right - rect.left, rect.bottom - rect.top);
    return TRUE;
}

LRESULT CLoginDlg::OnCtlColorStatic(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    HDC hDC = (HDC)wParam;
    SetBkMode(hDC, TRANSPARENT);
    SetTextColor(hDC, RGB(255, 255, 255));
    return (LRESULT)GetStockObject(NULL_BRUSH);
}

LRESULT CLoginDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    // get the account info
    CString user = _T("");
    CString pwd = _T("");
    GetDlgItemText(IDC_EDIT_USER, user);
    GetDlgItemText(IDC_EDIT_PSWD, pwd);

    // check input
    if (user.IsEmpty())
    {
        ::MessageBox(this->m_hWnd, L"User name is empty!", L"Input Error", MB_ICONEXCLAMATION);
        return 0;
    }
    if (pwd.IsEmpty())
    {
        ::MessageBox(this->m_hWnd, L"Password is empty!", L"Input Error", MB_ICONEXCLAMATION);
        return 0;
    }
    if (user.GetLength() > HELIOS_MAX_UNAME_LEN)
    {
        ::MessageBox(this->m_hWnd, L"User name too long!", L"Input Error", MB_ICONEXCLAMATION);
        return 0;
    }
    if (pwd.GetLength() >= HELIOS_MAX_PASSWORD_LEN)
    {
        ::MessageBox(this->m_hWnd, L"Password too long!", L"Input Error", MB_ICONEXCLAMATION);
        return 0;
    }

    // password convert to md5
    std::string pwdMd5 = Conver2Md5(pwd);
    if (pwdMd5.empty())
    {
        ::MessageBox(this->m_hWnd, L"md5 encode for password failed!", L"Input Error", MB_ICONEXCLAMATION);
        return 0;
    }

    // get user account info
    accoutInfo acInfo;
    acInfo.m_user = user;
    acInfo.m_password = CA2T(pwdMd5.c_str());

    // get check box
    int chkState = ((CButton)GetDlgItem(IDC_CHECK_ACCOUNT_STORE)).GetCheck();
    if (TRUE == chkState)
    {
        // need store the user's account
        // store the user name and the md5 value of the password
        bool ret = CSumsAddin::GetAddin()->GetDoc()->WriteUserAccount(acInfo);
        if (!ret)
        {
            ::MessageBox(this->m_hWnd, L"Store account failed!", L"Login Error", MB_ICONEXCLAMATION);
            return 0;
        }
    }
    else
    {
        //delete user info
        bool ret = CSumsAddin::GetAddin()->GetDoc()->ClearUserAccount();
        if (!ret)
        {
            ::MessageBox(this->m_hWnd, L"Clear account failed!", L"Login Error", MB_ICONEXCLAMATION);
            return 0;
        }
    }

    // login to CDH
    if (!CSumsAddin::GetAddin()->GetDoc()->GetLoginResult())
    {
        bool ret = CSumsAddin::GetAddin()->GetDoc()->LoginCDH(acInfo);
        if (ret)
        {
            ::MessageBox(this->m_hWnd, L"Login success.", L"MessageBox", MB_OK);
            m_login_result = true;
            CloseDialog(wID);
        }
        else
        {
            ::MessageBox(this->m_hWnd, L"Login failed!", L"Login Error", MB_ICONEXCLAMATION);
            m_login_result = false;
        }
    }

    return 0;
}

LRESULT CLoginDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CloseDialog(wID);
    return 0;
}

void CLoginDlg::CloseDialog(int nVal)
{
    Gdiplus::GdiplusShutdown(m_GdiToken);
    EndDialog(nVal);
}

std::string CLoginDlg::Conver2Md5(const CString &in)
{
    std::string stdIn = CT2A(in.GetString());
    std::string trimIn = lava::utils::trim(stdIn);
    char md5Value[33];
    size_t len = 33;
    int ret = md5_encode(trimIn.c_str(), trimIn.length(), md5Value, len);
    if (0 == ret)
    {
        std::string retValue(md5Value);
        return retValue;
    }
    else
        return "";
}
