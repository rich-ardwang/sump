
// QBQMTesterDlg.h : 头文件
//

#pragma once
#include "IUIOperate.h"

class CTestFrame;

// CQBQMTesterDlg 对话框
class CQBQMTesterDlg : public CDialog, 
                       public IUIOperate
{
// 构造
public:
	CQBQMTesterDlg(CWnd* pParent = NULL);	// 标准构造函数
    ~CQBQMTesterDlg();

// 对话框数据
	enum { IDD = IDD_QBQMTESTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
