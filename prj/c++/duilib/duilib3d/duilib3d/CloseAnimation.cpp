#include "CloseAnimation.h"
#include <CommCtrl.h>
#include <fstream>
CCloseAnimation::CCloseAnimation():
m_iInterval(10),
m_hWnd(NULL),
m_hWndDest(NULL),
m_iTimer(-1),
m_bCloseDest(false),
m_uStepCount(10),
m_eStyle(STYLE_CLOSE_NORMAL),
m_hMemDc(NULL),	
m_hMemBmp(NULL)
{
	ZeroMemory(&m_rcDest,sizeof(m_rcDest));
	ZeroMemory(&m_rcDraw,sizeof(m_rcDraw));
	ZeroMemory(&m_rcShellIcon,sizeof(m_rcShellIcon));
}

CCloseAnimation::~CCloseAnimation(void)
{
	Release();
}

bool CCloseAnimation::Create(HWND hWndDest,UINT uDuration,bool bCloseDest,int iCmdShow/* = SW_HIDE*/,AnimStyle eStyle /*= STYLE_CLOSE_NORMAL*/)
{
	if (!IsWindow(hWndDest))
	{
		return false;
	}
	if (IsIconic(hWndDest) != 0) //目标窗口最小化
	{
		ShowWindow(hWndDest,SW_HIDE);
		if (bCloseDest)
		{
			PostMessage(hWndDest,WM_CLOSE,0,0);
		}
		return false;
	}
	if(!Register(hWndDest))   //窗口注册失败
	{
		return false;
	}
	//create window and set pos as hWndDest
	m_hWndDest = hWndDest;
	m_bCloseDest = bCloseDest;
	m_eStyle = eStyle;
	RECT rcDest;
	GetWindowRect(hWndDest,&rcDest);
	m_rcDest = rcDest;
	m_rcDraw = rcDest;

	m_hWnd = CreateWindowEx(WS_EX_TOOLWINDOW,_T("closeAnimation"),NULL,WS_POPUP,rcDest.left,rcDest.top,rcDest.right-rcDest.left,rcDest.bottom-rcDest.top,NULL,NULL,(HINSTANCE)GetWindowLong(hWndDest,GWL_HINSTANCE),this);
	if (m_hWnd == NULL)
	{
		return false;
	}

	//get dc of dest window with sendmassage
	HDC hDcDest = GetDC(hWndDest);
	HDC hMemDestDc = CreateCompatibleDC(hDcDest);
	HBITMAP hBmpDest = CreateCompatibleBitmap(hDcDest,m_rcDest.right - m_rcDest.left,m_rcDest.bottom-m_rcDest.top);
	SelectObject(hMemDestDc,hBmpDest);
	SendMessage(hWndDest,WM_PRINTCLIENT,(WPARAM)hMemDestDc,PRF_CHECKVISIBLE | PRF_CLIENT | PRF_ERASEBKGND | PRF_CHILDREN);

	//copy it to compatible dc of this window
	HDC hDc = GetDC(m_hWnd);
	m_hMemDc = CreateCompatibleDC(hDc);
	m_hMemBmp = CreateCompatibleBitmap(hDc,m_rcDest.right - m_rcDest.left,m_rcDest.bottom-m_rcDest.top);
	SelectObject(m_hMemDc,m_hMemBmp);
	BitBlt(m_hMemDc,0,0,m_rcDest.right - m_rcDest.left,m_rcDest.bottom-m_rcDest.top,hMemDestDc,0,0,SRCCOPY);
	ReleaseDC(hWndDest,hDcDest);
	ReleaseDC(m_hWnd,hDc);
	DeleteDC(hMemDestDc);
	DeleteObject(hBmpDest);

//	SetWindowPos(m_hWnd,HWND_TOP,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
	if (!m_bCloseDest)
	{
		ShowWindow(hWndDest,iCmdShow);
	}
	else
	{
		ShowWindow(hWndDest,SW_HIDE);
	}
	SetForegroundWindow(m_hWnd);
	if (m_eStyle == STYLE_CLOSE_ICON)
	{
		SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
	}
	else
	{
		SetWindowPos(m_hWnd,HWND_TOP,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
	}
	ShowWindow(m_hWnd,SW_SHOW);	
	UpdateWindow(m_hWnd);	
	DWORD dwNewLong = GetWindowLong(m_hWnd, GWL_EXSTYLE); 
	dwNewLong |= WS_EX_TRANSPARENT | WS_EX_LAYERED; 
	SetWindowLong(m_hWnd, GWL_EXSTYLE, dwNewLong);

	m_uStepCount = (int)((float)uDuration / m_iInterval + 0.5f);
	
	if (m_eStyle == STYLE_CLOSE_ICON)
	{	
		GetShellIconPos(m_rcShellIcon,m_hWndDest);
	}
	return true;
}

bool CCloseAnimation::Register(HWND hWndDest)
{
	HINSTANCE hIst = (HINSTANCE)GetWindowLong(hWndDest,GWL_HINSTANCE);
	WNDCLASS wndcls;
	wndcls.cbClsExtra = 0;
	wndcls.cbWndExtra = 0;
	wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wndcls.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndcls.hIcon = NULL;
	wndcls.hInstance = hIst;
	wndcls.lpfnWndProc = WndProc;
	wndcls.lpszClassName = _T("closeAnimation");
	wndcls.lpszMenuName = NULL;
	wndcls.style = CS_HREDRAW | CS_VREDRAW;

	if (RegisterClass(&wndcls) == 0)
	{
		if (GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
		{
			return false;
		}		
	}	
	return true;
}

LRESULT CALLBACK CCloseAnimation::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
	case WM_CREATE:
		{
			LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
			CCloseAnimation* pThis = static_cast<CCloseAnimation*>(lpcs->lpCreateParams);
			pThis->m_iTimer = SetTimer(hWnd,1022,pThis->m_iInterval,NULL);
			::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(pThis));
		}break;
	case WM_TIMER:
		{
			CCloseAnimation* pThis = reinterpret_cast<CCloseAnimation*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
			pThis->OnTimer(hWnd,message,wParam,lParam);
		}break;
	case WM_DESTROY:
		{
			CCloseAnimation*pThis = reinterpret_cast<CCloseAnimation*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
			pThis->Release();
		}
		break;
 	case WM_PAINT:	//only once enter
 		{
			CCloseAnimation* pThis = reinterpret_cast<CCloseAnimation*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
			pThis->OnPaint(hWnd,message,wParam,lParam);
 		}break;
	case WM_ERASEBKGND:
		{
		}break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

bool CCloseAnimation::Release()
{
	if (m_hMemBmp != NULL)
	{
		DeleteObject(m_hMemBmp);
		m_hMemBmp = NULL;
	}
	if (m_hMemDc != NULL)
	{
		DeleteDC(m_hMemDc);
		m_hMemDc = NULL;
	}
	m_hWnd = NULL;
	m_hWndDest = NULL;
	m_iTimer = -1;
	m_bCloseDest = false;
	m_uStepCount = 10;
	m_eStyle = STYLE_CLOSE_NORMAL;

	ZeroMemory(&m_rcDest,sizeof(m_rcDest));
	ZeroMemory(&m_rcDraw,sizeof(m_rcDraw));
	ZeroMemory(&m_rcShellIcon,sizeof(m_rcShellIcon));
	return true;
}

bool CCloseAnimation::GetShellIconPos(RECT &rect,HWND hDestWnd)
{
	HWND hWnd = NULL;  
	bool bGetIt = false;
	hWnd = FindTrayWnd();
	if (hWnd != NULL)  
	{
		bGetIt = EnumNotifyWindow(rect,hWnd);
		if (!bGetIt)//如果没在普通托盘区  
		{
			hWnd = FindNotifyIconOverflowWindow();//在溢出区（win7）  
			if (hWnd != NULL)  
			{  
				bGetIt = EnumNotifyWindow(rect,hWnd);  
			}  
		} 		
		
 
	}  

	return bGetIt; 
}

HWND CCloseAnimation::FindTrayWnd()
{
	HWND hWnd = NULL;  
	HWND hWndPaper = NULL;  

	if ((hWnd = FindWindow(_T("Shell_TrayWnd"), NULL)) != NULL)  
	{  
		if ((hWnd = FindWindowEx(hWnd, 0, _T("TrayNotifyWnd"), NULL)) != NULL)  
		{  
			hWndPaper = FindWindowEx(hWnd, 0, _T("SysPager"), NULL);  
			if(!hWndPaper)  
				hWnd = FindWindowEx(hWnd, 0, _T("ToolbarWindow32"), NULL);  
			else  
				hWnd = FindWindowEx(hWndPaper, 0, _T("ToolbarWindow32"), NULL);  
		}  
	}  

	return hWnd; 
}

HWND CCloseAnimation::FindNotifyIconOverflowWindow()
{
	HWND hWnd = NULL;  

	hWnd = FindWindow(_T("NotifyIconOverflowWindow"), NULL);  
	if (hWnd != NULL)  
	{  
		hWnd = FindWindowEx(hWnd, NULL, _T("ToolbarWindow32"), NULL);  
	}  

	return hWnd;
}

bool CCloseAnimation::EnumNotifyWindow(RECT& rect, HWND hWnd )
{	
	if (hWnd == NULL)
		return false;

	DWORD lProcessID = 0;  
	long ret = GetWindowThreadProcessId(hWnd, &lProcessID);
	if (ret == 0 || lProcessID == 0)
		return false;

	if (!EnableDebugPrivilege())
	{
		return false;
	}
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS|PROCESS_VM_OPERATION|PROCESS_VM_READ|PROCESS_VM_WRITE,FALSE,lProcessID);
	if (hProcess == NULL)  
	{
		return false;
	}

	LPVOID lpAddress = VirtualAllocEx(hProcess,0, 0x4096, MEM_COMMIT, PAGE_READWRITE);  
	LPVOID lpRect = VirtualAllocEx(hProcess,0,sizeof(RECT), MEM_COMMIT, PAGE_READWRITE);  
	long lButCount = SendMessage(hWnd, TB_BUTTONCOUNT, 0, 0); //发送消息获取托盘button数量  
	bool bSuc = false; 

	if (lpAddress != NULL  && lpRect != NULL)  
	{ 
		int nDataOffset = sizeof(TBBUTTON) - sizeof(INT_PTR) - sizeof(DWORD_PTR);
		int nStrOffset = 18; 
		if ( IsWow64() ){
			nDataOffset+=4;
			nStrOffset+=6;
		}		
		long lHwndAdr = 0,lHwnd = 0;
		//不知道为什么都需要显示一次
// 		for(int iIndex=0 ;iIndex< lButCount;iIndex++)  
// 		{  
// 			LPARAM lParam  = MAKELPARAM(0,FALSE);
// 			SendMessage(hWnd,TB_HIDEBUTTON,(WPARAM)iIndex,lParam);
// 		}
		for(int iIndex=0 ;iIndex< lButCount;iIndex++)  
		{
			RECT rc = {0};   
			ret = SendMessage(hWnd,TB_GETBUTTON,iIndex,(long)lpAddress);//发送消息获取托盘项数据起始地址  
			ret = ReadProcessMemory(hProcess, LPVOID((long)lpAddress + nDataOffset),&lHwndAdr,4,0);  
			if(ret != 0 && lHwndAdr != -1)  
			{  
				ret = ReadProcessMemory(hProcess, (LPVOID)lHwndAdr,&lHwnd, 4,0);//获取句柄  
				if(ret != 0 && (HWND)lHwnd == m_hWndDest)
				{  
// 					ret = SendMessage(hWnd,TB_ISBUTTONHIDDEN,(WPARAM)iIndex,0);
// 					if (ret != 0) //hidden
// 					{
// 						ret = SendMessage(hWnd,TB_GETSTATE,(WPARAM)iIndex,0);
// 						if (ret != -1) //取回状态
// 						{
// 							LPARAM lParam  = MAKELPARAM(0,ret);
// 							ret = SendMessage(hWnd,TB_SETSTATE,(WPARAM)iIndex,lParam);
// 						}
// 					}
					ret = SendMessage(hWnd,TB_GETITEMRECT,(WPARAM)iIndex,(LPARAM)lpRect); //发送消息获取托盘项区域数据
					ret = ReadProcessMemory(hProcess,lpRect,&rc, sizeof(rc),0);  //读取托盘区域数据  
					if(ret != 0)  
					{  
						RECT rcBar;
						GetWindowRect(hWnd,&rcBar);
						OffsetRect(&rc,rcBar.left,rcBar.top);
						rect = rc;
					}
					bSuc = true;//在普通托盘区找到，在溢出区不再查找  
					break; 
				}  
			}  
		}   
	}  
	if (lpAddress != NULL)  
	{  
		VirtualFreeEx( hProcess, lpAddress, 0x4096, MEM_DECOMMIT);  
		VirtualFreeEx( hProcess, lpAddress, 0, MEM_RELEASE);  
	}  
	if (lpRect != NULL)
	{  
		VirtualFreeEx( hProcess, lpRect, sizeof(RECT), MEM_DECOMMIT);  
		VirtualFreeEx( hProcess, lpRect, 0, MEM_RELEASE);  
	}  
	CloseHandle(hProcess);

	return bSuc;  
}

