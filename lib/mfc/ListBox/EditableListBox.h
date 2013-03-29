#if !defined(AFX_EDITHELPER_H__546F7AFD_CFEC_416A_8896_B689665E3E86__INCLUDED_)
#define AFX_EDITHELPER_H__546F7AFD_CFEC_416A_8896_B689665E3E86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditHelper.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#define		WM_APP_ED_EDIT_FINISHED			( WM_APP + 04101 )		

/////////////////////////////////////////////////////////////////////////////
// CEditHelper window

class CEditHelper : public CEdit
{
	// Construction
public:
	CEditHelper();
	
	// Attributes
public:
	
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditHelper)
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CEditHelper();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CEditHelper)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

#if !defined(AFX_EDITABLELISTBOX_H__24BA31FC_1440_414B_B272_11723F7BFB6D__INCLUDED_)
#define AFX_EDITABLELISTBOX_H__24BA31FC_1440_414B_B272_11723F7BFB6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditableListBox.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#define		WM_APP_LB_ITEM_EDITED			( WM_APP + 04100 )

/////////////////////////////////////////////////////////////////////////////
// CEditableListBox window

class CEditableListBox : public CListBox
{
// Construction
public:
	CEditableListBox();

// Attributes
protected:
	CEditHelper		m_ceEdit;
	int				m_iItemBeingEdited;

// Operations
protected:
	void	EditStarts	();
	void	EditEnds	( BOOL bCommitText = TRUE );

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditableListBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditableListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditableListBox)
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	afx_msg LRESULT OnEditFinished(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITABLELISTBOX_H__24BA31FC_1440_414B_B272_11723F7BFB6D__INCLUDED_)*/
#endif
