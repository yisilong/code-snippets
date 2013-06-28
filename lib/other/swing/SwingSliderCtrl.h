#if !defined(AFX_SWINGSLIDERCTRL_H__B80F7D07_0F4F_407E_AB9D_0D682B15923A__INCLUDED_)
#define AFX_SWINGSLIDERCTRL_H__B80F7D07_0F4F_407E_AB9D_0D682B15923A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SwingSliderCtrl.h : header file
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
// CSwingSliderCtrl window

class  CSwingSliderCtrl : public CSliderCtrl
{
// Construction
public:
	CSwingSliderCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSwingSliderCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void DrawChannel (CDC *pDC, CRect pRect);
	void DrawInactiveDC();
	void DrawActiveDC();
	void DrawHorizontalThumb(CDC *pDC, CRect rect, BOOL hasFocus);
	void DrawVerticalThumb (CDC *pDC, CRect pRect, BOOL hasFocus);
	virtual ~CSwingSliderCtrl();

	// Generated message map functions
protected:
	CRect nLastRect;
	CPen  nDarkPen, nSelThumbPen, nUnselThumbPen;
	CBrush nSelectedPen;
	CDC nInactiveDC, nActiveDC, nTempDC;
	CBrush pBackBrush, pSelBackBrush, nGreyBrush;
	POINT points[5];
	BOOL bFocus;

	//{{AFX_MSG(CSwingSliderCtrl)
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SWINGSLIDERCTRL_H__B80F7D07_0F4F_407E_AB9D_0D682B15923A__INCLUDED_)
