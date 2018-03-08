#include "stdafx.h"

#include "base/Cef3/browser/cef_main_handler.h"
#include "main/WndSimpleFrame.h"

template <class T>
CWindowWnd* CreateMainWnd(LPCTSTR pstrName)
{
	CWindowWnd *mainFrame = new T;
	mainFrame->Create(NULL, pstrName
		, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS
		, WS_EX_OVERLAPPEDWINDOW
		, 0, 0, 0, 0);
	mainFrame->CenterWindow();
	mainFrame->ShowWindow();

	return mainFrame;
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{ 
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// ------------------CEF��ʼ��------------------
	CCefMainHandler CefMainHandler;
	if (!CefMainHandler.Initialize(hInstance, TRUE))
	{
		return -1;
	}

	// ------------------duilib��ʼ��------------------
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("Skin\\"));
	if (FAILED(::CoInitialize(NULL)))
	{
		return -1;
	}
	
	// ------------------��ʾ������------------------
	CreateMainWnd<CWndSimpleFrame>(_T("SimpleWnd"));


	// ------------------������Ϣѭ��------------------
	CefMainHandler.RunMessageLoop();
	

	// ------------------�����ر���Դ------------------
	CefMainHandler.Shutdown();

	::CoUninitialize();
	
	return 0;
}
