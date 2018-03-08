// Daemon.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Daemon.h"
#include "Config.h"
#include <shlobj.h>
#pragma comment( lib, "shell32.lib")
#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例

BOOL CreateFileShortcut(LPCWSTR lpszFileName, LPCWSTR lpszLnkFileDir, LPCWSTR lpszLnkFileName, LPCWSTR lpszWorkDir, WORD wHotkey, LPCTSTR lpszDescription, int iShowCmd = SW_SHOWNORMAL);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	HANDLE handle = ::CreateMutex(NULL, FALSE, L"daemon");//handle为声明的HANDLE类型的全局变量   
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return FALSE;
	}

	CoInitialize(NULL);

	wchar_t path[MAX_PATH];
	GetModuleFileName(NULL, path, sizeof(path));

	wchar_t cuPath[MAX_PATH];                                               //存放路径的变量

	GetCurrentDirectory(MAX_PATH, cuPath);                   //获取程序的当前目录

	LPITEMIDLIST pidl;
	TCHAR szPath[MAX_PATH];
	if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_ALTSTARTUP, &pidl)))
	{
		if (SHGetPathFromIDList(pidl, szPath))
		{
			//SHFILEINFO    sfi;
			//ZeroMemory(&sfi, sizeof(sfi));
			//UINT uFlags = SHGFI_PIDL | SHGFI_DISPLAYNAME;
			//SHGetFileInfo((LPCTSTR)pidl, 0, &sfi, sizeof(SHFILEINFO), uFlags);
			//sDisplayName = sfi.szDisplayName;
			CreateFileShortcut(NULL, szPath, L"daemon.lnk", cuPath, MAKEWORD(VK_F12, HOTKEYF_CONTROL), L"Start Link");
			CoTaskMemFree(pidl);          //free the resource  
		}
	}

	//判断环境是否为WOW64  
	BOOL isWOW64;
	REGSAM p;
	IsWow64Process(GetCurrentProcess(), &isWOW64);
	if (isWOW64) {
	 p = KEY_WRITE | KEY_WOW64_64KEY;
	}
	else {
	 p = KEY_WRITE;
	}
	
	//HKEY hlmKey;
	//if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, NULL, 0, p, NULL, &hlmKey, NULL) != ERROR_SUCCESS) {
	//	//失败  
	//	//return 0;
	//}
	//if (RegSetValueEx(hlmKey, TEXT("Daemon"), 0, REG_SZ, (BYTE*)path, sizeof(path) * sizeof(TCHAR)) != ERROR_SUCCESS) {
	//	//失败  
	//	//return 0;
	//}
	//RegCloseKey(hlmKey);
	HKEY hcuKey;
	if (RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, NULL, 0, p, NULL, &hcuKey, NULL) != ERROR_SUCCESS) {
		//失败  
		//return 0;
	}
	if (RegSetValueEx(hcuKey, TEXT("Daemon"), 0, REG_SZ, (BYTE*)path, sizeof(path) * sizeof(TCHAR)) != ERROR_SUCCESS) {
		//失败  
		//return 0;
	}
	RegCloseKey(hcuKey);

	HKEY hErrorReportKey;
	if (RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\Windows Error Reporting"), 0, NULL, 0, p, NULL, &hErrorReportKey, NULL) != ERROR_SUCCESS) {
		//失败  
		//return 0;
	}
	DWORD Data = 1;
	if (RegSetValueEx(hErrorReportKey, TEXT("DontShowUI"), 0, REG_DWORD, (LPBYTE)&Data, sizeof(Data)) != ERROR_SUCCESS) {
		//失败  
		//return 0;
	}
	if (RegSetValueEx(hErrorReportKey, TEXT("Disabled"), 0, REG_DWORD, (LPBYTE)&Data, sizeof(Data)) != ERROR_SUCCESS) {
		//失败  
		//return 0;
	}
	RegCloseKey(hErrorReportKey);

	while (1)
	{
		Config		config;
		config.ReadFile("config.conf");
		string pProgramPath = "";
		pProgramPath = config.Read("pProgramPath", pProgramPath);

		STARTUPINFO si = { 0 };
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);
		si.wShowWindow = SW_SHOW;
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

		// 运行进程
		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(pi));

		BOOL bRet = FALSE;
		bRet = CreateProcess(StringToWString(pProgramPath).c_str(), NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi);

		if (bRet)
		{
			WaitForSingleObject(pi.hProcess, INFINITE);
		}
		else
		{
			return 0;
		}

	}
	return 0;
}



BOOL CreateFileShortcut(LPCWSTR lpszFileName, LPCWSTR lpszLnkFileDir, LPCWSTR lpszLnkFileName, LPCWSTR lpszWorkDir, WORD wHotkey, LPCTSTR lpszDescription, int iShowCmd)
{
	if (lpszLnkFileDir == NULL)
		return FALSE;

	HRESULT hr;
	IShellLink     *pLink;  //IShellLink对象指针  
	IPersistFile   *ppf; //IPersisFil对象指针  

						 //创建IShellLink对象  
	hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pLink);
	if (FAILED(hr))
		return FALSE;

	//从IShellLink对象中获取IPersistFile接口  
	hr = pLink->QueryInterface(IID_IPersistFile, (void**)&ppf);
	if (FAILED(hr))
	{
		pLink->Release();
		return FALSE;
	}

	//目标  
	if (lpszFileName == NULL)
		pLink->SetPath(_wpgmptr);
	else
		pLink->SetPath(lpszFileName);

	//工作目录  
	if (lpszWorkDir != NULL)
		pLink->SetWorkingDirectory(lpszWorkDir);

	//快捷键  
	if (wHotkey != 0)
		pLink->SetHotkey(wHotkey);

	//备注  
	if (lpszDescription != NULL)
		pLink->SetDescription(lpszDescription);

	//显示方式  
	pLink->SetShowCmd(iShowCmd);


	//快捷方式的路径 + 名称  
	wchar_t szBuffer[MAX_PATH];
	if (lpszLnkFileName != NULL) //指定了快捷方式的名称  
		wsprintf(szBuffer, L"%s\\%s", lpszLnkFileDir, lpszLnkFileName);
	//保存快捷方式到指定目录下  

	hr = ppf->Save(szBuffer, TRUE);

	ppf->Release();
	pLink->Release();
	return SUCCEEDED(hr);
}

