/////////////////////////////////////////////////////////////////////////////
// TrayIcon.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "TrayIcon.h"
#include <assert.h>
#include <commctrl.h>
#include <process.h>

const UINT WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));

CTrayIcon* CTrayIcon::pTray=NULL;

/////////////////////////////////////////////////////////////////////////////
// CTrayIcon construction/creation/destruction
CTrayIcon::CTrayIcon()
{
	memset(&m_tray, 0, sizeof(m_tray));
	m_bIsEnabled = FALSE;
	m_bIsHidden  = TRUE;
}

CTrayIcon::~CTrayIcon()
{
	RemoveIcon();
}
//////////////////////////////////////////////////////////////////////////////

BOOL CTrayIcon::Create(HWND hWnd, UINT uCallbackMessage, LPCTSTR szToolTip, HICON hIcon, UINT uIDMenu/* = 0*/)
{
	// this is only for Windows 95 (or higher)
	m_bIsEnabled = (( GetVersion() & 0xff ) >= 4);
	if (!m_bIsEnabled) return FALSE;

	//Make sure Notification window is valid
	m_bIsEnabled = ::IsWindow(hWnd);
	if (!m_bIsEnabled) return FALSE;
	
	//Make sure we avoid conflict with other messages
	assert(uCallbackMessage >= WM_USER);

	//Tray only supports tooltip text up to 64 characters
	assert(_tcslen(szToolTip) <= 64);

	// load up the NOTIFYICONDATA structure
	m_tray.cbSize = sizeof(NOTIFYICONDATA);
	m_tray.hWnd	 = hWnd;
	m_tray.uID	= uIDMenu;
	m_tray.hIcon  = hIcon;
	m_tray.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	m_tray.uCallbackMessage = uCallbackMessage;
	_tcscpy_s(m_tray.szTip,_countof(m_tray.szTip), szToolTip);

	// Set the tray icon
	m_bIsHidden = !(Shell_NotifyIcon(NIM_ADD, &m_tray));
	m_traypos.SetNotifyIconInfo(hWnd,uIDMenu,uCallbackMessage);
	pTray=this;
    OldWndProc = (WNDPROC)SetWindowLong (hWnd, GWL_WNDPROC, (LONG)MyWndProc);
	return !m_bIsHidden;
}
//////////////////////////////////////////////////////////////////////////

// CTrayIcon icon manipulation
BOOL CTrayIcon::ShowBalloonTip(LPCTSTR szInfo, LPCTSTR szInfoTitle,UINT uTimeout , DWORD dwInfoFlags /* = NIIF_NONE */,HICON hIcon /* = NULL */)
{ 
	if (!m_bIsEnabled || m_bIsHidden) 
		return FALSE;
    if (hIcon)
    {
	    m_tray.hIcon = hIcon;
    }
	m_tray.cbSize=sizeof(NOTIFYICONDATA);
	m_tray.uFlags = m_tray.uFlags | NIF_INFO;
	m_tray.uTimeout = uTimeout;
	m_tray.dwInfoFlags = dwInfoFlags;
	_tcscpy_s(m_tray.szInfo,_countof(m_tray.szInfo),szInfo ? szInfo : _T(""));
	_tcscpy_s(m_tray.szInfoTitle,_countof(m_tray.szInfoTitle),szInfoTitle ? szInfoTitle : _T(""));
	return Shell_NotifyIcon(NIM_MODIFY, &m_tray);
}
/////////////////////////////////////////////////////////////////////////////


BOOL CTrayIcon::RemoveIcon()
{
	if (m_bIsEnabled && !m_bIsHidden) 
	{
		m_bIsHidden = Shell_NotifyIcon (NIM_DELETE, &m_tray);
		return m_bIsHidden;
	}
	return FALSE;
}

BOOL CTrayIcon::LoadIcon()
{
	if (m_bIsEnabled && m_bIsHidden) 
	{
		m_tray.uFlags = NIF_TIP | NIF_MESSAGE | NIF_ICON;
		m_bIsHidden = !(Shell_NotifyIcon(NIM_ADD, &m_tray));
		return !m_bIsHidden;
	}
	return FALSE;
}

BOOL  CTrayIcon::RetrieveIcon()
{
	if (m_bIsEnabled)
	{
		m_bIsHidden = !(Shell_NotifyIcon(NIM_ADD, &m_tray));
		return !m_bIsHidden;
	}
	return FALSE;
}

BOOL CTrayIcon::ModifyIcon(HICON hIcon)
{
	if (!m_bIsEnabled || m_bIsHidden) 
		return FALSE;
    assert(hIcon);
	m_tray.hIcon = hIcon;
	m_tray.uFlags = NIF_TIP | NIF_MESSAGE | NIF_ICON;

	return Shell_NotifyIcon(NIM_MODIFY, &m_tray);
}
///////////////////////////////////////////////////////////////////////////// 
HICON CTrayIcon::GetIcon() const
{
	HICON hIcon = NULL;
	if (m_bIsEnabled)
		hIcon = m_tray.hIcon;

	return hIcon;
}

LPCTSTR CTrayIcon::GetTooltipText() const
{
	if (m_bIsEnabled)
		return m_tray.szTip;
	
	return NULL;
}

HWND CTrayIcon::GetNotificationWnd() const
{
	return m_tray.hWnd;
}
/////////////////////////////////////////////////////////////////////////////
// CTrayIcon tooltip text manipulation

BOOL CTrayIcon::SetTooltipText(LPCTSTR pszTip)
{
	if (!m_bIsEnabled) 
		return FALSE;

	m_tray.uFlags = NIF_TIP | NIF_MESSAGE | NIF_ICON;
	_tcscpy_s(m_tray.szTip,_countof(m_tray.szTip), pszTip);

	return Shell_NotifyIcon(NIM_MODIFY, &m_tray);
}

