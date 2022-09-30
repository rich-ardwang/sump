
// QBQMTesterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QBQMTester.h"
#include "QBQMTesterDlg.h"
#include "TestFrame.h"
#include "DlgLogin.h"
#include "QBToQM\SSMQMsgSender.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CQBQMTesterDlg 对话框




CQBQMTesterDlg::CQBQMTesterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQBQMTesterDlg::IDD, pParent)
    , m_pTestFrame(NULL)
    , m_sUsr("")
    , m_sPwd("")
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_pTestFrame = new CTestFrame();
}

CQBQMTesterDlg::~CQBQMTesterDlg()
{
    if (m_pTestFrame)
    {
        delete m_pTestFrame;
    }
}

void CQBQMTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CQBQMTesterDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDOK, &CQBQMTesterDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CQBQMTesterDlg::OnBnClickedCancel)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_BUTTON1, &CQBQMTesterDlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CQBQMTesterDlg::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON3, &CQBQMTesterDlg::OnBnClickedButton3)
    ON_BN_CLICKED(IDC_BUTTON4, &CQBQMTesterDlg::OnBnClickedButton4)
    ON_BN_CLICKED(IDC_BUTTON5, &CQBQMTesterDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CQBQMTesterDlg 消息处理程序

BOOL CQBQMTesterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

    CDlgLogin MyDlgLogin;
    if (IDOK == MyDlgLogin.DoModal())
    {
        MyDlgLogin.GetLoginInfo(m_sUsr, m_sPwd);
        if (m_pTestFrame->Init(m_sUsr, m_sPwd))
        {
            return TRUE;
        }
    }
    else
    {
        OnCancel();
    }
    

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CQBQMTesterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CQBQMTesterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CQBQMTesterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CQBQMTesterDlg::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码
    if (m_pTestFrame)
    {
        m_pTestFrame->Release();
    }
    OnOK();
}

void CQBQMTesterDlg::OnBnClickedCancel()
{
    // TODO: 在此添加控件通知处理程序代码
    if (m_pTestFrame)
    {
        m_pTestFrame->Release();
    }
    OnCancel();
}

void CQBQMTesterDlg::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if (m_pTestFrame)
    {
        m_pTestFrame->Release();
    }
    __super::OnClose();
}

void CQBQMTesterDlg::OnBnClickedButton1()
{
    //HWND h = GetSafeHwnd();
    //unsigned long long u64 = (unsigned long long)h;
    //HWND i = (HWND)u64;
    // TODO: 在此添加控件通知处理程序代码
    if (m_pTestFrame)
    {
        m_pTestFrame->TestEditPurchase();
    }
}

void CQBQMTesterDlg::OnBnClickedButton2()
{
    // TODO: 在此添加控件通知处理程序代码
    if (m_pTestFrame)
    {
        m_pTestFrame->QueryPurchaseStatus();
    }
}

void CQBQMTesterDlg::OnBnClickedButton3()
{
    // TODO: 在此添加控件通知处理程序代码
    if (m_pTestFrame)
    {
        m_pTestFrame->ConfirmPurchase();
    }
}

void CQBQMTesterDlg::OnBnClickedButton4()
{
    // TODO: 在此添加控件通知处理程序代码
    if (m_pTestFrame)
    {
        m_pTestFrame->QueryWndHandle();
    }
}

void CQBQMTesterDlg::OnBnClickedButton5()
{
    // TODO: 在此添加控件通知处理程序代码

    sdbus::Message msg;
    msg.SetString(1, "PushMsgFromMockQM");
    CSSMQMsgSender::Post(sdbus::string("QBmsgQueue"), msg);
}
