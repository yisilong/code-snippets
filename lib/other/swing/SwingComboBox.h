#if !defined(AFX_SWINGCOMBOBOX_H__B6D7EDF2_490F_4E3E_A92C_0339DFC1FFF6__INCLUDED_)
#define AFX_SWINGCOMBOBOX_H__B6D7EDF2_490F_4E3E_A92C_0339DFC1FFF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SwingComboBox.h : header file
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

#include "SwingButton.h"
/////////////////////////////////////////////////////////////////////////////
// CSwingComboBox window

class  CSwingComboBox : public CComboBox
{
// Construction
public:
	CSwingComboBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSwingComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSwingComboBox();

	// Generated message map functions
protected:
	CBrush nActiveBrush, nInactiveBrush;
	CPen nDarkBorder, nWhiteBorder, nSelectedBorder;
	//{{AFX_MSG(CSwingComboBox)
	afx_msg void OnNcPaint();
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SWINGCOMBOBOX_H__B6D7EDF2_490F_4E3E_A92C_0339DFC1FFF6__INCLUDED_)
