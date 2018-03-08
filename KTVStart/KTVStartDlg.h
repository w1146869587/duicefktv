
// KTVStartDlg.h : 头文件
//

#pragma once

#include "simple_handler.h"
// CKTVStartDlg 对话框
class CKTVStartDlg : public CDialogEx
{
// 构造
public:
	CKTVStartDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KTVSTART_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	CefRefPtr<SimpleHandler>	g_handler;
// 实现
protected:
	HICON m_hIcon;
	bool	m_bClose = false;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void CreateBrowser();


	BOOL PreTranslateMessage(MSG* pMsg);
	int TestHostUrl();


};
