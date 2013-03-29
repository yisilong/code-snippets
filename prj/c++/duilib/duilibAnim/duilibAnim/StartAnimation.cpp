#include "StartAnimation.h"
#include <CommCtrl.h>
#include <fstream>
#include <process.h>

CStartAnimation::CStartAnimation():
m_iInterval(5),
m_hWnd(NULL),
m_hWndDest(NULL),
m_iTimer(0),
m_uStepCount(10),
m_eStyle(STYLE_START_NORMAL),
m_hMemDc(NULL),	
m_hMemBmp(NULL)
{
	ZeroMemory(&m_rcDest,sizeof(m_rcDest));
	ZeroMemory(&m_rcDraw,sizeof(m_rcDraw));
}

CStartAnimation::~CStartAnimation(void)
{
	Release();
}

bool CStartAnimation::Create(HWND hWndDest,UINT uDuration,bool bShowDest /* = true */,AnimStyle eStyle /* = STYLE_START_NORMAL */)
{
	if (!IsWindow(hWndDest))
	{
		return false;
	}
	if (!Register(hWndDest))
	{
		return false;
	}

	m_hWndDest = hWndDest;
	m_eStyle = eStyle;
	m_bShowDest = bShowDest;
	m_uStepCount = (int)((float)uDuration / m_iInterval + 0.5f);
	HANDLE hTread = reinterpret_cast<HANDLE>(_beginthreadex(NULL,0,MyThread,this,0,NULL));
	CloseHandle(hTread);
	return true;
}

bool CStartAnimation::Register(HWND hWndDest)
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
	wndcls.lpszClassName = _T("startAnimation");
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

LRESULT CALLBACK CStartAnimation::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
	case WM_CREATE:
		{
			LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
			CStartAnimation* pThis = static_cast<CStartAnimation*>(lpcs->lpCreateParams);
			::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(pThis));
			pThis->OnCreate(hWnd,message,wParam,lParam);
		}break;
	case WM_TIMER:
		{
			CStartAnimation* pThis = reinterpret_cast<CStartAnimation*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
			pThis->OnTimer(hWnd,message,wParam,lParam);
		}break;

	case WM_DESTROY:
		{
			CStartAnimation* pThis = reinterpret_cast<CStartAnimation*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
			pThis->Release();
			PostQuitMessage(0);
		}
		break; 	case WM_PAINT:	//only once enter
 		{
			CStartAnimation* pThis = reinterpret_cast<CStartAnimation*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
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

bool CStartAnimation::Release()
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
	m_uStepCount = 10;
	m_eStyle = STYLE_START_NORMAL;

	ZeroMemory(&m_rcDest,sizeof(m_rcDest));
	ZeroMemory(&m_rcDraw,sizeof(m_rcDraw));
	return true;
}

LRESULT CStartAnimation::OnTimer( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
// 	if (wParam == 9527)
// 	{
// 		KillTimer(hWnd,9527);
// 		PostMessage(hWnd,WM_CLOSE,0,0);
// 		return 0;
// 	}
	RECT &rcWindow = m_rcDraw;

	BOOL bAniming = TRUE; //还在做动画?
	//调整窗口大小
	if (m_eStyle == STYLE_START_NORMAL)
	{
		bAniming = AdjustWinodwRect_Normal(rcWindow);
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
	SetStretchBltMode(hMemDc,COLORONCOLOR);
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
	DeleteDC(hMemDc);
	ReleaseDC(hWnd,hdc);
/***************************************************************************/

	return 0;
}

LRESULT CStartAnimation::OnPaint( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hWnd,&ps);	
	RECT rcWindow = m_rcDraw;
	long cx = rcWindow.right - rcWindow.left;
	long cy = rcWindow.bottom - rcWindow.top;
	if (m_eStyle == STYLE_START_NORMAL)
	{
		rcWindow.left = 0;
		rcWindow.top = 0;
	}
	SetStretchBltMode(hdc,COLORONCOLOR);
	StretchBlt(hdc,0,0,cx,cy,m_hMemDc,0,0,cx,cx,SRCCOPY);
	EndPaint(hWnd,&ps);

	return 0;
}

BOOL CStartAnimation::AdjustWinodwRect_Normal( RECT& rcWindow )
{
	long cx = rcWindow.right - rcWindow.left;
	long cy = rcWindow.bottom - rcWindow.top;
	int nStepX = (int)((m_rcDest.right - m_rcDest.left)/(float)m_uStepCount);
	int nStepY = (int)((m_rcDest.bottom - m_rcDest.top)/(float)m_uStepCount);
	long cxDest = m_rcDest.right - m_rcDest.left;
	long cyDest = m_rcDest.bottom - m_rcDest.top;
	if (cx < cxDest && cy < cyDest)
	{
		rcWindow.left -= nStepX;
		rcWindow.right += nStepX;
		rcWindow.top -= nStepY;
		rcWindow.bottom += nStepY;
		if (rcWindow.left < m_rcDest.left || rcWindow.right > m_rcDest.right || rcWindow.top < m_rcDest.top || rcWindow.bottom > m_rcDest.bottom)
		{
			rcWindow = m_rcDest;
		}
#ifdef _DEBUG
		char ch[1024]={0};
		sprintf_s(ch,1024,"**********************************%d,%d,%d,%d\n",rcWindow.left,rcWindow.top,rcWindow.right-rcWindow.left,rcWindow.bottom-rcWindow.top);
		OutputDebugStringA(ch);
#endif

	}
	else
	{
		KillTimer(m_hWnd,m_iTimer);
 //		SetWindowPos(m_hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_NOREDRAW);
		EnableWindow(m_hWnd,FALSE);
// 		ShowWindow(m_hWnd,SW_HIDE);
		if (m_bShowDest)
		{
			ShowWindow(m_hWndDest,SW_SHOW);
			SetForegroundWindow(m_hWndDest);
			//SetWindowPos(m_hWndDest,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);
		}
		PostMessage(m_hWnd,WM_CLOSE,0,0);
		//SetTimer(m_hWnd,9527,20,NULL);

		//SetWindowPos(m_hWndDest,HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
		return FALSE;
	}
	return TRUE;
}

