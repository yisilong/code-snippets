#if !defined(AFX_MYLISTBOX_H__5529A6B1_584A_11D2_A41A_006097BD277B__INCLUDED_)
#define AFX_MYLISTBOX_H__5529A6B1_584A_11D2_A41A_006097BD277B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "..//FICS.h"
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
	int AddString(LPCTSTR lpszItem, CString strPathIcon);
protected:	   
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyListBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
// Implementation
protected:
	//{{AFX_MSG(CMyListBox)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSelchange();
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM wParam ,LPARAM  lParam );
	DECLARE_MESSAGE_MAP()
protected:
	typedef struct ItemData   //item附加数据
	{
		char chImgPath[30];
	}ItemData,*PItemData;	
protected:
	int  m_nPreHoverIndex;          //上一次鼠标经过的Item

protected:
	COLORREF m_clrDlgBorder;
	COLORREF m_clrItemBorderNormal;
	COLORREF m_clrItemBorderHover;
	COLORREF m_clrItemBorderSelected;

	Color    m_clrItemTopStartNormal;
	Color    m_clrItemBottomEndNormal;
	Color    m_clrItemTopStartHover;
	Color    m_clrItemBottomEndHover;
	Color    m_clrItemTopStartSelected;
	Color    m_clrItemBottomEndSelected;

	COLORREF m_clrText;
    COLORREF m_clrSelText;

	CBrush   m_dlgBrush;
	COLORREF m_clrDlgBackGround;

	bool bScrollEnd;
	bool bWhellEnd;

	CDC m_memDC;
};//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLISTBOX_H__5529A6B1_584A_11D2_A41A_006097BD277B__INCLUDED_)
