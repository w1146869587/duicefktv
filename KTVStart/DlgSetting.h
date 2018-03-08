#pragma once


// DlgSetting 对话框

class DlgSetting : public CDialogEx
{
	DECLARE_DYNAMIC(DlgSetting)

public:
	DlgSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgSetting();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRestart();
  
};
