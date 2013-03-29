#if !defined(AFX_SWINGRICHEDITCTRL_H__A7549B5F_C5E6_4813_BF84_4CFBBE7A55F3__INCLUDED_)
#define AFX_SWINGRICHEDITCTRL_H__A7549B5F_C5E6_4813_BF84_4CFBBE7A55F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SwingRichEditCtrl.h : header file
//
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

/////////////////////////////////////////////////////////////////////////////
// CSwingRichEditCtrl window

class  CSwingRichEditCtrl : public CRichEditCtrl
{
// Construction
public:
	CSwingRichEditCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSwingRichEditCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSwingRichEditCtrl();

	// Generated message map functions
protected:
	CBrush nActiveBrush, nInactiveBrush;
	CPen nDarkBorder, nWhiteBorder, nSelectedBorder;
	//{{AFX_MSG(CSwingRichEditCtrl)
	afx_msg void OnNcPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SWINGRICHEDITCTRL_H__A7549B5F_C5E6_4813_BF84_4CFBBE7A55F3__INCLUDED_)