// 兼容win7 64位

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

BOOL CCloseAnimation::IsWow64()
{
	BOOL bIsWow64 = FALSE;

	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
		GetModuleHandle(_T("kernel32")),"IsWow64Process");

	if (NULL != fnIsWow64Process)
	{
		if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
		{
			// handle error
		}
	}
	return bIsWow64;
}
LRESULT CCloseAnimation::OnTimer( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	RECT &rcWindow = m_rcDraw;

	BOOL bAniming = TRUE; //还在做动画?
	//调整窗口大小
	if (m_eStyle == STYLE_CLOSE_NORMAL)
	{
		bAniming = AdjustWinodwRect_Normal(rcWindow);
	}
	else
	{
		bAniming = AdJustWindowRect_Icon(rcWindow);
	}
	if (bAniming == FALSE)
	{
		return 0;
	}

	int cx = rcWindow.right-rcWindow.left;
	int cy = rcWindow.bottom-rcWindow.top;

/****************************************************************************/
	//创建兼容dc,将原始图片压缩,因为UpdateLayeredWindow没有压缩功能
	HDC hdc = GetDC(hWnd);
	HDC hMemDc = CreateCompatibleDC(hdc);
	HBITMAP hMemBmp = CreateCompatibleBitmap(hdc,cx,cy);
	SelectObject(hMemDc,hMemBmp);
	StretchBlt(hMemDc,0,0,cx,cy,m_hMemDc,0,0,m_rcDest.right-m_rcDest.left,m_rcDest.bottom-m_rcDest.top,SRCCOPY);
	POINT ptSrcPoint = {0L,0L};
	POINT ptDestPt = {rcWindow.left,rcWindow.top};			
	SIZE ptSrcSize = {cx,cy};

	//绘制并改变了窗口大小
	BLENDFUNCTION blendFunc32bpp;
	blendFunc32bpp.AlphaFormat = AC_SRC_ALPHA;
	blendFunc32bpp.BlendFlags = 0;
	blendFunc32bpp.BlendOp = AC_SRC_OVER;
	blendFunc32bpp.SourceConstantAlpha = 255;
	UpdateLayeredWindow(hWnd,hdc,&ptDestPt,&ptSrcSize,hMemDc,&ptSrcPoint,0,&blendFunc32bpp,ULW_ALPHA);

	//释放资源
	DeleteObject(hMemBmp);
	DeleteObject(hMemDc);
	ReleaseDC(hWnd,hdc);
/***************************************************************************/

	return 0;
}