UINT CStartAnimation::MyThread(LPVOID lpParam)
{
	CStartAnimation* pThis = static_cast<CStartAnimation*>(lpParam);
	return pThis->ThreadRun();
}
UINT CStartAnimation::ThreadRun()
{
	RECT rcDest;
	GetWindowRect(m_hWndDest,&rcDest);
	m_rcDest = rcDest;

	m_rcDraw.left =  (LONG)((rcDest.right - rcDest.left)/2.0f + rcDest.left + 0.5f);
	m_rcDraw.top =  (LONG)((rcDest.bottom - rcDest.top)/2.0f + rcDest.top + 0.5f);
	m_rcDraw.right = m_rcDraw.left+1;
	m_rcDraw.bottom = m_rcDraw.top+1;

	HINSTANCE hInst = (HINSTANCE)GetWindowLong(m_hWndDest,GWL_HINSTANCE);
	m_hWnd = CreateWindowEx(WS_EX_TOOLWINDOW,_T("startAnimation"),NULL,WS_POPUP,/*rcDest.left,rcDest.top,rcDest.right-rcDest.left,rcDest.bottom-rcDest.top*/0,0,1,1,NULL,NULL,hInst,this);
	if (m_hWnd == NULL)
	{
		return false;
	}
	ShowWindow(m_hWnd,SW_SHOW);	
	UpdateWindow(m_hWnd);

	DWORD dwNewLong = GetWindowLong(m_hWnd, GWL_EXSTYLE); 
	dwNewLong |= WS_EX_TRANSPARENT | WS_EX_LAYERED; 
	SetWindowLong(m_hWnd, GWL_EXSTYLE, dwNewLong);
	MSG msg = {0};
	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CStartAnimation::OnCreate( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	DWORD dwStyle = GetWindowLong(m_hWndDest,GWL_EXSTYLE);
	dwStyle |=WS_EX_TOOLWINDOW;
	SetWindowLong(m_hWndDest,GWL_EXSTYLE,dwStyle);
	RECT rcDesktop;
	GetClientRect(GetDesktopWindow(),&rcDesktop);
	SetWindowPos(m_hWndDest,HWND_TOP,rcDesktop.right-1,0,0,0,SWP_NOZORDER | SWP_NOSIZE);
	ShowWindow(m_hWndDest,SW_SHOW);
	UpdateWindow(m_hWndDest);
	ShowWindow(m_hWndDest,SW_HIDE);
	dwStyle &= ~WS_EX_TOOLWINDOW;
	SetWindowLong(m_hWndDest,GWL_EXSTYLE,dwStyle);
	SetWindowPos(m_hWndDest,HWND_TOP,m_rcDest.left,m_rcDest.top,m_rcDest.right-m_rcDest.left,m_rcDest.bottom-m_rcDest.top,SWP_NOZORDER);;
	//get dc of dest window with sendmessage
	HDC hDcDest = GetDC(m_hWndDest);
	HDC hMemDestDc = CreateCompatibleDC(hDcDest);
	HBITMAP hBmpDest = CreateCompatibleBitmap(hDcDest,m_rcDest.right - m_rcDest.left,m_rcDest.bottom-m_rcDest.top);
	SelectObject(hMemDestDc,hBmpDest);
	SendMessage(m_hWndDest,WM_PRINTCLIENT,(WPARAM)hMemDestDc,PRF_CHECKVISIBLE | PRF_CLIENT | PRF_ERASEBKGND | PRF_CHILDREN);

	//copy it to compatible dc of this window
	HDC hDc = GetDC(hWnd);
	m_hMemDc = CreateCompatibleDC(hDc);
	m_hMemBmp = CreateCompatibleBitmap(hDc,m_rcDest.right - m_rcDest.left,m_rcDest.bottom-m_rcDest.top);
	SelectObject(m_hMemDc,m_hMemBmp);
	BitBlt(m_hMemDc,0,0,m_rcDest.right - m_rcDest.left,m_rcDest.bottom-m_rcDest.top,hMemDestDc,0,0,SRCCOPY);
	ReleaseDC(m_hWndDest,hDcDest);
	ReleaseDC(hWnd,hDc);
	DeleteDC(hMemDestDc);
	DeleteObject(hBmpDest);

	//	SetWindowPos(m_hWnd,HWND_TOP,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);

	SetForegroundWindow(hWnd);
	if (m_eStyle == STYLE_START_NORMAL)
	{
		SetWindowPos(hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
	}
	m_iTimer = SetTimer(hWnd,1022,m_iInterval,NULL);
	return 0;
}
