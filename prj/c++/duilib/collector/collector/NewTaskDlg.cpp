#include "NewTaskDlg.h"
#include "../common/tools.h"
#include "Message.h"

CNewTaskDlg::CNewTaskDlg(HWND hWndOwner)
{	
	assert(IsNotNULL(hWndOwner) && IsWindow(hWndOwner));
	m_hWndOwner = hWndOwner;
	m_nCurrStep = 1;
	m_uListUrlStart = 1;
	m_uListUrlEnd = 100;
	m_uPageUrlStart = 1;
	m_uPageUrlEnd = 1000;
}

CNewTaskDlg::~CNewTaskDlg(void)
{
}
void CNewTaskDlg::Init() 
{
	m_pPrevBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("prev-btn")));
	m_pNextBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("next-btn")));
	m_pOkBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("ok-btn")));
	m_pCancelBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("cancel-btn")));

	m_pStepsHL = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("steps")));
	m_pContentsHL = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("contents")));
	m_pOperatesHL = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("operates")));

	assert(IsNotNULL(m_pPrevBtn) && IsNotNULL(m_pNextBtn) && IsNotNULL(m_pOkBtn) && IsNotNULL(m_pCancelBtn) && IsNotNULL(m_pStepsHL) && IsNotNULL(m_pContentsHL) && IsNotNULL(m_pOperatesHL));

	UINT nIndex = 1;
	TCHAR szBuffer[64];
	CTextUI* pTextUI = NULL;
	// 获取所有step
	for (;;)
	{
		_stprintf_s(szBuffer,64,_T("step%d"),nIndex);
		pTextUI = static_cast<CTextUI*>(m_pStepsHL->FindSubControl(szBuffer));
		if (IsNotNULL(pTextUI))
		{
			m_stepsVec.push_back(pTextUI);
			++nIndex;
			continue;
		}
		nIndex = 1;
		break;
	}
	// 获取所有content
	CHorizontalLayoutUI* pHLUI = NULL;
	for (;;)
	{
		_stprintf_s(szBuffer,64,_T("content%d"),nIndex);
		pHLUI = static_cast<CHorizontalLayoutUI*>(m_pContentsHL->FindSubControl(szBuffer));
		if (IsNotNULL(pHLUI))
		{
			m_contentsVec.push_back(pHLUI);
			++nIndex;
			continue;
		}
		break;
	}

	// 保证数据正确
	assert(m_stepsVec.size() > 0 && m_contentsVec.size() > 0 && m_stepsVec.size() == m_contentsVec.size());

	if (m_stepsVec.size() == 1) // 只有一个页面
	{
		m_pNextBtn->SetVisible(false);
		m_pOkBtn->SetVisible(true);
	}
}

void CNewTaskDlg::Notify(TNotifyUI& msg)
{
	if (IsNULL(msg.pSender))
	{
		return;
	}
	CDuiString strName = msg.pSender->GetName();
	if (msg.sType == _T("windowinit"))
	{
		Init();
	}
	else if (msg.sType == _T("click"))
	{
		if (strName == _T("sys-min-btn"))
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); 
		}
		else if (strName == _T("sys-max-btn"))
		{
			SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE,0);
		}
		else if (strName == _T("sys-restore-btn"))
		{
			SendMessage(WM_SYSCOMMAND,SC_RESTORE,0);
		}
		else if (strName == _T("sys-close-btn"))
		{
			SendMessage(WM_SYSCOMMAND,SC_CLOSE,0);
		}
		else if (strName == _T("prev-btn"))
		{
			--m_nCurrStep;
			if (m_nCurrStep == 1) // 将要显示第一个页面
			{
				m_pPrevBtn->SetVisible(false);
			}
			m_stepsVec[m_nCurrStep]->SetTextColor(0xFFC1CCD2);
			m_contentsVec[m_nCurrStep]->SetVisible(false);
			m_stepsVec[m_nCurrStep - 1]->SetTextColor(0xFF5483A1);
			m_contentsVec[m_nCurrStep - 1]->SetVisible(true);
		}
		else if (strName == _T("next-btn"))
		{
			if (m_nCurrStep == 1)
			{
				m_pPrevBtn->SetVisible(true);
			}
			++m_nCurrStep;
			if (m_nCurrStep == m_stepsVec.size())
			{
				m_pNextBtn->SetVisible(false);
				m_pOkBtn->SetVisible(true);
			}
			m_stepsVec[m_nCurrStep -2]->SetTextColor(0xFFC1CCD2);
			m_contentsVec[m_nCurrStep - 2]->SetVisible(false);
			m_stepsVec[m_nCurrStep - 1]->SetTextColor(0xFF5483A1);
			m_contentsVec[m_nCurrStep - 1]->SetVisible(true);			
		}
		else if (strName == _T("cancel-btn"))
		{
			::SendMessage(m_hWndOwner,WM_ADDTASK_NOTIFY,0,reinterpret_cast<LPARAM>(this));
			Close(IDCANCEL);
		}
		else if (strName == _T("ok-btn"))
		{
			/*todo*/
			::SendMessage(m_hWndOwner,WM_ADDTASK_NOTIFY,1,reinterpret_cast<LPARAM>(this));
			Close(IDOK);
		}
	}
}

LRESULT CNewTaskDlg::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

LRESULT CNewTaskDlg::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(m_hWnd, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	styleValue &= ~WS_THICKFRAME;
	::SetWindowLong(m_hWnd, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	HMENU hMenu = GetSystemMenu(m_hWnd,FALSE);
	if (hMenu != NULL)
	{
		DeleteMenu(hMenu,SC_MAXIMIZE,MF_BYCOMMAND);
	}

	//根据MainPanel.xml创建程序界面
	m_PaintManager.Init(m_hWnd);

	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("new-task-panel.xml"), (UINT)0,  NULL, &m_PaintManager);
	ASSERT(pRoot && "Failed to parse XML");

	m_PaintManager.AttachDialog(pRoot);
	m_PaintManager.AddNotifier(this);

	SetIcon(IDI_SHELLICON);

	ShowWindow(true);
	return 0;
}

LRESULT CNewTaskDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CNewTaskDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CNewTaskDlg::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(m_hWnd) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CNewTaskDlg::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CNewTaskDlg::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CNewTaskDlg::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

		if( IsNotNULL(pControl) 
			&& IsNotZero(_tcscmp(pControl->GetClass(), _T("ButtonUI")))
			&& IsNotZero(_tcscmp(pControl->GetClass(), _T("OptionUI"))))
		{
			return HTCAPTION;
		}
	}

	return HTCLIENT;
}

LRESULT CNewTaskDlg::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CNewTaskDlg::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CNewTaskDlg::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if( wParam == SC_CLOSE ) 
	{
		PostMessage (WM_CLOSE);
		bHandled = TRUE;
		return 0;
	}

	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if( ::IsZoomed(m_hWnd) != bZoomed ) 
	{
		if( !bZoomed ) 
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("sys-max-btn")));
			if (pControl)
			{
				pControl->SetVisible(false);
			}

			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("sys-restore-btn")));
			if (pControl)
			{
				pControl->SetVisible(true);
			}
		}
		else
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("sys-max-btn")));
			if (pControl) 
			{
				pControl->SetVisible(true);
			}

			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("sys-restore-btn")));
			if (pControl) 
			{
				pControl->SetVisible(false);
			}
		}
	}
	return lRes;
}
