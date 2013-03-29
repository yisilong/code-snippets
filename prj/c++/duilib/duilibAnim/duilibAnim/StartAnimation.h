#pragma once
#include <Windows.h>
#include <tchar.h>
class CStartAnimation
{
public:
	enum AnimStyle {STYLE_START_NORMAL};
private:
	HWND m_hWnd;
	HWND m_hWndDest;
	RECT m_rcDest;
	HBITMAP m_hMemBmp;
	HDC m_hMemDc;
	UINT m_iTimer;
	RECT m_rcDraw;
	UINT m_uStepCount;     //需要触发多少次
	const int m_iInterval; //多少时间触发一次
	AnimStyle m_eStyle;
	bool m_bShowDest;
public:
	CStartAnimation();
	~CStartAnimation();
	bool Create(HWND hWndDest,UINT uDuration,bool bShowDest = true,AnimStyle eStyle = STYLE_START_NORMAL);
	bool Release();

	//返回TRUE继续动画,返回FALSE关闭
	BOOL AdjustWinodwRect_Normal(RECT& rcWindow);

	bool Register(HWND hWndDest);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT OnTimer( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	LRESULT OnPaint( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	LRESULT OnCreate( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );	
	BOOL EnableDebugPrivilege();

	static UINT WINAPI MyThread(LPVOID lpParam);
	UINT ThreadRun();
};
