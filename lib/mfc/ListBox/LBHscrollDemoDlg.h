/////////////////////////////////////////////////////////////////////////////
// LBHscrollDemoDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "hscrolllistbox.h"


/////////////////////////////////////////////////////////////////////////////
// CLBHscrollDemoDlg dialog
/////////////////////////////////////////////////////////////////////////////
class CLBHscrollDemoDlg : public CDialog
{
// Construction
public:
	CLBHscrollDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LBHSCROLLDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButAddStr();
	afx_msg void OnBnClickedButDir();
	afx_msg void OnBnClickedButReset();
	afx_msg void OnBnClickedButTabbed();
	CHScrollListBox m_listBox;
};
