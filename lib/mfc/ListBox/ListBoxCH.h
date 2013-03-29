#if !defined(AFX_LISTBOXCH_H__C572274F_C595_11D6_8D12_004033579553__INCLUDED_)
#define AFX_LISTBOXCH_H__C572274F_C595_11D6_8D12_004033579553__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListBoxCH.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListBoxCH window
#define MAXITEMSTRING	256
#include <afxtempl.h>
class CExtItem
{
public:
	CExtItem(LPCSTR szItemName,HICON hIcon)
	{
		lstrcpy(m_szItemName,szItemName);
		m_hIcon = hIcon;
	}
public:
	TCHAR   m_szItemName[MAXITEMSTRING];
	HICON	m_hIcon;
};
class CListBoxCH : public CListBox
{
// Construction
public:
	CListBoxCH();

// Attributes
public:

// Operations
public:
	void AddItem(LPCSTR lpszItemName,HICON hIcon); 
	void SetSelColor(COLORREF clr);
	void SetBgColor(COLORREF clr);
	void SetCurSel(int curSel);
	void SetTextColor(COLORREF clr);
	void EnableEdge(BOOL bEnable);
	int  GetCurSel();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListBoxCH)
	public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual int VKeyToItem(UINT nKey, UINT nIndex);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListBoxCH();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListBoxCH)
	afx_msg void OnSelchange();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	CTypedPtrList<CPtrList,CExtItem *> m_pItemList;	// item list
	COLORREF			m_clrSel;	   // select color
	COLORREF			m_clrBg;	   // background color
	COLORREF			m_clrText;	   // text color
	BOOL				m_bEdge;	   // edge line
	int					m_curSel;	   // current select index base on zero
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTBOXCH_H__C572274F_C595_11D6_8D12_004033579553__INCLUDED_)
