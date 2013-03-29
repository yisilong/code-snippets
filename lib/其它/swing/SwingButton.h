#if !defined(AFX_SWINGBUTTON_H__3FCDAC33_73E4_4D36_BA32_E389646272EF__INCLUDED_)
#define AFX_SWINGBUTTON_H__3FCDAC33_73E4_4D36_BA32_E389646272EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SwingButton.h : header file
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
// CSwingButton window

class  CSwingButton : public CButton
{
// Construction
public:
	CSwingButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSwingButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	CString GetButtonText();
	void DrawButton(LPDRAWITEMSTRUCT lpDIS);
	void  SetButtonText (CString pString);
	virtual ~CSwingButton();

	// Generated message map functions
protected:
	CString m_strCaption;
	CBrush nActiveBrush, nInactiveBrush;
	CPen nDarkBorder, nWhiteBorder, nSelectedBorder;
	//{{AFX_MSG(CSwingButton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SWINGBUTTON_H__3FCDAC33_73E4_4D36_BA32_E389646272EF__INCLUDED_)
