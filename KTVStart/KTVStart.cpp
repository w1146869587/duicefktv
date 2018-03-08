
// KTVStart.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "KTVStart.h"
#include "KTVStartDlg.h"
#include "include/cef_sandbox_win.h"
#include "simple_app.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#if defined(CEF_USE_SANDBOX)
// The cef_sandbox.lib static library is currently built with VS2013. It may not
// link successfully with other VS versions.
#pragma comment(lib, "cef_sandbox.lib")
#endif

// CKTVStartApp

BEGIN_MESSAGE_MAP(CKTVStartApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

Config		g_config;
// CKTVStartApp 构造

CKTVStartApp::CKTVStartApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}
// 全局Hook ID
HHOOK g_HookHwnd = NULL;


// 唯一的一个 CKTVStartApp 对象

CKTVStartApp theApp;


// CKTVStartApp 初始化

BOOL CKTVStartApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。



	// Enable High-DPI support on Windows 7 or newer.
	CefEnableHighDPISupport();

	//wchar_t path[MAX_PATH];
	//GetModuleFileName(NULL, path, sizeof(path));
	////判断环境是否为WOW64  
	//BOOL isWOW64;
	//REGSAM p;
	//IsWow64Process(GetCurrentProcess(), &isWOW64);
	//if (isWOW64) {
	// p = KEY_WRITE | KEY_WOW64_64KEY;
	//}
	//else {
	// p = KEY_WRITE;
	//}
	//HKEY hKey;
	//if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, NULL, 0, p, NULL, &hKey, NULL) != ERROR_SUCCESS) {
	// //失败  
	// return 0;
	//}
	//if (RegSetValueEx(hKey, TEXT("KTVStart"), 0, REG_SZ, (BYTE*)path, sizeof(path) * sizeof(TCHAR)) != ERROR_SUCCESS) {
	// //失败  
	// return 0;
	//}
	//RegCloseKey(hKey);

	CefMainArgs main_args(theApp.m_hInstance);

	CefRefPtr<SimpleApp> app(new SimpleApp);

	int exit_code = CefExecuteProcess(main_args, app.get(), NULL);
	if (exit_code >= 0) {
		exit(exit_code);
	}

	CefSettings settings;
	//settings.log_severity = LOGSEVERITY_DISABLE;
	settings.ignore_certificate_errors = true;
	//settings.command_line_args_disabled = 0;
	//settings.Add("enable-media-stream", "enable-media-stream");
	CefSettingsTraits::init(&settings);
	settings.multi_threaded_message_loop = true;
	
	CefInitialize(main_args, settings, app.get(), NULL);





	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	// 安装钩子
	//g_HookHwnd = SetWindowsHookEx(WH_KEYBOARD_LL, MyHookFun, m_hInstance, 0);

	CKTVStartDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。



	return FALSE;
}

