#if !defined(AFX_MYLISTBOX_H__5529A6B1_584A_11D2_A41A_006097BD277B__INCLUDED_)
#define AFX_MYLISTBOX_H__5529A6B1_584A_11D2_A41A_006097BD277B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
/////////////////////////////////////////////////////////////////////////////		
//EditHelper.h : header file
#define	WM_APP_ED_EDIT_FINISHED	 ( WM_APP + 04101 )	
/////////////////////////////////////////////////////////////////////////////
class CEditHelper : public CEdit
{
	// Construction
public:
	CEditHelper();
	
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditHelper)
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CEditHelper();
	// Generated message map functions
protected:
	//{{AFX_MSG(CEditHelper)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG		
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//MyListBox.h : header file
class CMyListBox : public CListBox
{
	DECLARE_DYNAMIC(CMyListBox) 
// Construction
public:
	CMyListBox();
    virtual ~CMyListBox();
// Operations
public:
	int  AddString(LPCTSTR lpszItem, COLORREF clr = -1,HICON hIcon=NULL);	// Adds a colored string to the list box
	int  InsertString(int nIndex, LPCTSTR lpszItem,  COLORREF clr = -1,HICON hIcon=NULL);	// Inserts a colored string to the list box
	void SetItemColor(int nIndex, COLORREF clr);							// Sets the color of an item in the list box
	void EnableDragSort(BOOL bEnable) { m_bEnableDragSort = bEnable;}
	void EnableDropFiles(BOOL bEnable);

	void StartEdit();
	void EndEdit( BOOL bCommitText = TRUE );

	COLORREF SetDlgBackGround(COLORREF clr=-1);          //rgb=-1时,不改变颜色,返回当前颜色
	COLORREF SetItemBackGroundColor(COLORREF clr=-1);    //rgb=-1时,不改变颜色,返回当前颜色
	COLORREF SetItemSelBackGroundColor(COLORREF clr=-1); //rgb=-1时,不改变颜色,返回当前颜色
	COLORREF SetTextColor(COLORREF clr=-1);          //rgb=-1时,不改变颜色,返回当前颜色
	COLORREF SetSelTextColor(COLORREF clr=-1);       //rgb=-1时,不改变颜色,返回当前颜色
    HICON    SetItemIcon(int nIndex,HICON hIcon=NULL);   //hIcon=null时,不改变图标,返回当前图标
protected:	   
	void InsertDraggedItem();
    void DoTheScrolling(CPoint Point,CRect ClientRect);
    void DrawTheLines(int Index);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyListBox)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
// Implementation
protected:
	//{{AFX_MSG(CMyListBox)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM wParam ,LPARAM  lParam );
	afx_msg LRESULT OnEditFinished(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
protected:
protected:
	typedef struct ItemData   //item附加数据
	{
		HICON hIcon;
		COLORREF clr;
	}ItemData,*PItemData;

	CEditHelper		m_Edit;
	int				m_nItemBeingEdited;
	BOOL            m_bIsEditing;
    BOOL            m_bIsMsgBoxSend;	
protected:
	COLORREF m_clrItemBackGround ;
	COLORREF m_clrItemSelBackGround;
	COLORREF m_clrText;
    COLORREF m_clrSelText;
	COLORREF m_clrMouseMove;
	COLORREF m_clrDlgBackGround;
	CBrush   m_dlgBrush;
protected:
	BOOL     m_bEnableDragSort;
	BOOL     m_bEnableDropFiles;
protected:
	int      m_nPreIndex;          //上一次鼠标经过的Item
	COLORREF m_clrPreItem;         //上一次鼠标经过的Item的颜色	
protected:
	int   m_nMovingIndex;
	int   m_nMoveToIndex;
    BOOL  m_bCaptured;
    DWORD m_dwInterval;    //scroll speed when an item is dragged above or below the window
};//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLISTBOX_H__5529A6B1_584A_11D2_A41A_006097BD277B__INCLUDED_)
