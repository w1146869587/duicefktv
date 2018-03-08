// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
#include "Ping.h"
#include "simple_handler.h"
#include "P2PLog.h"
#include <sstream>
#include <string>
#include <thread>

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
//#include "include/views/cef_browser_view.h"
//#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

#define TAG "CefHandler"
extern HWND		g_WND;
namespace {

SimpleHandler* g_instance = NULL;

}  // namespace

SimpleHandler::SimpleHandler(bool use_views)
    : use_views_(use_views), is_closing_(false) {
  DCHECK(!g_instance);
  g_instance = this;
}

SimpleHandler::~SimpleHandler() {
  g_instance = NULL;
}

// static
SimpleHandler* SimpleHandler::GetInstance() {
  return g_instance;
}

void SimpleHandler::OnTitleChange(CefRefPtr<CefBrowser> browser,
                                  const CefString& title) {
  CEF_REQUIRE_UI_THREAD();

  if (use_views_) {
    // Set the title of the window using the Views framework.
    //CefRefPtr<CefBrowserView> browser_view =
    //    CefBrowserView::GetForBrowser(browser);
    //if (browser_view) {
    //  CefRefPtr<CefWindow> window = browser_view->GetWindow();
    //  if (window)
    //    window->SetTitle(title);
    //}
  } else {
    // Set the title of the window using platform APIs.
    PlatformTitleChange(browser, title);
  }
}

void SimpleHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  // Add to the list of existing browsers.
  browser_list_.push_back(browser);
}

bool SimpleHandler::DoClose(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  // Closing the main window requires special handling. See the DoClose()
  // documentation in the CEF header for a detailed destription of this
  // process.
  if (browser_list_.size() == 1) {
    // Set a flag to indicate that the window close should be allowed.
    is_closing_ = true;
  }

  // Allow the close. For windowed browsers this will result in the OS close
  // event being sent.
  return false;
}

void SimpleHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  // Remove from the list of existing browsers.
  BrowserList::iterator bit = browser_list_.begin();
  for (; bit != browser_list_.end(); ++bit) {
    if ((*bit)->IsSame(browser)) {
      browser_list_.erase(bit);
      break;
    }
  }

  if (browser_list_.empty()) {
    // All browser windows have closed. Quit the application message loop.
    CefQuitMessageLoop();
  }
}
bool SimpleHandler::OnPreKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event,
	CefEventHandle os_event,
	bool* is_keyboard_shortcut)
{
	if (os_event->message == WM_KEYDOWN)
	{
		switch (os_event->wParam)
		{
		case VK_ESCAPE:
			PostMessage(g_WND, WM_USER + 100, 1, 1);
			break;
		case VK_F12:
			PostMessage(g_WND, WM_USER + 101, 1, 1);
			break;

		}
	}
		
	
	return true;
}
void SimpleHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                ErrorCode errorCode,
                                const CefString& errorText,
                                const CefString& failedUrl) {
  CEF_REQUIRE_UI_THREAD();

  // Don't display an error for downloaded files.
  if (errorCode == ERR_ABORTED)
    return;

  // Display a load error message.
  //std::stringstream ss;
  //ss << "<html><body bgcolor=\"white\">"
  //      "<h2>Failed to load URL "
  //   << std::string(failedUrl) << " with error " << std::string(errorText)
  //   << " (" << errorCode << ").</h2></body></html>";
  //frame->LoadString(ss.str(), failedUrl);
  LOGI("加载页面错误 跳转到本地页面");
  frame->LoadURL(m_strErrorUrl);
  //frame->LoadURL("file:///C:/Users/WhaleyUPC/Desktop/1.html");
  m_bLoadError = true;
  //std::thread th(std::bind(&SimpleHandler::CheckNetworkThread, this));
  //th.detach();  
}
void SimpleHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	int httpStatusCode)
{
	CefString cefstr = frame->GetURL();
	if (cefstr == m_strDefaultUrl&&httpStatusCode == 200)
	{
		LOGI("加载远程页面成功");
		m_bLoadError = false;
	}
}
CefRefPtr<CefResourceHandler> SimpleHandler::GetResourceHandler(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefRequest> request)
{
	OutputDebugString(L"GetResourceHandler\n");
	return NULL;
}
bool SimpleHandler::OnResourceResponse(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefRequest> request,
	CefRefPtr<CefResponse> response)
{
	OutputDebugString(L"OnResourceResponse\n");
	return false;
}
void  SimpleHandler::OnResourceLoadComplete(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefRequest> request,
	CefRefPtr<CefResponse> response,
	URLRequestStatus status,
	int64 received_content_length)
{
	OutputDebugString(L"OnResourceLoadComplete\n");
}
bool SimpleHandler::OnQuotaRequest(CefRefPtr<CefBrowser> browser,
	const CefString& origin_url,
	int64 new_size,
	CefRefPtr<CefRequestCallback> callback)
{
	OutputDebugString(L"OnQuotaRequest\n");
	return false;
}

void SimpleHandler::CloseAllBrowsers(bool force_close) {
  if (!CefCurrentlyOn(TID_UI)) {
    // Execute on the UI thread.
    CefPostTask(TID_UI, base::Bind(&SimpleHandler::CloseAllBrowsers, this,
                                   force_close));
    return;
  }

  if (browser_list_.empty())
    return;

  BrowserList::const_iterator it = browser_list_.begin();
  for (; it != browser_list_.end(); ++it)
    (*it)->GetHost()->CloseBrowser(force_close);
}

bool	SimpleHandler::ReLoadDefault()
{
	BrowserList::const_iterator it = browser_list_.begin();
	for (; it != browser_list_.end(); ++it)
	{
		(*it)->GetMainFrame()->LoadURL(m_strDefaultUrl);
		OutputDebugString(L"load defalut url\n");
	}
	return true;
}
bool	SimpleHandler::ReLoadError()
{
	BrowserList::const_iterator it = browser_list_.begin();
	for (; it != browser_list_.end(); ++it)
	{
		(*it)->GetMainFrame()->LoadURL(m_strErrorUrl);
		OutputDebugString(L"load error url\n");
	}
	return true;
}
/*
void	SimpleHandler::CheckNetworkThread()
{
	CPing objPing;
	struct hostent* pHostEnt;
	//char *szDestIP = "223.5.5.5";
	//char *szDestIP = "61.135.169.125";
	char szDestIP[100] = { 0 };
	strcpy(szDestIP, "61.135.169.125");
	struct sockaddr_in   sAddr;
	std::string url = m_strDefaultUrl;
	int pos1 = -1;
	pos1 = url.find("://");
	int pos2 = -1;
	int len = -1;
	if (pos1>0)
	{
		pos2 = url.find("/", pos1 + 3);
		if (pos2 > 0)
		{
			len = pos2 - pos1;
		}
		else
		{
			len = url.length() - pos1;
		}

	}
	
	std::string hosturl = url.substr(pos1 + 3, len - 3);

	PingReply reply;
	while (m_bLoadError)
	{
		if ((pHostEnt = gethostbyname(hosturl.c_str())) == NULL) {
			Sleep(100);
			continue;
		}
		else
		{
			sprintf(szDestIP, "%s", inet_ntoa(*(struct in_addr*)pHostEnt->h_addr_list[0]));
		}
		bool ret = objPing.Ping(szDestIP, &reply);
		if (ret)
		{
			OutputDebugString(L"ip enable\n");
			break;
		}
		else
		{
			OutputDebugString(L"ip unenable\n");
		}
		Sleep(100);

	}
	ReLoad();
}
*/