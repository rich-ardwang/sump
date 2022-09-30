#pragma once
#include "afxwin.h"
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>

// CDlgLogin �Ի���

class CDlgLogin : public CDialog
{
	DECLARE_DYNAMIC(CDlgLogin)

public:
	CDlgLogin(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgLogin();

// �Ի�������
	enum { IDD = IDD_DIALOG_Login };

    void GetLoginInfo(std::string& sUsr, std::string& sPwd);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();

private:
    CEdit m_edit_usr;
    CEdit m_edit_pwd;
    CString m_strUsr;
    CString m_strPwd;

    boost::mutex m_mutex;
    boost::condition m_conditionRecv;

public:
    virtual BOOL OnInitDialog();

protected:
    void ExTest();
    void TestNotify();
};
