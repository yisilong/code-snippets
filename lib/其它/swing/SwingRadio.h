#if !defined(AFX_SWINGRADIO_H__C7A88230_59A7_4D36_A8C4_F783DCDA2C0D__INCLUDED_)
#define AFX_SWINGRADIO_H__C7A88230_59A7_4D36_A8C4_F783DCDA2C0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SwingRadio.h : header file
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
// CSwingRadio window

class  CSwingRadio : public CButton
{
// Construction
public:
	CSwingRadio();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSwingRadio)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_Checked;
	void SetCheck();
	BOOL GetCheck();
	CString GetRadioText();
	void SetRadioText (CString pStr);
	virtual ~CSwingRadio();

	// Generated message map functions
protected:
	void UncheckOtherRadios();
	CString m_strCaption;
	CBrush nActiveBrush, nInactiveBrush, nCheck;
	CPen nDarkBorder, nWhiteBorder, nSelectedBorder;
	//{{AFX_MSG(CSwingRadio)
	afx_msg void OnClicked();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SWINGRADIO_H__C7A88230_59A7_4D36_A8C4_F783DCDA2C0D__INCLUDED_)
