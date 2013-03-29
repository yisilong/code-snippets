/////////////////////////////////////////////////////////////////////////////
// TrayIcon.h : header file
//////////////////////////////////////////////////////////////////////////   
#pragma once
#include <shellapi.h>
#pragma comment(lib, "shell32.lib ") 
class CTrayPos
{
private:
	POINT				m_ptMouse;	
	HANDLE				m_hThread;
	HANDLE				m_hExitEvent;	
	BOOL				m_bTrackMouse;
	CRITICAL_SECTION	m_cs;
private:
	HWND	m_hNotifyWnd;
	UINT	m_uID;
	UINT	m_uCallbackMsg;
	
public:
	CTrayPos();
	virtual ~CTrayPos();
	
	static UINT CALLBACK TrackMouseThread(LPVOID lpvoid);                //Ïß³Ì
	VOID OnMouseMove();
	BOOL IsMouseHover();
	VOID SetNotifyIconInfo(HWND hwnd, UINT uID, UINT uCallbackMsg);	
protected:
	virtual VOID OnMouseHover();
	virtual VOID OnMouseLeave();
};	

//////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
class CTrayIcon
{
protected:
	BOOL			m_bIsEnabled;	// does OS support tray icon?
	BOOL			m_bIsHidden;	// Has the icon been hidden?
	NOTIFYICONDATA	m_tray;
    static CTrayIcon *pTray;
	WNDPROC         OldWndProc; 
	CTrayPos        m_traypos;
// Construction/destruction
public:
	CTrayIcon();
	virtual ~CTrayIcon();

// Operations
public:
	BOOL IsEnabled() { return m_bIsEnabled; }
	BOOL IsVisible() { return !m_bIsHidden; }
	
	//Create the tray icon
	BOOL Create(HWND hWnd, UINT uCallbackMessage, LPCTSTR szTip, HICON hIcon, UINT uIDMenu = 0);
	
	//Change or retrieve the Tooltip text
	BOOL    SetTooltipText(LPCTSTR pszTooltipText);
	LPCTSTR GetTooltipText() const;
	
	//Change or retrieve the icon displayed
	BOOL  ModifyIcon(HICON hIcon);
	HICON GetIcon() const;
	BOOL  RemoveIcon();
	BOOL  LoadIcon();
                               
	BOOL  ShowBalloonTip(LPCTSTR szInfo, LPCTSTR szInfoTitle,UINT uTimeout, DWORD dwInfoFlags = NIIF_NONE,HICON hIcon = NULL);
	//Change or retrieve the window to send notification messages to  
	BOOL  SetNotificationWnd(HWND hwnd);
	HWND  GetNotificationWnd() const;
private:
	BOOL  RetrieveIcon();  	//Only use when explore restart
	//Default handler for tray notification message
	virtual LRESULT OnNotifyIcon(WPARAM wParam,LPARAM lParam);
	//wndproc
    static LRESULT CALLBACK MyWndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
};

/////////////////////////////////////////////////////////////////////////////
