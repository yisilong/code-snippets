#if !defined(AFX_SWINGCHECKBOX_H__B73F7941_9702_4C68_8227_6E13300E62D7__INCLUDED_)
#define AFX_SWINGCHECKBOX_H__B73F7941_9702_4C68_8227_6E13300E62D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SwingCheckBox.h : header file
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
// CSwingCheckBox window

class  CSwingCheckBox : public CButton
{
// Construction
public:
	CSwingCheckBox();

// Attributes
public:

// Operations
public:
	BOOL GetCheck() { return m_Checked;}
	void SetCheck(BOOL check = TRUE) { m_Checked = check;}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSwingCheckBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	CString GetCheckboxText();
	void SetCheckboxText (CString pStr);
	virtual ~CSwingCheckBox();

	// Generated message map functions
protected:
	CString m_strCaption;
	CBrush nActiveBrush, nInactiveBrush;
	CPen nDarkBorder, nWhiteBorder, nSelectedBorder, nCheck;
	BOOL m_Checked;
	//{{AFX_MSG(CSwingCheckBox)
	afx_msg void OnClicked();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SWINGCHECKBOX_H__B73F7941_9702_4C68_8227_6E13300E62D7__INCLUDED_)
