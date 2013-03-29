#if !defined(AFX_MYEDIT_H__7AE5DE9E_76C4_496A_AF5F_B39F3D703CA4__INCLUDED_)
#define AFX_MYEDIT_H__7AE5DE9E_76C4_496A_AF5F_B39F3D703CA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyEdit window

class CMyEdit : public CEdit
{
// Construction
public:
	CMyEdit();

// Attributes
public:
private:
	bool     m_bIsReadOnly;               //是否只读
	bool     m_bMouseOver;	             // 鼠标是否经过
	bool     m_bFocus;                 	 // 是否有焦点

	COLORREF m_clrNormalFrmClr;	         // 控件无焦点时,控件边框颜色
	COLORREF m_clrFocusFrmClr;     	     // 控件有焦点时,控件边框颜色
	COLORREF m_clrTextColor;	         // 字体颜色

	CBrush   m_TextBkBrush;	             // 字体背景画刷
	CFont    m_ftEditFont;	             // 字体
    int      m_nFontSize;                // 字号
	COLORREF m_clrNormalBkColor;         // 无焦点时字体浅景色
    COLORREF m_clrFocusBkColor;          // 有焦点时字体浅景色
	UINT     m_nVMargins;                // 垂直上下间距
// Operations
public:
	void     SetHMargins(UINT nLeft, UINT nRight);      //设置水平间距
	void     SetVMargins(UINT nVMargins);  //设置垂直间距
	
	void     SetEditFont(int nSize, LPCSTR lpszFontName = _T( "Arial" ));	    // Set font
	COLORREF SetNormalFrmClr( COLORREF clrNormalFrmClr );	                    // Set normal frame color	
	COLORREF GetNormalFrmClr() const;                                           // Get normal frame color
	COLORREF SetFocusFrmClr( COLORREF clrFocusFrmClr );	                        // Set focus frame color
	COLORREF GetFocusFrmClr() const;	                                        // Get focus frame color
	COLORREF SetTextColor( COLORREF clrTextColor );	                            // Set text color
	COLORREF GetTextColor() const;	                                            // Get text color
	COLORREF SetBkColor( COLORREF clrBkColor );	                                // Set back color
	COLORREF GetBkColor() const;                                                // Get text back color
	BOOL     ModifyStyle( DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0 );           //
	BOOL     ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);
	void     EnableDropFiles(BOOL bEnable = TRUE);
	
private:
    void     Draw(CDC *pDC);                                    //辅助绘制函数  	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyEdit)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CMyEdit();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CMyEdit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pNewWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYEDIT_H__7AE5DE9E_76C4_496A_AF5F_B39F3D703CA4__INCLUDED_)
