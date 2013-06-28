//----------------------------------------------------设计人:牛文平
#if !defined(AFX_MYTOOLBAREX_H__D152D63B_A5F3_4646_B062_D2D04653ED1B__INCLUDED_)
#define AFX_MYTOOLBAREX_H__D152D63B_A5F3_4646_B062_D2D04653ED1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
// CMyToolBar window

class CMyToolBar : public CToolBar
{
	DECLARE_DYNAMIC(CMyToolBar)
// Construction
public:
	CMyToolBar();

// Attributes
public:

// Operations
public:
	BOOL LoadToolBar(UINT nIDResource);
	void RepaintBackground();
	void DrawSeparators();
	void DrawSeparators(CClientDC* pDC);
	void DrawGripper(CWindowDC *pDC, CRect& rectWindow);
	BOOL PreTranslateMessage(MSG* pMsg);
	void MouseHover(POINT pt);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyToolBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnBarStyleChange(DWORD dwOldStyle, DWORD dwNewStyle);
	BOOL LoadBitmap(LPCTSTR);
	void SetBKImage(UINT);
	void SetBKImage(LPCTSTR);
	void SetBKColor(COLORREF);
	void EraseNonClient();
	void SetDefaultStyle();
	void SetShowText(BOOL blnShow);
	virtual ~CMyToolBar();
	virtual void SetSizes(SIZE sizeButton);
	virtual void SetSizes( SIZE sizeButton, SIZE sizeImage );
private:
	LPBYTE m_pbtButtonStyle;
	LPCTSTR m_lpszResourceName;
	BOOL SetButtonEx(UINT);
	BOOL SetButtonEx(LPCTSTR);
	UINT m_nButtonCount;
	UINT m_nSeparatorCount;
	CSize m_ButtonSize;
	CSize m_ButtonImageSize;
	CImageList m_imglstNormal;
	CImageList m_imglstDisabled;
	CImageList m_imglstHot;
	UINT m_nButtonHeight;
	UINT m_nButtonWidth;
	WORD m_wToolBarBitCount;
	CBrush m_bkbrush;
	COLORREF m_clrBack;
	enum BackStyle{bs_ORG,bs_RGB,bs_BITMAP};
	BackStyle m_bBKStyle;
	// Generated message map functions
protected:
	//{{AFX_MSG(CMyToolBar)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBAREX_H__D152D63B_A5F3_4646_B062_D2D04653ED1B__INCLUDED_)
