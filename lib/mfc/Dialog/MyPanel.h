#if !defined(AFX_MYPANEL_H__0E538463_B96D_43B1_9F48_0D213ACD3B88__INCLUDED_)
#define AFX_MYPANEL_H__0E538463_B96D_43B1_9F48_0D213ACD3B88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyPanel.h : header file
//

#include "ScrollHelper.h"
/////////////////////////////////////////////////////////////////////////////
// CMyPanel dialog

class CMyPanel : public CDialog
{
// Construction
public:
	CMyPanel(UINT nIDTemplate,CWnd* pParent = NULL);   // standard constructor
	~CMyPanel();
// Dialog Data
	//{{AFX_DATA(CMyPanel)
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyPanel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void   SetDisplaySize(int displayWidth, int displayHeight);
	void   OffsetDispSize(int nWidth,int nHeight);
	CSize  GetDisplaySize() const;
	CSize  GetScrollPos() const;
// Implementation
protected:
	 CScrollHelper* m_pScrollHelper;
	 int m_nDisplayHeight;   //完全显示需要的高度
	 int m_nDisplayWidth;	//完全显示需要的宽度



	// Generated message map functions
	//{{AFX_MSG(CMyPanel)
	afx_msg void OnNcDestroy();
 	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
 	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
 	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
 	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYPANEL_H__0E538463_B96D_43B1_9F48_0D213ACD3B88__INCLUDED_)
