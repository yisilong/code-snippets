//MyStatic.h
#if !defined(AFX_MYSTATIC_H__78BB3B69_440F_4A81_80BE_A219A53A80EC__INCLUDED_)
#define AFX_MYSTATIC_H__78BB3B69_440F_4A81_80BE_A219A53A80EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyStatic.h : header file
//
#ifndef IDC_HAND
#define IDC_HAND MAKEINTRESOURCE(32649)
#endif
/////////////////////////////////////////////////////////////////////////////
// CMyStatic window

class CMyStatic : public CStatic
{
// Construction
public:
	CMyStatic();

// Attributes
public:

// Operations
public:
	// interface declaration
    void SetTextColor(COLORREF clrText);  // 设置文本颜色
    void SetBackgroundColor(COLORREF clrBackgroundColor);  //设置控件背景颜色
	void SetBkColor(COLORREF clrBkTextColor);  //设置文本背景颜色
    void EnableTrack(BOOL bEnable = FALSE);  // 是否鼠标跟踪.即鼠标移入控件时指针切换的功能
    void SetLinkUrl(LPCTSTR lpszTargetUrl,LPCTSTR lpszCmd="mailto:");    // 设置目标链接地址和命令
	void SetBkMode(int iBkMode=TRANSPARENT);//设置文本显示模式
	void SetWindowText(LPCTSTR lpszString );    //设置文本
protected:
	BOOL m_bHover;            //鼠标是否位于空间之上
    BOOL m_bTrack;                 //是否跟踪控件区域（显示小手）
    COLORREF m_clrTextColor;       //字体颜色
    COLORREF m_clrBkTextColor;  //文字背景颜色
    CBrush m_Brush;        //控件背景颜色画刷
    CString m_sTargetUrl; //超链接
	CString m_strCmd;   //命令,如mailto,打开网页时为""
	int m_nBkMode;    //文字显示背景混合模式
    CPoint m_ptCenter; //空间原始中心
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyStatic)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

public:
	virtual ~CMyStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyStatic)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnClicked();
	//}}AFX_MSG
    afx_msg HBRUSH CtlColor(CDC *pDC, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSTATIC_H__78BB3B69_440F_4A81_80BE_A219A53A80EC__INCLUDED_)