LRESULT CCloseAnimation::OnPaint( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hWnd,&ps);	
	RECT rcWindow = m_rcDraw;
	long cx = rcWindow.right - rcWindow.left;
	long cy = rcWindow.bottom - rcWindow.top;
	if (m_eStyle == STYLE_CLOSE_ICON)
	{
		rcWindow.left = 0;
		rcWindow.top = 0;
	}
	StretchBlt(hdc,rcWindow.left,rcWindow.top,cx,cy,m_hMemDc,0,0,cx,cx,SRCCOPY);
	EndPaint(hWnd,&ps);
	return 0;
}

BOOL CCloseAnimation::AdjustWinodwRect_Normal( RECT& rcWindow )
{
	long cx = rcWindow.right - rcWindow.left;
	long cy = rcWindow.bottom - rcWindow.top;
	int nStepX = (int)((m_rcDest.right - m_rcDest.left)/(float)m_uStepCount);
	int nStepY = (int)((m_rcDest.bottom - m_rcDest.top)/(float)m_uStepCount);
	if (cy > nStepY*2)
	{
		rcWindow.top += (nStepY-1);
		rcWindow.bottom -= (nStepY-1);
	}
	else if (cy > 2)
	{
		rcWindow.top = m_rcDest.top + (m_rcDest.bottom - m_rcDest.top)/2;
		rcWindow.bottom = rcWindow.top + 2;
		RECT rc = m_rcDest;
		OffsetRect(&rc,-m_rcDest.left,-m_rcDest.top);
		FillRect(m_hMemDc,&rc,(HBRUSH)GetStockObject(WHITE_BRUSH));
	}
	else if (cx > nStepX*2)
	{
		rcWindow.left += (nStepX-1);
		rcWindow.right -= (nStepX-1);
	}
	else if (cx > 2)
	{
		rcWindow.left = m_rcDest.left + (m_rcDest.right - m_rcDest.left)/2;
		rcWindow.right = rcWindow.left + 2;
	}
	else
	{		
		KillTimer(m_hWnd,m_iTimer);
		PostMessage(m_hWnd,WM_CLOSE,0,0);
		if (m_bCloseDest)
		{
			PostMessage(m_hWndDest,WM_CLOSE,0,0);
		}
		return FALSE;
	}
	return TRUE;
}

