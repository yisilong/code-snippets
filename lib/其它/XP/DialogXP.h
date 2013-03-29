///////////////////////////////////////////////////////////////////////////////
//
// DialogXP.h
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "tools.h"

#include "autofont.h"
///////////////////////////////////////////////////////////////////////////////
class CDialogXP : public CDialog
{
// Construction
public:
	CAutoFont fTitleBar;
	CDialogXP(UINT nIDTemplate, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlg)
	enum { IDD = 0 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDialogXP)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    public:
    virtual void PreSubclassWindow();
    //}}AFX_VIRTUAL

// Implementation
public:
	CSize GetFrameSize();
    virtual ~CDialogXP();

   // Generated message map functions
protected:
    //{{AFX_MSG(CDialogXP)
    afx_msg void OnNcPaint();
	afx_msg void OnNcLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nFlags, CPoint point);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

    DECLARE_MESSAGE_MAP()
private:
	CSize GetCaptionSize();
	BOOL MouseOnMaximize();
	BOOL MouseOnMinimize();
	BOOL MouseOnClose();
	void DrawCloseButton(CWindowDC& dc,CRect rc,UINT state);
	void DrawMinimizeButton(CWindowDC& dc,CRect rc,UINT state);
	void DrawMaximizeButton(CWindowDC& dc,CRect rc,UINT state);
	void DrawRestoreButton(CWindowDC& dc,CRect rc,UINT state);
	UINT PaintCaptionButtons(CSize szRect,CWindowDC& dc,UINT ms=0);
	void PaintCaption(BOOL bEntire=0);
	CSize GetCaptionButtonSize();
	CRect GetCaptionRect();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
