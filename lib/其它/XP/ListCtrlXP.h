///////////////////////////////////////////////////////////////////////////////
//
// ListCtrlXP.h
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "tools.h"

///////////////////////////////////////////////////////////////////////////////
class CListCtrlXP : public CListCtrl
{
// Construction
public:
    CListCtrlXP();

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CListCtrlXP)
    public:
    virtual void PreSubclassWindow();
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CListCtrlXP();

protected:
    CMouseMgr m_MouseMgr;

    // Generated message map functions
protected:
    //{{AFX_MSG(CListCtrlXP)
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseOut();
    afx_msg void OnNcPaint();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