BOOL CTrayIcon::SetNotificationWnd(HWND hwnd)
{
	if (!m_bIsEnabled)
		return FALSE;

	//Make sure Notification window is valid
	if (!::IsWindow(hwnd))
		return FALSE;

	m_tray.hWnd = hwnd;
	return Shell_NotifyIcon(NIM_MODIFY, &m_tray);
}
/////////////////////////////////////////////////////////////////////////////
LRESULT CTrayIcon::OnNotifyIcon(WPARAM wParam, LPARAM lParam) 
{	
	UINT uID=(UINT)wParam;  //发送该消息的图标id
	UINT uMouseMsg=(UINT)lParam;
	if (m_tray.uID==uID)
	{
		switch(uMouseMsg)
		{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
			{
				if (IsWindowVisible(m_tray.hWnd))
				{
					::ShowWindow(m_tray.hWnd,SW_HIDE);
				}
				else
				{
					::ShowWindow(m_tray.hWnd,SW_SHOW);
				}				
			}break;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
			{
				POINT point;
				GetCursorPos(&point);
				if (m_tray.uID!=0)//为没有设菜单
				{
					HMENU hMenu = LoadMenu((HINSTANCE)GetWindowLong (m_tray.hWnd, GWL_HINSTANCE),MAKEINTRESOURCE(m_tray.uID));
					HMENU hSubMenu = GetSubMenu(hMenu,0);
					SetMenuDefaultItem(hSubMenu,0,TRUE);//选择第一项为默认
					SetForegroundWindow(m_tray.hWnd);  //当点击别的地方的时候弹出菜单消失
					TrackPopupMenu(hSubMenu,TPM_LEFTALIGN,point.x,point.y,0,m_tray.hWnd,NULL);
				}
			}break;
		case WM_MOUSEMOVE:
			{
				m_traypos.OnMouseMove();
			}break;
		default:
			break;
		}
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//     CTrayPos

CTrayPos::CTrayPos()
{
	UINT	uThreadId;
	m_bTrackMouse = FALSE;
	m_hExitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hThread = (HANDLE) _beginthreadex(NULL, 0, CTrayPos::TrackMouseThread, this, 0, &uThreadId);
	InitializeCriticalSection(&m_cs);
}

CTrayPos::~CTrayPos()
{
	if(m_hThread != NULL)
	{
		SetEvent(m_hExitEvent);
		if(WaitForSingleObject(m_hThread, 5000) == WAIT_TIMEOUT)
		{
			TerminateThread(m_hThread, 0);
		}
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	
	if(m_hExitEvent != NULL)
	{
		CloseHandle(m_hExitEvent);
		m_hExitEvent = NULL;
	}
	DeleteCriticalSection(&m_cs);
}

//////////////////////////////////////////////////////////////////////////
UINT CALLBACK CTrayPos::TrackMouseThread(LPVOID lpvoid) //线程函数
{
	POINT		ptMouse;
	CTrayPos	*pTrayPos = (CTrayPos *) lpvoid;

	
	while(WaitForSingleObject(pTrayPos->m_hExitEvent, 500) == WAIT_TIMEOUT)
	{
		if(pTrayPos->m_bTrackMouse == TRUE)
		{
			GetCursorPos(&ptMouse);
			
			if(ptMouse.x != pTrayPos->m_ptMouse.x || ptMouse.y != pTrayPos->m_ptMouse.y)
			{
				pTrayPos->OnMouseLeave();	
				pTrayPos->m_bTrackMouse = FALSE;
			}
		}
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////


VOID CTrayPos::OnMouseMove()
{
	EnterCriticalSection(&m_cs);
	
	GetCursorPos(&m_ptMouse);
	if(m_bTrackMouse == FALSE)
	{
		OnMouseHover();
		m_bTrackMouse = TRUE;
	}
	LeaveCriticalSection(&m_cs);
}

BOOL CTrayPos::IsMouseHover()
{
	return m_bTrackMouse;
}
//////////////////////////////////////////////////////////////////////////

VOID CTrayPos::OnMouseHover()
{
	if(m_hNotifyWnd != NULL && IsWindow(m_hNotifyWnd))
		PostMessage(m_hNotifyWnd, m_uCallbackMsg, m_uID, WM_MOUSEHOVER);
}

VOID CTrayPos::OnMouseLeave()
{
	if(m_hNotifyWnd != NULL && IsWindow(m_hNotifyWnd))
		PostMessage(m_hNotifyWnd, m_uCallbackMsg, m_uID, WM_MOUSELEAVE);
}
//////////////////////////////////////////////////////////////////////////


VOID CTrayPos::SetNotifyIconInfo(HWND hwnd, UINT uID, UINT uCallbackMsg)
{
	m_hNotifyWnd = hwnd;
	m_uID = uID;
	m_uCallbackMsg = uCallbackMsg;
}
//////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK CTrayIcon::MyWndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if (pTray->m_tray.hWnd == hwnd)
	{
		if (uMsg == WM_TASKBARCREATED)
		{
			pTray->RetrieveIcon();                //找回Tray
		}
		else if(uMsg==pTray->m_tray.uCallbackMessage)
		{
			pTray->OnNotifyIcon(wParam,lParam);
		}
		return ::CallWindowProc(pTray->OldWndProc,hwnd,uMsg,wParam,lParam);
	}
	return ::DefWindowProc(hwnd,uMsg,wParam,lParam);
}
//////////////////////////////////////////////////////////////////////////
