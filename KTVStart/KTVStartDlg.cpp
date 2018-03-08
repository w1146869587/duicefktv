
// KTVStartDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KTVStart.h"
#include "KTVStartDlg.h"
#include "afxdialogex.h"
#include "simple_app.h"
#include "DlgSetting.h"
#include "afxinet.h"
#include <thread>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TAG "MainDialog"

HWND		g_WND;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CKTVStartDlg 对话框



CKTVStartDlg::CKTVStartDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_KTVSTART_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKTVStartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CKTVStartDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CKTVStartDlg 消息处理程序

BOOL CKTVStartDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码


	int  cx = GetSystemMetrics(SM_CXSCREEN);
	int  cy = GetSystemMetrics(SM_CYSCREEN);
	RECT rc;
	rc.top = 0;
	rc.left = 0;
	rc.right = cx;
	rc.bottom = cy;
	MoveWindow(&rc);
	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);


	m_bClose = false;
	std::thread th(std::bind(&CKTVStartDlg::CreateBrowser, this));
	th.detach();
	g_WND = m_hWnd;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CKTVStartDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CKTVStartDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CKTVStartDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CKTVStartDlg::OnDestroy()
{
	// Shut down CEF.
	m_bClose = true;
	CefShutdown();
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}


void CKTVStartDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}
void CKTVStartDlg::CreateBrowser()
{
	Sleep(5000);
	CefRefPtr<SimpleHandler> handler(new SimpleHandler(true));
	g_handler = handler;
	//g_handler = new SimpleHandler(false);
	Config		config;
	config.ReadFile("config.conf");
	string url = "https://www.baidu.com";
	url = config.Read("DefaultUrl", url);
	//url = "http://statics.holdfun.cn/serviceplatform/bar/official/pc/index.html?bu=d9252e4d31ed44adb1dfbb487df55582&qrcodeType=app";
	g_handler->m_strDefaultUrl = url;
	string errurl = "https://www.sina.com.cn";
	errurl = config.Read("ErrorUrl", errurl);
	g_handler->m_strErrorUrl = errurl;
	string testurl = "http://xshare.holdfun.cn/bportal/api/common/time";
	testurl = config.Read("TestUrl", testurl);
	g_handler->m_strTestUrl = testurl;
	RECT rc;

	CefWindowInfo window_info;
	GetClientRect(&rc);
	//window_info.SetAsPopup(m_hWnd, "1111");
	window_info.SetAsChild(m_hWnd, rc);
	//window_info.SetAsWindowless(m_hWnd);

	// Specify CEF browser settings here.
	CefBrowserSettings browser_settings;

	LOGI("程序启动，加载远程页面");
	// Create the first browser window.
	bool ret = CefBrowserHost::CreateBrowser(window_info, g_handler, url, browser_settings, NULL);
	if (ret)
	{
		OutputDebugString(L"create success\n");
	}
	else
	{
		OutputDebugString(L"create failed\n");
	}

	bool bFailed = false;
	int errornum = 0;
	int i = 0;
	bool LoadOver = false;
	bool bStartLoad = false;
	while (!m_bClose)
	{
		Sleep(100);
		i++;
		if (i > 50)
		{
			::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			
			//if (TestHostUrl() < 0)
			//{
			//	
			//	errornum++;				
			//	if (errornum > 1&& !bFailed)//测试没通过,目前还处在远程页面
			//	{
			//		LOGI("第三次测试连接请求错误，准备加载本地页面");
			//		g_handler->ReLoadError();
			//		bFailed = true;
			//		errornum = 0;
			//	}
			//}
			//else if (bFailed||g_handler->m_bLoadError) //测试通过，但是目前处在本地页面
			//{
			//	LOGI("测试连接恢复，准备加载远程页面");
			//	g_handler->ReLoadDefault();
			//	bFailed = false;
			//	errornum = 0;
			//}
			if (g_handler->m_bLoadError&&!LoadOver)
			{
				LOGI("没有网络，测试网络连接中。。。");
				if (TestHostUrl() >= 0)
				{
					LOGI("测试连接恢复，准备加载远程页面");
					g_handler->ReLoadDefault();
					bStartLoad = true;
				}
			}
			if (!g_handler->m_bLoadError&&bStartLoad)
			{
				LoadOver = true;
			}
			i = 0;
		}
	}
}


BOOL CKTVStartDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_USER+100)     
	{
		DlgSetting dlg;
		if (dlg.DoModal() == IDOK)
		{
			SetFocus();
		}
		else
		{
			CDialogEx::OnCancel();
		}
	}
	if (pMsg->message == WM_USER + 101) 
	{
		RECT rc;
		rc.top = 0;
		rc.left = 0;
		rc.right = 1;
		rc.bottom = 1;
		MoveWindow(&rc);
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	else
		return CDialogEx::PreTranslateMessage(pMsg);
}



int CKTVStartDlg::TestHostUrl()
{
	CInternetSession *pInetSession = new CInternetSession(AfxGetAppName(), 1, PRE_CONFIG_INTERNET_ACCESS);
	CHttpConnection *pHttpConn = NULL;
	CHttpFile *pHttpFile = NULL;
	CString strUrl;
	DWORD dwServiceType;
	CString strServer, strObj, strUser, strPwd;
	INTERNET_PORT nPort;

	pInetSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 5000);      // 5秒的连接超时
	pInetSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 1000);           // 1秒的发送超时
	pInetSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 5000);        // 7秒的接收超时
	pInetSession->SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, 1000);     // 1秒的发送超时
	pInetSession->SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, 5000);       // 7秒的接收超时
	pInetSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);          // 1次重试

	strUrl = StringToWString(g_handler->m_strTestUrl).c_str();// "http://www.tdx.com.cn/products/data/data/dbf/base.zip";
	if (!AfxParseURLEx(strUrl, dwServiceType, strServer, strObj, nPort, strUser, strPwd, ICU_NO_ENCODE))
	{
		//MessageBox("下载地址有误！...");
		return -1;
	}
	if (!pHttpConn)
	{
		pHttpConn = pInetSession->GetHttpConnection(strServer);
	}
	int nVal = 6000;
	pInetSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, &nVal, sizeof(nVal));
	pHttpFile = pHttpConn->OpenRequest(L"GET", strObj);
	try
	{
		//MessageBox(_T("正在连接到网站..."));
		pHttpFile->SendRequest();
	}
	catch (CInternetException *pEx)
	{
		//MessageBox("下载出错...");
		pEx->Delete();
		delete pHttpFile;
		return -1;
	}
	DWORD dwRet;
	pHttpFile->QueryInfoStatusCode(dwRet);
	if (dwRet != HTTP_STATUS_OK)
	{
		//CString error;
		//error.Format("出错了，错误码：%d", dwRet);
		//AfxMessageBox(error);
		delete pHttpFile;
		return -1;
	}
	delete pHttpFile;
	return 0;
}
