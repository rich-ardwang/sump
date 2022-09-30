
// QBQMTesterDlg.h : ͷ�ļ�
//

#pragma once
#include "IUIOperate.h"

class CTestFrame;

// CQBQMTesterDlg �Ի���
class CQBQMTesterDlg : public CDialog, 
                       public IUIOperate
{
// ����
public:
	CQBQMTesterDlg(CWnd* pParent = NULL);	// ��׼���캯��
    ~CQBQMTesterDlg();

// �Ի�������
	enum { IDD = IDD_QBQMTESTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
    CTestFrame* m_pTestFrame;
    std::string m_sUsr;
    std::string m_sPwd;
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnClose();
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedButton3();
    afx_msg void OnBnClickedButton4();
    afx_msg void OnBnClickedButton5();
};
