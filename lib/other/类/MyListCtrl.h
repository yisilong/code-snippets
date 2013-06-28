//说明：
//1:此列的建立可以方便的进行列表的各种操作
//2:在数据应经存在的情况下不可以添加新的列
//3:当blnMenu设置为TRUE时可以显示排列菜单
//4:可以操作的数据类型为
//		A:CBo列表型(ctr_Edit_Cbo):AddCboItem和RemoveCboItem	
//      B:Edit编辑性(ctr_Edit_Text)
//      C:DateTime型(ctr_Edit_DateTime)
//      E:Time型(Ctr_Edit_Time)
//      F:Date型(Ctr_Edit_Date)
//      G:数字型(Ctr_Edit_Num):SetNumStyle和GetNumStyle
//		H:没有编辑框型(Ctr_Edit_No)
//5:使用过程中出现其他错误不予负责
//----------------------------------------------------------设计人:牛文平
#ifndef MYSORTLISTCTRL_H
#define MYSORTLISTCTRL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef SORTHEADERCTRL_H
	#include "MyHeaderCtrl.h"
#endif	
#ifdef _DEBUG
	#define ASSERT_VALID_STRING( str ) ASSERT( !IsBadStringPtr( str, 0xfffff ) )
#else	//	_DEBUG
	#define ASSERT_VALID_STRING( str ) ( (void)0 )
#endif	//	_DEBUG
//菜单资源号
#include "MyDateEdit.h"
#include "MyNumEdit.h"
#define ID_MENU_show1 WM_USER+11
#define ID_MENU_show2 WM_USER+11
#define ID_MENU_sep WM_USER+12
#define ID_MENU_icon WM_USER+13
#define ID_MENU_smallicon WM_USER+14
#define ID_MENU_list WM_USER+15
#define ID_MENU_report WM_USER+16
#define ID_MENU_No WM_USER+17
#define ID_MENU_Click WM_USER+18
#define ID_MENU_DblClick WM_USER+19
//编辑框资源号
#define ID_EditText WM_USER+1
#define ID_EditDateTime WM_USER+2
#define ID_EditCboDown WM_USER+3
#define ID_EditCboList WM_USER+4
#define ID_EditNum WM_USER+5
#define ID_EditPoP WM_USER+6

//列表编辑模式
#define ctr_Edit_No 0
#define ctr_Edit_Text 1
#define ctr_Edit_Date 2
#define ctr_Edit_DateTime 3
#define ctr_Edit_Time 4
#define ctr_Edit_CboDown 5
#define ctr_Edit_CboDownList 6
#define ctr_Edit_Num 7
//启动编辑模式
#define ctr_Edit_No 0
#define ctr_Edit_Click 1
#define ctr_Edit_DblClick 2
class CMyListCtrl : public CListCtrl
{
public:
	//构造函数
	CMyListCtrl();
	//删除一项
	BOOL DeleteItem( int iItem );
	//删除所有项
	BOOL DeleteAllItems();
	//设置子项字符
	BOOL SetItemText( int nItem, int nSubItem, LPCTSTR lpszText );
	//排列单列
	void Sort( int iColumn, BOOL bAscending );
	//设置单项数据
	BOOL SetItemData(int nItem, DWORD dwData);
	//返回单项数据
	DWORD GetItemData(int nItem) const;
	//添加一项
	//image:图像索引
	int AddItem(int image);
	//添加一列
	//nformat:LVCFMT_CENTER
	int InsertColumnX(CString strName,int nFormat,int iWidth,int intEdit);
	//设置排列方式方式(LVS_ICON,LVS_SMALLICON,LVS_LIST,LVS_REPORT)
	void SetMyStyle(DWORD dwNewStyle);
	//返回当前的排列方式(LVS_ICON,LVS_SMALLICON,LVS_LIST,LVS_REPORT)
	DWORD GetMyStyle();
	//注册列头
	BOOL SetHeader();
	//设置是否显示菜单
	void SetShowMenu(BOOL blnShow);
	//设置编辑框的位置
	void SetEditPos(BOOL blnShow,int itemX,int itemY,BOOL blnCbo);
	//设置列的编辑模式
	void SetColumnStyle(int item,int intEdit);
	//得到框的编辑模式
	int GetColumnStyle(int item);
	CStringArray *GetCboArrayData(int nitem);
	BOOL blnAllEdit;
	BOOL blnMenu;
	CEdit ctl_EditText;
	CMyDateEdit ctl_EditDateTime;
	CComboBox ctl_EditComboDown;
	CComboBox ctl_EditComboDownList;
	CMyNumEdit ctl_EditNum;
	BOOL m_bSortAscending;
	//0:NO 1:click 2:doubleclick
	int m_EditStyle;
	CStringArray strArrayEditStyle;
	CStringArray CboArrayIndex;
	CObArray CboArrayData;
	CStringArray NumArrayIndex;
	CObArray NumArrayData;
	CStringArray NumArrayAuto;
	CMyHeaderCtrl m_ctlHeader;
	int itemX;
	int itemY;
	void GetItemPos(CPoint point);
	BOOL Key_Shift(int& nItem,int& nSub);
	BOOL Key_Ctrl(int& nItem,int &nSub);
	//向Cbo中添加数据
	void AddCboItem(int nItem,CString strText);
	//在Cbo删除strText项
	void RemoveCboItem(int nitem,CString strText);
	//向Cbo中填充数据
	void FillCboData(int nitem);
	//intLeft:整数部分
	//intRight:小数部分	
	void SetNumEditStyle(int nitem,int intLeft,int intRight);
	void SetNumEditAuto(int nitem,BOOL blnAuto);
	//返回数字型的是否自动填充
	BOOL GetNumEditAuto(int nitem);
	//返回Num类型的范围
	//CPoint.X:整数部分
	//CPoint.y:小数部分
	CPoint *GetNumEditStyle(int nitem);
	//
	void SetItemFont(CFont ft);
	//{{AFX_VIRTUAL(CMyListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
public:
	virtual ~CMyListCtrl();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditListCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
	void FreeItemMemory( const int iItem );
	BOOL CMyListCtrl::SetTextArray( int iItem, LPTSTR* arrpsz );
	LPTSTR* CMyListCtrl::GetTextArray( int iItem ) const;
	int m_iNumColumns;
	int m_iSortColumn;
	//{{AFX_MSG(CMyListCtrl)
	afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnRButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnLButtonDblClk( UINT nFlags, CPoint point );
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnMenuitemlist();
	afx_msg void OnMenuitemicon();
	afx_msg void OnMenuitemsmallicon();
	afx_msg void OnMenuitemreport();
	afx_msg void OnMenuNo();
	afx_msg void OnMenuClick();
	afx_msg void OnMenuDblClick();
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // SORTLISTCTRL_H
