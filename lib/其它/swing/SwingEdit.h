#if !defined(AFX_SWINGEDIT_H__4CBA701D_1CF6_4AFD_A1F4_8D399C303B0D__INCLUDED_)
#define AFX_SWINGEDIT_H__4CBA701D_1CF6_4AFD_A1F4_8D399C303B0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SwingEdit.h : header file
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
// CSwingEdit window

class  CSwingEdit : public CEdit
{
// Construction
public:
	CSwingEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSwingEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSwingEdit();

	// Generated message map functions
protected:
	CBrush nActiveBrush, nInactiveBrush;
	CPen nDarkBorder, nWhiteBorder, nSelectedBorder;
	//{{AFX_MSG(CSwingEdit)
	afx_msg void OnNcPaint();
	afx_msg void OnHscroll();
	afx_msg void OnVscroll();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SWINGEDIT_H__4CBA701D_1CF6_4AFD_A1F4_8D399C303B0D__INCLUDED_)
