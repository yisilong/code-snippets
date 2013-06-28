#if !defined(AFX_SWINGPROGRESSCTRL_H__F2EAB2EE_E47B_43DD_9B35_96776A7E665F__INCLUDED_)
#define AFX_SWINGPROGRESSCTRL_H__F2EAB2EE_E47B_43DD_9B35_96776A7E665F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SwingProgressCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSwingProgressCtrl window
//	SwingControl Classes class, version 1.0
//
//	Copyright (c) 2000 Abhijit N Bhole (abhijit_bhole@hotmail.com.com)
//
// Feel free to modifiy and/or distribute this file, but
// do not remove this header.
//
// I would appreciate a notification of any bugs discovered or 
// improvements that could be made.
//
// This file is provided "as is" with no expressed or implied warranty.
//
//	History:
//		PMM	5/14/2000		Initial implementation.		

class  CSwingProgressCtrl : public CProgressCtrl
{
// Construction
public:
	CSwingProgressCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSwingProgressCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	CString m_strCaption;
	CString GetCaption();
	void SetCaption (CString pstr);
	virtual ~CSwingProgressCtrl();

	// Generated message map functions
protected:
	CBrush nProgressBrush, nBkBrush;
	CPen nDarkBorder, nWhiteBorder, nSelectedBorder;
	void DrawHorzBar(CDC *pDC, CRect pRect);
	//{{AFX_MSG(CSwingProgressCtrl)
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SWINGPROGRESSCTRL_H__F2EAB2EE_E47B_43DD_9B35_96776A7E665F__INCLUDED_)
