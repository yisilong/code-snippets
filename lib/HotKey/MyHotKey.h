#if !defined(AFX_MYHOTKEY_H__0A7A42B2_DEEF_4EB4_B93D_F24A57B66E80__INCLUDED_)
#define AFX_MYHOTKEY_H__0A7A42B2_DEEF_4EB4_B93D_F24A57B66E80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyHotKey.h : header file
//
/************************************************************************/
/* 
	1.可将CHotKeyCtrl子类化于此对象.
    2.可单独使用，并不子类化.
*/
/************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// CMyHotKey window

class CMyHotKey : public CHotKeyCtrl
{
// Construction
public:
	CMyHotKey();
	virtual ~CMyHotKey();
	
// Attributes
private:
	static int nCurrentHotKeyIncrease;
	int  m_nHotKeyID;
	HWND m_hOwnerWnd;

// Operations
public:
	int  GetHotKeyID() { return m_nHotKeyID; }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyHotKey)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL SetupHotKey(LPCTSTR strKeyName,LPCTSTR strCtrlKey,HWND hRecvWnd);
	BOOL SetupHotKey(WORD wKeyCode,WORD wCtrlCode,HWND hRecvWnd);

	//适用于关联到CHotKeyCtrl控件的情况
	BOOL SetupHotKey(HWND hRecvWnd);

	void SetHotKeyString(LPCTSTR strKeyName,LPCTSTR strCtrlKey);
private:
	void ResetHotKey();
	
	WORD GetVirtualCtrlCode(LPCTSTR strCtrlKey);
	WORD GetVirtualKeyCode(LPCTSTR strKeyName);
	// Generated message map functions
protected:
	//{{AFX_MSG(CMyHotKey)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYHOTKEY_H__0A7A42B2_DEEF_4EB4_B93D_F24A57B66E80__INCLUDED_)
