#pragma once
#include <Windows.h>
#include <tchar.h>
class CCloseAnimation
{
public:
	enum AnimStyle {STYLE_CLOSE_NORMAL,STYLE_CLOSE_ICON};
private:
	HWND m_hWnd;
	HWND m_hWndDest;
	RECT m_rcDest;
	HBITMAP m_hMemBmp;
	HDC m_hMemDc;
	UINT m_iTimer;
	bool m_bCloseDest;
	RECT m_rcDraw;
	RECT m_rcShellIcon;
	UINT m_uStepCount;
	const int m_iInterval;
	int m_iCmdShow;
	AnimStyle m_eStyle;
public:
	CCloseAnimation();
	~CCloseAnimation();
	bool Create(HWND hWndDest,UINT uDuration,bool bCloseDest,int iCmdShow = SW_HIDE,AnimStyle eStyle = STYLE_CLOSE_NORMAL); //bCloseDest 为true,忽略iCmdShow
	bool Release();
private:	
	bool GetShellIconPos(RECT &rect,HWND hDestWnd);
	HWND FindTrayWnd();
	HWND FindNotifyIconOverflowWindow();
	bool EnumNotifyWindow(RECT& rect,HWND hWnd);
	BOOL IsWow64();

	//返回TRUE继续动画,返回FALSE关闭
	BOOL AdjustWinodwRect_Normal(RECT& rcWindow);
	BOOL AdJustWindowRect_Icon(RECT& rcWindow);

	bool Register(HWND hWndDest);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT OnTimer( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	LRESULT OnPaint( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	LRESULT OnCreate( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	
	BOOL EnableDebugPrivilege();

	static UINT WINAPI MyThread(LPVOID lpParam);
	UINT ThreadRun();
};
