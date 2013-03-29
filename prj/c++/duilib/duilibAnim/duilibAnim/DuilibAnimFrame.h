#pragma once

#include "Stdafx.h"
#include "CloseAnimation.h"
#include "StartAnimation.h"
#include "TrayIcon.h"
class CDuilibAnimFrame:public CWindowWnd, public INotifyUI
{
public:
	CDuilibAnimFrame();
	~CDuilibAnimFrame();

	inline LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };
	inline UINT GetClassStyle() const { return CS_DBLCLKS; };
	inline void OnFinalMessage(HWND /*hWnd*/) { delete this; };

	void Init();
	void Notify(TNotifyUI& msg);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	CPaintManagerUI m_PaintManager;
	CCloseAnimation m_closeAnim;
	CStartAnimation m_startAnim;
	CTrayIcon m_tray;
};
