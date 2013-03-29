#if !defined(AFX_COLORCHECKBOX_H__FDC9C955_9912_11D3_90E8_0020AFBC499D__INCLUDED_)
#define AFX_COLORCHECKBOX_H__FDC9C955_9912_11D3_90E8_0020AFBC499D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorCheckBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMacCheckBox window

class CMacCheckBox : public CButton
{
// Construction
public:
	CMacCheckBox();


// Operations
public:
	void SetCheck(int nCheck);
	int GetCheck() const;
	void SetBackGroundColor(COLORREF clr); //当父窗口是其他颜色
	void SetCheckboxImgPath(CString strChecked="",CString strUnchecked="");
private:
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	void InitColors();
	void RedrawCheck();
	void DrawCheckMark(CDC *pDC, CRect rect, UINT nStyle, UINT nState);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMacCheckBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMacCheckBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMacCheckBox)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	COLORREF m_clrCheckBoxFace;
	COLORREF m_clrShadow;
	COLORREF m_clrHilight;
	COLORREF m_clrDkShadow;
	COLORREF m_clrLightShadow;
	COLORREF m_clrFouse;
	BOOL m_bMouseDown;
	BOOL m_bChecked;

	BOOL m_bOver;  //鼠标是否在上面	
	CDC m_dcBk;    //用于画特殊背景
	BOOL m_bSetBackGround; //是否设置特殊背景
	COLORREF m_clrbackground;

	CString   m_strChecked;
	CString   m_strUnchecked;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORCHECKBOX_H__FDC9C955_9912_11D3_90E8_0020AFBC499D__INCLUDED_)
