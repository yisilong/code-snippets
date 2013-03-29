#include "Duilib3dFrame.h"
#include "ControlEx.h"
#include "resource.h"
CDuilib3dFrame::CDuilib3dFrame() 
{

}

CDuilib3dFrame::~CDuilib3dFrame() 
{
}

void CDuilib3dFrame::Init() 
{
}

void CDuilib3dFrame::Notify(TNotifyUI& msg)
{
	CStdString strName = msg.pSender->GetName();
	if(msg.sType == _T("windowinit"))
	{
		
	}
	else if (msg.sType == _T("click"))
	{
		if (strName == _T("closebtn"))
		{
			//Close();
			m_cls.Create(m_hWnd,230,false,SW_MINIMIZE,CCloseAnimation::STYLE_CLOSE_ICON);
		}

		else if (strName == _T("scanbtn"))
		{
			CControlUI* pControl = m_PaintManager.FindControl(_T("testanibtn"));
			RECT rc = pControl->GetPos();
			rc.right = rc.left+64;
			rc.bottom = rc.top+64;
			POINT ptCenter = {rc.left+32,rc.top+32};
			POINT ptTrans = {-120,100};
			CAnimJobUI anim(0,500,ptCenter,ptTrans,1.0f,1.0f);
			anim.SetTextureFromFile(_T("../bin/skin/texture.png"));
			m_PaintManager.AddAnimJob(anim);
		}
		else if (strName == _T("common") || strName == _T("mumaoption")
			 || strName == _T("shaduoption") || strName == _T("wangdunoption")
			  || strName == _T("fdhoption") || strName == _T("rjgjoption"))
		{
			RECT rc = msg.pSender->GetPos();
			POINT ptStartCenter = {rc.left+(rc.right-rc.left)/2,rc.top+(rc.bottom-rc.top)/2};
			POINT ptTrans = {200,0};
			CAnimJobUI anim(0,500,ptStartCenter,ptTrans,1.0f,1.0f,false,false,rc,rc);
			anim.SetSpeedType(SPEED_LINEAR);
			m_PaintManager.AddAnimJob(anim);
		}
		else if (strName == _T("testanibtn"))
		{
		}
	}
	else if(msg.sType==_T("selectchanged"))
	{
		CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("switch")));			
		RECT rcSrc = pControl->GetPos();
		RECT rcClip = {rcSrc.left+1,rcSrc.top+1,rcSrc.right-1,rcSrc.bottom};;
		long rcWidth = rcClip.right - rcClip.left;
		long rcHeight = rcClip.bottom - rcClip.top;
		CStdString stdString[8] = {_T("examine"),_T("trojan"),_T("plugins"),_T("vulnerability"),_T("rubbish"),_T("cleanup"),_T("fix"),_T("tool")};
		POINT ptStartCenter = {rcClip.left+rcWidth/2,rcClip.top+rcHeight/2};
		POINT ptTrans = {-rcWidth,0};

		for (int i=0;i<8;++i)
		{
			if (strName == stdString[i])
			{
				CAnimJobUI anim(0,500,ptStartCenter,ptTrans,1.0f,1.0f,true,true,rcClip,rcClip);
				m_PaintManager.AddAnimJob(anim);
				pControl->SelectItem(i);
				ptStartCenter.x += rcWidth;
				CAnimJobUI animTmp(0,500,ptStartCenter,ptTrans,1.0f,1.0f,true,true,rcClip,rcClip,WM_USER+1,pControl);				
				if (m_PaintManager.AddAnimJob(animTmp))
				{
					pControl->SetVisible(false);
				}
			}
		}
	}
	else if (msg.sType == _T("tabselect"))
	{
	}
}

LRESULT CDuilib3dFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	case WM_USER+1:
		{
			CControlUI* pControl = (CControlUI*)lParam;
			pControl->SetVisible(true);
		}break;
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

LRESULT CDuilib3dFrame::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	styleValue &= ~WS_THICKFRAME;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	HMENU hMenu = GetSystemMenu(m_hWnd,FALSE);
	if (hMenu != NULL)
	{
		DeleteMenu(hMenu,SC_MAXIMIZE,MF_BYCOMMAND);
	}

	//根据skin.xml创建程序界面
	m_PaintManager.Init(m_hWnd);

	CDialogBuilder builder;
	CDialogBuilderCallbackEx cb;
	CControlUI* pRoot = builder.Create(_T("skin.xml"), (UINT)0,  &cb, &m_PaintManager);
	ASSERT(pRoot && "Failed to parse XML");

	m_PaintManager.AttachDialog(pRoot);
	m_PaintManager.AddNotifier(this);

	HICON hIcon = LoadIcon((HINSTANCE)GetWindowLong(m_hWnd,GWL_HINSTANCE),MAKEINTRESOURCE(IDI_ICON));
	m_tray.Create(m_hWnd,WM_USER+1021,_T("360安全卫士"),hIcon,NULL);

	return 0;
}

LRESULT CDuilib3dFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CDuilib3dFrame::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostQuitMessage(0L);

	bHandled = FALSE;
	return 0;
}

LRESULT CDuilib3dFrame::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CDuilib3dFrame::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CDuilib3dFrame::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CDuilib3dFrame::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

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

LRESULT CDuilib3dFrame::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_PaintManager.GetRoundCorner();

	if( !::IsIconic(*this) 
		&& (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) 
	{
		CRect rcWnd;
		::GetWindowRect(*this, &rcWnd);

		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; 
		rcWnd.bottom++;

		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;
	return 0;
}

LRESULT CDuilib3dFrame::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CRect rcWork = oMonitor.rcWork;
	rcWork.Offset(-rcWork.left, -rcWork.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
	lpMMI->ptMaxPosition.x	= rcWork.left;
	lpMMI->ptMaxPosition.y	= rcWork.top;
	lpMMI->ptMaxSize.x		= rcWork.right;
	lpMMI->ptMaxSize.y		= rcWork.bottom;

	bHandled = FALSE;
	return 0;
}

LRESULT CDuilib3dFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if( wParam == SC_CLOSE ) 
	{
		::PostMessage (m_hWnd, WM_CLOSE, 0, 0);
		//::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}

	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if( ::IsZoomed(*this) != bZoomed ) 
	{
		if( !bZoomed ) 
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
			if (pControl)
			{
				pControl->SetVisible(false);
			}

			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));
			if (pControl)
			{
				pControl->SetVisible(true);
			}
		}
		else
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
			if (pControl) 
			{
				pControl->SetVisible(true);
			}

			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));
			if (pControl) 
			{
				pControl->SetVisible(false);
			}
		}
	}
	return lRes;
}
