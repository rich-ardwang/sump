// DlgLogin.cpp : 实现文件
//

#include "stdafx.h"
#include "QBQMTester.h"
#include "DlgLogin.h"


// CDlgLogin 对话框

IMPLEMENT_DYNAMIC(CDlgLogin, CDialog)

CDlgLogin::CDlgLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLogin::IDD, pParent)
    , m_strUsr(_T(""))
    , m_strPwd(_T(""))
{

}

CDlgLogin::~CDlgLogin()
{
}

void CDlgLogin::GetLoginInfo(std::string& sUsr, std::string& sPwd)
{
    sUsr = SS_BASE::CString2string(m_strUsr);
    sPwd = SS_BASE::CString2string(m_strPwd);
}

void CDlgLogin::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT1, m_edit_usr);
    DDX_Control(pDX, IDC_EDIT2, m_edit_pwd);
}


BEGIN_MESSAGE_MAP(CDlgLogin, CDialog)
    ON_BN_CLICKED(IDOK, &CDlgLogin::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CDlgLogin::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgLogin 消息处理程序

void CDlgLogin::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码

    m_edit_usr.GetWindowText(m_strUsr);
    m_edit_pwd.GetWindowText(m_strPwd);
    OnOK();
}

void CDlgLogin::OnBnClickedCancel()
{
    // TODO: 在此添加控件通知处理程序代码
    OnCancel();
}

BOOL CDlgLogin::OnInitDialog()
{
    CDialog::OnInitDialog();

    //ExTest();

    // TODO:  在此添加额外的初始化
    m_edit_usr.SetWindowText(_T("hytest02"));
    m_edit_pwd.SetWindowText(_T("123456"));

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CDlgLogin::ExTest()
{
    boost::mutex::scoped_lock lock(m_mutex);
    
    boost::thread* pT = new boost::thread(boost::bind(&CDlgLogin::TestNotify, this));
    Sleep(5000);
    m_conditionRecv.wait(m_mutex);
    Sleep(1000);
}

void CDlgLogin::TestNotify()
{
    Sleep(2000);
    m_conditionRecv.notify_one();
}