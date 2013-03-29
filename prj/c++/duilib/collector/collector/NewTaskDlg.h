#pragma once
#include "StdAfx.h"

class CNewTaskDlg: public CWindowWnd, public INotifyUI
{
private:
	CPaintManagerUI m_PaintManager;
	HWND m_hWndOwner;
	// 固定容器
	CHorizontalLayoutUI* m_pStepsHL;
	CHorizontalLayoutUI* m_pContentsHL;
	CHorizontalLayoutUI* m_pOperatesHL;
	// 固定按钮
	CButtonUI* m_pPrevBtn;
	CButtonUI* m_pNextBtn;
	CButtonUI* m_pOkBtn;
	CButtonUI* m_pCancelBtn;
	// 动态获取控件
	std::vector<CTextUI*> m_stepsVec;
	std::vector<CHorizontalLayoutUI*> m_contentsVec;
	std::vector<CTextUI*>::size_type m_nCurrStep;
	// 向导设置数据
	CDuiString m_strTaskName;
	CDuiString m_strListUrl;
	ULONG m_uListUrlStart;
	ULONG m_uListUrlEnd;
	
	CDuiString m_strPageUrl;
	ULONG m_uPageUrlStart;
	ULONG m_uPageUrlEnd;

	CDuiString m_strTorrentReg;
	CDuiString m_strMagnetReg;
	CDuiString m_strUrlReg;
	CDuiString m_strTextReg;

	CDuiString m_strFilter;
public:
	explicit CNewTaskDlg(HWND hWndOwner);
	~CNewTaskDlg();
	LPCTSTR GetWindowClassName() const { return _T("UINewTaskDlg"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };
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
protected:
	void OnFinalMessage(HWND hWnd) { delete this; };
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};