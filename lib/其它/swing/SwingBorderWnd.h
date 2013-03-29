#if !defined(AFX_SWINGBORDERWND_H__6051417E_D11F_4C97_9B14_A127682AF7D5__INCLUDED_)
#define AFX_SWINGBORDERWND_H__6051417E_D11F_4C97_9B14_A127682AF7D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SwingBorderWnd.h : header file
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
// CSwingBorderWnd window

class  CSwingBorderWnd : public CWnd
{
// Construction
public:
	CSwingBorderWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSwingBorderWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSwingBorderWnd();

	// Generated message map functions
protected:
	CBrush nActiveBrush, nInactiveBrush;
	CPen nDarkBorder, nWhiteBorder, nSelectedBorder;
	//{{AFX_MSG(CSwingBorderWnd)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SWINGBORDERWND_H__6051417E_D11F_4C97_9B14_A127682AF7D5__INCLUDED_)
