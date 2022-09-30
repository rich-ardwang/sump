
// QBQMTesterDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CQBQMTesterDlg �Ի���




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


// CQBQMTesterDlg ��Ϣ�������

BOOL CQBQMTesterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

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
    

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CQBQMTesterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CQBQMTesterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CQBQMTesterDlg::OnBnClickedOk()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (m_pTestFrame)
    {
        m_pTestFrame->Release();
    }
    OnOK();
}

void CQBQMTesterDlg::OnBnClickedCancel()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (m_pTestFrame)
    {
        m_pTestFrame->Release();
    }
    OnCancel();
}

void CQBQMTesterDlg::OnClose()
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (m_pTestFrame)
    {
        m_pTestFrame->TestEditPurchase();
    }
}

void CQBQMTesterDlg::OnBnClickedButton2()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (m_pTestFrame)
    {
        m_pTestFrame->QueryPurchaseStatus();
    }
}

void CQBQMTesterDlg::OnBnClickedButton3()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (m_pTestFrame)
    {
        m_pTestFrame->ConfirmPurchase();
    }
}

void CQBQMTesterDlg::OnBnClickedButton4()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (m_pTestFrame)
    {
        m_pTestFrame->QueryWndHandle();
    }
}

void CQBQMTesterDlg::OnBnClickedButton5()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    sdbus::Message msg;
    msg.SetString(1, "PushMsgFromMockQM");
    CSSMQMsgSender::Post(sdbus::string("QBmsgQueue"), msg);
}
