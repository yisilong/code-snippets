// HotKeyExt.cpp : implementation file
//

#include "stdafx.h"
#include "MyHotKey.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define  RUN_IN_DLL   
int CMyHotKey::nCurrentHotKeyIncrease = 1;
/////////////////////////////////////////////////////////////////////////////
// CMyHotKey

CMyHotKey::CMyHotKey()
{
	m_nHotKeyID = 0;
	m_hOwnerWnd = NULL;
}

CMyHotKey::~CMyHotKey()
{
	UnregisterHotKey(m_hOwnerWnd,m_nHotKeyID);
}


BEGIN_MESSAGE_MAP(CMyHotKey, CHotKeyCtrl)
	//{{AFX_MSG_MAP(CMyHotKey)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyHotKey message handlers

/************************************************************************/
/* 设置当前的HOT KEY                                                    */
/************************************************************************/
void CMyHotKey::SetHotKeyString(LPCTSTR strKeyName, LPCTSTR strCtrlKey)
{
	CHotKeyCtrl::SetHotKey(GetVirtualKeyCode(strKeyName),GetVirtualCtrlCode(strCtrlKey));
}

WORD CMyHotKey::GetVirtualCtrlCode(LPCTSTR strCtrlKey)
{
	WORD wCode = 0;
	CString strTemp = strCtrlKey;
	strTemp.MakeUpper();
	if(strTemp.Find(_T("CTRL")) != -1)
		wCode |= HOTKEYF_CONTROL;
	if(strTemp.Find(_T("ALT")) != -1)
		wCode |= HOTKEYF_ALT;
	if(strTemp.Find(_T("EXT")) != -1)
		wCode |= HOTKEYF_EXT;
	if(strTemp.Find(_T("SHIFT")) != -1)
		wCode |= HOTKEYF_SHIFT;
	return wCode;
}

WORD CMyHotKey::GetVirtualKeyCode(LPCTSTR strKeyName)
{
	WORD wCode = 0;
	int  nKeyLen = lstrlen(strKeyName);
	if(nKeyLen == 1)
	{
		TCHAR strKey ;
		lstrcpyn(&strKey,strKeyName,2);
		if(strKey >= _T('A') && strKey <= _T('Z'))
			wCode = 0x41 + strKey - _T('A');
		else if(strKey >= _T('a') && strKey <= _T('z'))
			wCode = 0x41 + strKey - _T('a');
		else if(strKey >= _T('0') && strKey <= _T('9'))
			wCode = 0x30 + strKey - _T('0');
	}
	else if(nKeyLen == 2)
	{
		TCHAR strKey[2];
		lstrcpyn(strKey,strKeyName,3);
		if(strKey[0] == _T('F'))
		{
			if(strKey[1] >= _T('1') && strKey[1] <= _T('9'))
				wCode = 0x70 + strKey[1] - _T('1');
		}
	}
	else if(nKeyLen == 3)
	{
		TCHAR strKey[3];
		lstrcpyn(strKey,strKeyName,4);
		if(strKey[0] == _T('F'))
		{
			if(strKey[1] == _T('1'))
			{
				if(strKey[2] >= _T('0') && strKey[2] <= _T('9'))
					wCode = 0x79 + strKey[2] - _T('0');
			}
			else if(strKey[1] == _T('2'))
			{
				if(strKey[2] >= _T('0') && strKey[2] <= _T('4'))
					wCode = 0x83 + strKey[2] - _T('0');
			}
		}
	}
	return wCode;
}
/************************************************************************/
/* 清除当前的HOT KEY                                                    */
/************************************************************************/
void CMyHotKey::ResetHotKey()
{
	CHotKeyCtrl::SetHotKey(0,0);
}

BOOL CMyHotKey::SetupHotKey(HWND hRecvWnd)
{
	WORD wKeyCode,wCtrlCode;
	GetHotKey(wKeyCode,wCtrlCode);
	return SetupHotKey(wKeyCode,wCtrlCode,hRecvWnd);
}

BOOL CMyHotKey::SetupHotKey(LPCTSTR strKeyName,LPCTSTR strCtrlKey,HWND hRecvWnd)
{
	WORD wKeyCode,wCtrlCode;
	wKeyCode = GetVirtualKeyCode(strKeyName);
	wCtrlCode = GetVirtualCtrlCode(strCtrlKey);
	return SetupHotKey(wKeyCode,wCtrlCode,hRecvWnd);
}

BOOL CMyHotKey::SetupHotKey(WORD wKeyCode,WORD wCtrlCode,HWND hRecvWnd)
{
	if(m_nHotKeyID > 0)
	{
		UnregisterHotKey(m_hOwnerWnd,m_nHotKeyID);
		m_nHotKeyID = 0;
	}
	BOOL bRet = FALSE;
	UINT nCtrlCode = 0;
	if(wCtrlCode & HOTKEYF_CONTROL)
		nCtrlCode |= MOD_CONTROL;
	if(wCtrlCode & HOTKEYF_ALT)
		nCtrlCode |= MOD_ALT;
	if(wCtrlCode & HOTKEYF_SHIFT)
		nCtrlCode |= MOD_SHIFT;
	if(wCtrlCode & HOTKEYF_EXT)
		nCtrlCode |= MOD_WIN;
	UINT nKeyCode = wKeyCode;
	m_hOwnerWnd = hRecvWnd;
	//设置热键ID
	CString strHotKeyName;
	strHotKeyName.Format("%d_%d",wCtrlCode,wKeyCode);
#ifdef RUN_IN_DLL
	m_nHotKeyID = GlobalAddAtom(strHotKeyName);
#else
	m_nHotKeyID = nCurrentHotKeyIncrease++;
#endif
	DWORD dwErr = 0;
	bRet = RegisterHotKey(m_hOwnerWnd,m_nHotKeyID,nCtrlCode,nKeyCode);
	if (!bRet)
	{
		dwErr = GetLastError();
		if(dwErr == ERROR_HOTKEY_ALREADY_REGISTERED)
			MessageBox("热键设置重复！");
	}
	return bRet;
}
