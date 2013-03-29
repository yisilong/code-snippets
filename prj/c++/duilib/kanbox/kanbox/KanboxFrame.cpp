#include "KanboxFrame.h"

#define WM_ICON_NOTIFY (WM_USER+1)

CKanboxFrame::CKanboxFrame(void)
{
}

CKanboxFrame::~CKanboxFrame(void)
{
}
void CKanboxFrame::Init() 
{
}

void CKanboxFrame::Notify(TNotifyUI& msg)
{
	if (msg.pSender == NULL)
	{
		return;
	}
	CDuiString strName = msg.pSender->GetName();
	if (msg.sType == _T("windowinit"))
	{

	}
	else if (msg.sType == _T("click"))
	{
		if (strName == _T("sys_min_btn"))
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); 
		}
		else if (strName == _T("sys_max_btn"))
		{
			SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE,0);
		}
		else if (strName == _T("sys_restore_btn"))
		{
			SendMessage(WM_SYSCOMMAND,SC_RESTORE,0);
		}
		else if (strName == _T("sys_close_btn"))
		{
			//SendMessage(WM_SYSCOMMAND,SC_CLOSE,0);
			ShowWindow(false);
		}
	}
}

LRESULT CKanboxFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;

	switch( uMsg ) 
	{
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	case WM_COMMAND:	   lRes = OnCommand(uMsg,wParam,lParam,bHandled);break;
	default:
		bHandled = FALSE;
	}

	if( bHandled )
	{
		return lRes;
	}

	if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) 
	{
		return lRes;
	}

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CKanboxFrame::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(m_hWnd, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	styleValue &= ~WS_THICKFRAME;
	::SetWindowLong(m_hWnd, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

// 	HMENU hMenu = GetSystemMenu(m_hWnd,FALSE);
// 	if (hMenu != NULL)
// 	{
// 		DeleteMenu(hMenu,SC_MAXIMIZE,MF_BYCOMMAND);
// 	}

	//根据skin.xml创建程序界面
	m_PaintManager.Init(m_hWnd);

	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("skin.xml"), (UINT)0,  NULL, &m_PaintManager);
	ASSERT(pRoot && "Failed to parse XML");

	m_PaintManager.AttachDialog(pRoot);
	m_PaintManager.AddNotifier(this);

	SetIcon(IDI_SHELLICON);
	HICON hIcon = LoadIcon((HINSTANCE)GetWindowLong(m_hWnd,GWL_HINSTANCE),MAKEINTRESOURCE(IDI_SHELLICON));
	m_trayIcon.Create(m_hWnd,WM_ICON_NOTIFY,_T("kanbox便携版"),hIcon,IDR_SHELLMENU);
	return 0;
}

LRESULT CKanboxFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CKanboxFrame::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostQuitMessage(0L);

	bHandled = FALSE;
	return 0;
}

LRESULT CKanboxFrame::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(m_hWnd) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CKanboxFrame::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CKanboxFrame::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CKanboxFrame::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(m_hWnd, &pt);

	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	RECT rcCaption = m_PaintManager.GetCaptionRect();

	if( pt.x >= rcClient.left + rcCaption.left 
		&& pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top 
		&& pt.y < rcCaption.bottom )
	{
		CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(pt));

		if( pControl != NULL
			&& _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 
			&& _tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 
			&& _tcscmp(pControl->GetClass(), _T("TextUI")) != 0 )
		{
			return HTCAPTION;
		}
	}

	return HTCLIENT;
}

LRESULT CKanboxFrame::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_PaintManager.GetRoundCorner();

	if( !::IsIconic(m_hWnd) 
		&& (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) 
	{
		CDuiRect rcWnd;
		::GetWindowRect(m_hWnd, &rcWnd);

		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; 
		rcWnd.bottom++;

		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(m_hWnd, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;
	return 0;
}

LRESULT CKanboxFrame::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
	rcWork.Offset(-rcWork.left, -rcWork.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
	lpMMI->ptMaxPosition.x	= rcWork.left;
	lpMMI->ptMaxPosition.y	= rcWork.top;
	lpMMI->ptMaxSize.x		= rcWork.right;
	lpMMI->ptMaxSize.y		= rcWork.bottom;

	bHandled = FALSE;
	return 0;
}

LRESULT CKanboxFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if( wParam == SC_CLOSE ) 
	{
		//::PostMessage (m_hWnd, WM_CLOSE, 0, 0);
		//::PostQuitMessage(0L);
		ShowWindow(false);
		bHandled = TRUE;
		return 0;
	}

	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if( ::IsZoomed(m_hWnd) != bZoomed ) 
	{
		if( !bZoomed )
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("sys_max_btn")));
			if (pControl)
			{
				pControl->SetVisible(false);
			}

			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("sys_restore_btn")));
			if (pControl)
			{
				pControl->SetVisible(true);
			}
		}
		else
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("sys_max_btn")));
			if (pControl) 
			{
				pControl->SetVisible(true);
			}

			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("sys_restore_btn")));
			if (pControl) 
			{
				pControl->SetVisible(false);
			}
		}
	}
	return lRes;
}

LRESULT CKanboxFrame::OnCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	switch(HIWORD(wParam))
	{
	case 0:  // menu
		{
			switch(LOWORD(wParam))
			{
			case ID_SHELLMENU_SHOW:
				{
					if (IsWindowVisible(m_hWnd))
					{
						ShowWindow(false);
					}
					else
					{
						ShowWindow(true);
					}
				}break;
			case ID_SHELLMENU_EXIT:
				{
					PostMessage(WM_CLOSE);
				}break;
			default:break;
			}
		}break;
	case 1:  // Accelerator
		break;
	default: // Control-defined notification code
		break;
	}
	return 0;
}
