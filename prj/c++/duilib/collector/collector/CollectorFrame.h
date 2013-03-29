#pragma once
#include "StdAfx.h"
#include "TrayIcon.h"

class CTaskData
{
public:
	CDuiString m_strName; 	// 任务名称
	CDuiString m_strState;	// 任务状态
	CDuiString m_strSpeed;	// 任务速度
	CDuiString m_strTime; 	// 最近修改时间
public:
	void SetAttribute(CDuiString strKey,CDuiString strValue);
	CTaskData();
	~CTaskData();
};
class CNewTaskDlg;
class CCollectorFrame: public CWindowWnd, public INotifyUI,public IListCallbackUI
{
private:
	CPaintManagerUI m_PaintManager;
	CTrayIcon m_trayIcon;
	CListUI* m_pListUI;
	std::map<UINT,CTaskData> m_tasksMap;
	CNewTaskDlg *m_pNewTaskDlg;
public:
	CCollectorFrame();
	~CCollectorFrame();
	LPCTSTR GetWindowClassName() const { return _T("UICollectorFrame"); };
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
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnAddTaskNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LPCTSTR GetItemText(CControlUI* pList, int iItem, int iSubItem);

protected:
	void OnFinalMessage(HWND hWnd) { delete this; };
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	bool InitTasks();
};