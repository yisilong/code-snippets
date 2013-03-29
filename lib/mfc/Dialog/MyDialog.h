#if !defined(AFX_MYDIALOG_H__91C840E6_2B61_4A5C_B46C_64F1B83CF247__INCLUDED_)
#define AFX_MYDIALOG_H__91C840E6_2B61_4A5C_B46C_64F1B83CF247__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyDialog dialog

class CMyDialog : public CDialog
{
// Construction
public:
	CMyDialog(UINT nIDTemplate,CWnd* pParent = NULL);   // standard constructor

	enum DRAWBUTTONSTATUS {
		DTS_NORMAL,DTS_HOVER,DTS_CLICKED
	};

// Dialog Data
	//{{AFX_DATA(CMyDialog)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
	int m_nTopBorder;
	int m_nBottomBorder;
	int m_nLeftBorder;
	int m_nRightBorder;

	CRect m_rcIcon;
	CRect m_rcMin;
//	CRect m_rcMax;
	CRect m_rcClose;

	BOOL m_bMinHover;
//	BOOL m_bMaxHover;
	BOOL m_bCloseHover;

	BOOL m_bTracked;

	CDC m_StoreBgDC;  //储存画好的背景，每次直接粘贴，加快速度
	BOOL m_bHaveStoreBgDC;

public:
	void DrawMinButton(CDC *pDC,int enumStatus);
	void DrawCloseButton(CDC *pDC,int enumStatus);
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyDialog)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYDIALOG_H__91C840E6_2B61_4A5C_B46C_64F1B83CF247__INCLUDED_)
