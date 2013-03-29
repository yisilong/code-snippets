#if !defined(AFX_MACBUTTON_H__B9D2AA55_ABF4_11D3_90F4_0020AFBC499D__INCLUDED_)
#define AFX_MACBUTTON_H__B9D2AA55_ABF4_11D3_90F4_0020AFBC499D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MacButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMacButton window

class CMacButton : public CButton
{
// Construction
public:
	CMacButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMacButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL GetBold();
	void SetBold(BOOL bBold = TRUE);
	virtual ~CMacButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMacButton)
	afx_msg void OnSysColorChange();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bBold;
	void DrawButton(CDC *pDC, CRect rectItem, UINT nStyle, UINT nState);
	void GetColors();

	COLORREF m_crFace;
	COLORREF m_crShadow;
	COLORREF m_crHilight;
	COLORREF m_crDkShadow;
	COLORREF m_crLiteShadow;
	COLORREF m_crLiteFace;
	COLORREF m_crDarkDkShadow;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MACBUTTON_H__B9D2AA55_ABF4_11D3_90F4_0020AFBC499D__INCLUDED_)
