#include "CollectorFrame.h"
#include "../common/tools.h"
#include "NewTaskDlg.h"
#include "Message.h"

CCollectorFrame::CCollectorFrame(void)
{
	m_pListUI = NULL;
	m_pNewTaskDlg = NULL;
}

CCollectorFrame::~CCollectorFrame(void)
{
}
void CCollectorFrame::Init() 
{
	m_pListUI = static_cast<CListUI*>(m_PaintManager.FindControl(_T("filelist")));
	InitTasks();
	for (UINT i=0;i<m_tasksMap.size();i++)
	{
		CListTextElementUI* pListElement = new CListTextElementUI;
		pListElement->SetTag(i);
		m_pListUI->Add(pListElement);
	}
	m_pListUI->SetTextCallback(this);
}

void CCollectorFrame::Notify(TNotifyUI& msg)
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
			//SendMessage(WM_SYSCOMMAND,SC_CLOSE,0);
			ShowWindow(false);
		}
		else if (strName == _T("newTask"))
		{
			if (m_pNewTaskDlg == NULL)
			{
				m_pNewTaskDlg = new CNewTaskDlg(m_hWnd);
				m_pNewTaskDlg->Create(NULL,_T("新建任务"),WS_POPUP, 0L, 0, 0, 0, 0);
				m_pNewTaskDlg->CenterWindow();
			}
			SetForegroundWindow(m_pNewTaskDlg->GetHWND());
			m_pNewTaskDlg->ShowWindow(true);
		}
		else if (strName == _T("startTask"))
		{
		}
		else if (strName == _T("pauseTask"))
		{
		}
		else if (strName == _T("editTask"))
		{
		}
		else if (strName == _T("detailTask"))
		{
		}
		else if (strName == _T("delTask"))
		{
		}
	}
}

LRESULT CCollectorFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	case WM_ADDTASK_NOTIFY: lRes = OnAddTaskNotify(uMsg,wParam,lParam,bHandled);break;
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

LRESULT CCollectorFrame::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

	//根据MainPanel.xml创建程序界面
	m_PaintManager.Init(m_hWnd);

	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("main-panel.xml"), (UINT)0,  NULL, &m_PaintManager);
	ASSERT(pRoot && "Failed to parse XML");

	m_PaintManager.AttachDialog(pRoot);
	m_PaintManager.AddNotifier(this);

	SetIcon(IDI_SHELLICON);
	HICON hIcon = LoadIcon((HINSTANCE)GetWindowLong(m_hWnd,GWL_HINSTANCE),MAKEINTRESOURCE(IDI_SHELLICON));
	m_trayIcon.Create(m_hWnd,WM_ICON_NOTIFY,_T("kanbox便携版"),hIcon,IDR_SHELLMENU);

	return 0;
}

LRESULT CCollectorFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CCollectorFrame::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostQuitMessage(0L);

	bHandled = FALSE;
	return 0;
}

LRESULT CCollectorFrame::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(m_hWnd) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CCollectorFrame::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CCollectorFrame::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CCollectorFrame::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CCollectorFrame::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CCollectorFrame::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CCollectorFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CCollectorFrame::OnCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
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
						::ShowWindow(m_hWnd,SW_HIDE);
					}
					else
					{
						::ShowWindow(m_hWnd,SW_SHOWNORMAL);
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

LRESULT CCollectorFrame::OnAddTaskNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == 1)
	{
	}
	else
	{
		
	}
	m_pNewTaskDlg = NULL;
	return 0;
}

LPCTSTR CCollectorFrame::GetItemText( CControlUI* pList, int iItem, int iSubItem )
{
	UINT_PTR nTag = pList->GetTag();
	std::map<UINT,CTaskData>::iterator it = m_tasksMap.find(nTag);
	CDuiString strReturn;
	if (it != m_tasksMap.end())
	{
		switch(iSubItem)
		{
		case 0: //名字
			{
				strReturn = it->second.m_strName;
			}break;
		case 1: //状态
			{
				strReturn = it->second.m_strState;
			}break;
		case 2: //速度
			{
				strReturn = it->second.m_strSpeed;
			}break;
		case 3: //时间
			{
				strReturn = it->second.m_strTime;
			}break;
		default:break;
		}
	}
	return strReturn.GetData();
}

bool CCollectorFrame::InitTasks()
{
	using namespace std;
	using namespace rapidxml;
	tfstream inStream(_T("../bin/user/tasks.xml"),ios::in | ios::binary);
	inStream.seekg (0, ios::end);
	int length = inStream.tellg();
	inStream.seekg(0, ios::beg);
	TCHAR* pData = new TCHAR[length+1];
	inStream.read(pData,length);
	pData[length] = TCHAR('\0');
	inStream.close();
	CSafeDeleteArray<TCHAR> data(pData);
	xml_document<TCHAR> xml;
	try
	{
		xml.parse<0>(pData);
	}
	catch (...)
	{
		// 清空xml
		tfstream inStream(_T("../bin/user/tasks.xml"),ios::out | ios::binary | ios::trunc);
		inStream<<_T("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<tasks />")<<endl;
		inStream.close();
	}

	xml_node<TCHAR>* pRoot = xml.first_node(_T("tasks"));
	if (IsNULL(pRoot))
	{
		return false;
	}

	xml_node<TCHAR>* pNode = pRoot->first_node(_T("task"));
	CTaskData tmpData;
	UINT iTag = 0;
	xml_node<TCHAR>* pChildNode = NULL;
	while (IsNotNULL(pNode))
	{
		pChildNode = pNode->first_node();
		while (IsNotNULL(pChildNode))
		{
			tmpData.SetAttribute(pChildNode->name(),pChildNode->value());
			pChildNode = pChildNode->next_sibling();
		}
		pNode = pNode->next_sibling(_T("task"));
		m_tasksMap[iTag] = tmpData;
		++iTag;
	}

	return true;
}

CTaskData::CTaskData()
{

}

CTaskData::~CTaskData()
{

}

void CTaskData::SetAttribute( CDuiString strKey,CDuiString strValue )
{
	if (strKey == _T("name"))
	{
		m_strName = strValue;
	}
	else if (strKey == _T("state"))
	{
		m_strState = strValue;
	}
	else if (strKey == _T("speed"))
	{
		m_strSpeed = strValue;
	}
	else if (strKey == _T("time"))
	{
		m_strTime = strValue;
	}
}
