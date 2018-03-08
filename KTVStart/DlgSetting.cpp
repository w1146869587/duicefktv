// DlgSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "KTVStart.h"
#include "DlgSetting.h"
#include "afxdialogex.h"


// DlgSetting 对话框

IMPLEMENT_DYNAMIC(DlgSetting, CDialogEx)

DlgSetting::DlgSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SETTING, pParent)
{

}

DlgSetting::~DlgSetting()
{
}

void DlgSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgSetting, CDialogEx)
	ON_BN_CLICKED(IDOK, &DlgSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDRESTART, &DlgSetting::OnBnClickedRestart)
END_MESSAGE_MAP()

BOOL DlgSetting::PreTranslateMessage(MSG* pMsg)
{
     //屏蔽ESC关闭窗体/
     if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) 
		 return TRUE;
     else
        return CDialogEx::PreTranslateMessage(pMsg);
}
// DlgSetting 消息处理程序

BOOL DlgSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	Config		config;
	config.ReadFile("config.conf");
	string url= "https://www.baidu.com";
	url = config.Read("DefaultUrl", url);	
	SetDlgItemText(IDC_EDIT_URL, StringToWString(url).c_str());
	url = config.Read("ErrorUrl", url);
	SetDlgItemText(IDC_EDIT_URL2, StringToWString(url).c_str());
	url = config.Read("TestUrl", url);
	SetDlgItemText(IDC_EDIT_URL3, StringToWString(url).c_str());
	return true;
}

void DlgSetting::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString	url;
	Config		config;
	GetDlgItemText(IDC_EDIT_URL, url);
	config.ReadFile("config.conf");
	config.Add("DefaultUrl", WStringToString(wstring(url)));
	GetDlgItemText(IDC_EDIT_URL2, url);
	config.Add("ErrorUrl", WStringToString(wstring(url)));
	GetDlgItemText(IDC_EDIT_URL3, url);
	config.Add("TestUrl", WStringToString(wstring(url)));
	config.SaveFile();
	CDialogEx::OnOK();
}


void DlgSetting::OnBnClickedRestart()
{
	// TODO: 在此添加控件通知处理程序代码
	CString	url;
	Config		config;
	GetDlgItemText(IDC_EDIT_URL, url);
	config.ReadFile("config.conf");
	config.Add("DefaultUrl", WStringToString(wstring(url)));
	GetDlgItemText(IDC_EDIT_URL2, url);
	config.Add("ErrorUrl", WStringToString(wstring(url)));
	GetDlgItemText(IDC_EDIT_URL3, url);
	config.Add("TestUrl", WStringToString(wstring(url)));
	config.SaveFile();
	CDialogEx::OnCancel();
}