BOOL CCloseAnimation::AdJustWindowRect_Icon( RECT& rcWindow )
{
	long cxShellIcon = m_rcShellIcon.right - m_rcShellIcon.left;
	long cyShellIcon = m_rcShellIcon.bottom - m_rcShellIcon.top;
	int nStepLeft = (int)((m_rcShellIcon.left - m_rcDest.left)/(float)m_uStepCount);
	int nStepTop = (int)((m_rcShellIcon.top - m_rcDest.top)/(float)m_uStepCount);
	int fStepInflateW = (int)(((m_rcDest.right-m_rcDest.left - cxShellIcon))/(float)m_uStepCount+0.1f);
	int fStepInflateH = (int)(((m_rcDest.bottom-m_rcDest.top - cyShellIcon))/(float)m_uStepCount+0.1f);
	
	long cx = rcWindow.right - rcWindow.left; //上一次宽度
	long cy = rcWindow.bottom - rcWindow.top; //上一次高度

	if (rcWindow.left + nStepLeft < m_rcShellIcon.left)
	{
		rcWindow.left += nStepLeft;
		rcWindow.top += nStepTop;
		rcWindow.right = rcWindow.left + cx - fStepInflateW;
		rcWindow.bottom = rcWindow.top + cy - fStepInflateH;
	}
	else if (rcWindow.left + (cxShellIcon-5) < m_rcShellIcon.left)
	{
		rcWindow = m_rcShellIcon;
		OffsetRect(&rcWindow,-(cxShellIcon-5),-(cyShellIcon-5));
	}
	else
	{
		KillTimer(m_hWnd,m_iTimer);
		PostMessage(m_hWnd,WM_CLOSE,0,0);
		if (m_bCloseDest)
		{
			PostMessage(m_hWndDest,WM_CLOSE,0,0);
		}
		return FALSE;
	}
	return TRUE;
}

BOOL CCloseAnimation::EnableDebugPrivilege()
{
	HANDLE   hToken;
	BOOL   fOk=FALSE;
	if(OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES,&hToken))
	{
		TOKEN_PRIVILEGES   tp;
		tp.PrivilegeCount=1;
		BOOL bResult = LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&tp.Privileges[0].Luid);
		if (!bResult)
		{
			OutputDebugString( _T("Can 't   lookup   privilege   value.\n "));
		}
		tp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
		bResult = AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(tp),NULL,NULL);
		if (!bResult)
		{
			OutputDebugString( _T("Can 't   adjust   privilege   value.\n "));
		}
		fOk=(GetLastError()==ERROR_SUCCESS);
		CloseHandle(hToken);
	}
	return   fOk;
} 