
#if !defined(AFX_SEARCHBOX_H__4AAEB096_F117_4139_8F63_94C9E64041E5__INCLUDED_)
#define AFX_SEARCHBOX_H__4AAEB096_F117_4139_8F63_94C9E64041E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySearchBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMySearchBox window
#define UM_BEGINSEARCH WM_USER + 1
class CMySearchBox : public CWnd
{
// Construction
public:
	CMySearchBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySearchBox)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	void GetSearchText(CString& strSearch)
	{
		m_pEdit.GetWindowText(strSearch);
	}
	BOOL Create(DWORD dwStyle, const RECT& rcLayout, CWnd* pParentWnd, UINT nID);
	virtual ~CMySearchBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMySearchBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonClick();
	afx_msg void OnNcPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CEdit m_pEdit;
	CButton m_pBtn;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEARCHBOX_H__4AAEB096_F117_4139_8F63_94C9E64041E5__INCLUDED_)
