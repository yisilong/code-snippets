#if !defined(AFX_MACRADIOBUTTON_H__D8AD3D95_AC2E_11D3_90F4_0020AFBC499D__INCLUDED_)
#define AFX_MACRADIOBUTTON_H__D8AD3D95_AC2E_11D3_90F4_0020AFBC499D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MacRadioButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMacRadioButton window

class CMacRadioButton : public CButton
{
	DECLARE_DYNAMIC(CMacRadioButton)

// Construction
public:
	CMacRadioButton();

// Attributes
public:

// Operations
public:
	void SetCheck(int nCheck);
	int GetCheck() const;
	void SetBackGround(CDC* pDC);  //当父窗口背景是图片时
	void SetBackGroundColor(COLORREF clr); //当父窗口是其他颜色
	void SetRadioBtnImgPath(CString strChecked = "",CString strUnchecked = "");
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMacRadioButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMacRadioButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMacRadioButton)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSysColorChange();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	void DrawRadio(CDC *pDC, CRect rect, UINT nStyle, UINT nState);
	void InitColors();
	void UncheckOtherRadios();
	BOOL m_bMouseDown;
	void RedrawCheck();
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
private:

	COLORREF m_clrRadioFace;
	COLORREF m_clrShadow;
	COLORREF m_clrHilight;
	COLORREF m_clrDkShadow;
	COLORREF m_clrLightShadow;
	COLORREF m_clrLightFace;
	COLORREF m_clrDarkDkShadow;
	COLORREF m_clrbackground;
	COLORREF m_clrFouse;

	BOOL m_bChecked;
	BOOL m_bOver;  //鼠标是否在上面	
	CDC m_dcBk;    //用于画特殊背景
	BOOL m_bSetBackGround; //是否设置特殊背景

	CString m_strChecked;
	CString m_strUnchecked;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MACRADIOBUTTON_H__D8AD3D95_AC2E_11D3_90F4_0020AFBC499D__INCLUDED_)
