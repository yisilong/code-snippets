#if !defined(AFX_MYBUTTON_H__44CD5B2A_756E_4939_9261_E0034E0F2DEF__INCLUDED_)
#define AFX_MYBUTTON_H__44CD5B2A_756E_4939_9261_E0034E0F2DEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyButton window
#include "FICS.h"
class CMyButton : public CButton
{
// Construction
public:
	CMyButton();

	// Attributes
protected:
	//按钮的状态
	BOOL m_bOver;	    //鼠标位于按钮之上时该值为true，反之为flase
	BOOL m_bTracked;	//在鼠标位于按钮上,避免一直发送OnMouseHover消息
	CString m_strPathNormal; //正常时候的按钮背景
	CString m_strPathHover;  //获得焦点时的按钮的背景
	BOOL m_bIsDrawImgBtn;    //是否画以图片为背景的按钮
	Color m_clrTodStartNormal;
	Color m_clrBottomEndNormal;
	COLORREF m_clrBorderNormal;
	Color m_clrTodStartHover;
	Color m_clrBottomEndHover;
	COLORREF m_clrBorderHover;

// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyButton)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	LRESULT OnSetStyle(WPARAM wParam, LPARAM lParam);
	void SetBtnBkImgPath(CString strNormal,CString strHover);
	void SetDrawImgBtn(BOOL bDrawImgBtn = TRUE);
	void SetNormalColors(Color clrTodStartNormal,Color clrBottomEndNormal,COLORREF clrBorderNormal);
	void SetHoverColors(Color clrTodStartHover,Color clrBottomEndHover,	COLORREF clrBorderHover);
	// Implementation
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual ~CMyButton();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CMyButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYBUTTON_H__44CD5B2A_756E_4939_9261_E0034E0F2DEF__INCLUDED_)
