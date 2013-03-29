// MyMAPI.h: interface for the CMyMAPI class.
//
//////////////////////////////////////////////////////////////////////
/*
ULONG (FAR PASCAL *lpfnMAPISendMail) (LHANDLE lhSession,ULONG ulUIParam, lpMapiMessage lpMessage,FLAGS flFlags, ULONG ulReserved);

ULONG (FAR PASCAL *lpfnMAPIResolveName) (LHANDLE lhSession,ULONG ulUIParam, LPTSTR lpszName, FLAGS ulFlags, ULONG ulReserved,lpMapiRecipDesc FAR *lppRecip);

ULONG (FAR PASCAL *lpfnMAPILogon)(ULONG ulUIParam, LPSTR lpszProfileName, LPSTR lpszPassword,FLAGS flFlags, ULONG ulReserved, LPLHANDLE lplhSession);

ULONG (FAR PASCAL *lpfnMAPILogoff)(LHANDLE lhSession,ULONG ulUIParam, FLAGS flFlags,ULONG ulReserved);

ULONG (FAR PASCAL *lpfnMAPIFreeBuffer)(LPVOID lpBuffer);

ULONG (FAR PASCAL *lpfnMAPIAddress)(LHANDLE lhSession,ULONG ulUIParam, LPSTR lpszCaption,ULONG nEditFields, LPSTR lpszLabels,ULONG nRecips, lpMapiRecipDesc lpRecips,FLAGS flFlags, ULONG ulReserved,LPULONG lpnNewRecips,lpMapiRecipDesc FAR *lppNewRecips);

ULONG (FAR PASCAL *lpfnMAPIFindNext)(LHANDLE lhSession,ULONG ulUIParam, LPSTR lpszMessageType,LPSTR lpszSeedMessageID, FLAGS flFlags, ULONG ulReserved, LPSTR lpszMessageID);

ULONG (FAR PASCAL *lpfnMAPIReadMail)(LHANDLE lhSession,ULONG ulUIParam, LPSTR lpszMessageID,FLAGS flFlags, ULONG ulReserved,lpMapiMessage FAR *lppMessage);
*/

#if !defined(AFX_MYMAPI_H__26D02AC4_A92B_4850_9DC6_891EB3909B23__INCLUDED_)
#define AFX_MYMAPI_H__26D02AC4_A92B_4850_9DC6_891EB3909B23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <MAPI.h>

typedef ULONG (FAR PASCAL *lpfnMAPISendMail) (LHANDLE,ULONG, lpMapiMessage,FLAGS, ULONG);

typedef ULONG (FAR PASCAL *lpfnMAPIResolveName) (LHANDLE,ULONG, LPTSTR, FLAGS, ULONG,lpMapiRecipDesc FAR *);

typedef ULONG (FAR PASCAL *lpfnMAPILogon)(ULONG,LPTSTR,LPTSTR,FLAGS,ULONG,LPLHANDLE);

typedef ULONG (FAR PASCAL *lpfnMAPILogoff)(LHANDLE,ULONG,FLAGS,ULONG);

typedef ULONG (FAR PASCAL *lpfnMAPIFreeBuffer)(LPVOID);

typedef ULONG (FAR PASCAL *lpfnMAPIAddress)(LHANDLE,ULONG, LPTSTR,ULONG, LPTSTR,ULONG, lpMapiRecipDesc,FLAGS, ULONG,LPULONG,lpMapiRecipDesc FAR *);

typedef ULONG (FAR PASCAL *lpfnMAPIFindNext)(LHANDLE,ULONG,LPTSTR,LPTSTR,FLAGS,ULONG,LPTSTR);

typedef ULONG (FAR PASCAL *lpfnMAPIReadMail)(LHANDLE ,ULONG , LPTSTR ,FLAGS , ULONG ,lpMapiMessage FAR *);

class CMyMAPI  
{
public:

	CMyMAPI();
	virtual ~CMyMAPI();
public:
	BOOL Init();
	CString GetLastError() const;
	BOOL LogonIn(LPCTSTR lpszUserName,LPCTSTR lpszPassword,BOOL bNewSession = FALSE);
	BOOL Logoff();
	BOOL FindMail(LPTSTR lpszMessageID,LPTSTR lpszMessageType = NULL,LPTSTR  lpszSeedMessageID = NULL,
		FLAGS flFlags = MAPI_LONG_MSGID | MAPI_UNREAD_ONLY | MAPI_GUARANTEE_FIFO);
	BOOL ReadMail(lpMapiMessage FAR * lppMessage,LPTSTR lpszMessageID,FLAGS flFlags = MAPI_ENVELOPE_ONLY ,BOOL bMarkAsRead = FALSE);
	BOOL FreeBuffer(LPVOID pBuffer);
	BOOL SendMail(LPTSTR Address,LPTSTR Subject,  LPTSTR BodyText);

//	BOOL MAPIAddress();//µ÷ÓÃµØÖ·±¡
private:
	BOOL ResolveName(lpMapiRecipDesc FAR *lppRecip,LPTSTR lpszName,FLAGS flFlags=MAPI_DIALOG | MAPI_LOGON_UI);
private:
	HMODULE m_hModMAPI;	

	lpfnMAPILogon  m_pMAPILogon;
	lpfnMAPILogoff m_pMAPILogoff;
	lpfnMAPIFindNext m_pMAPIFindNext;
    lpfnMAPIReadMail m_pMAPIReadMail;
	lpfnMAPIFreeBuffer m_pMAPIFreeBuffer;
	lpfnMAPISendMail m_pMAPISendMail;
	lpfnMAPIResolveName m_pMAPIResolveName;
	
	lpfnMAPIAddress m_pMAPIAddress;

	CString m_szLastErrorMsg;
	LHANDLE m_lhSession;
	BOOL m_bHaveLogIn;
};

#endif // !defined(AFX_MYMAPI_H__26D02AC4_A92B_4850_9DC6_891EB3909B23__INCLUDED_)
