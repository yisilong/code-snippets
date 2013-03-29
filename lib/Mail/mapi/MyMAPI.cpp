// MyMAPI.cpp: implementation of the CMyMAPI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyMAPI.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyMAPI::CMyMAPI()
{
   	m_hModMAPI=NULL;
	m_pMAPISendMail=NULL;
	m_pMAPIResolveName=NULL;
	m_pMAPILogon=NULL;
	m_pMAPILogoff=NULL;
	m_pMAPIFreeBuffer=NULL;
	m_pMAPIAddress=NULL;
	m_pMAPIFindNext=NULL;
    m_pMAPIReadMail=NULL;

	m_lhSession=NULL;
	m_szLastErrorMsg.Empty();
	m_bHaveLogIn = FALSE;
}

CMyMAPI::~CMyMAPI()
{
	if (!m_hModMAPI)
	{
		FreeLibrary(m_hModMAPI);
		m_hModMAPI=NULL;
	}
	m_pMAPISendMail=NULL;
	m_pMAPIResolveName=NULL;
	m_pMAPILogon=NULL;
	m_pMAPILogoff=NULL;
	m_pMAPIFreeBuffer=NULL;
	m_pMAPIAddress=NULL;
	m_pMAPIFindNext=NULL;
    m_pMAPIReadMail=NULL;
	m_lhSession=NULL;
	m_szLastErrorMsg.Empty();
	m_bHaveLogIn= FALSE;
}

BOOL CMyMAPI::Init()
{
	m_hModMAPI=::LoadLibrary("MAPI32.DLL");
	if (!m_hModMAPI)  
	{
		m_szLastErrorMsg="MAPI32.dll loading fail";
		return FALSE;
	}

	m_pMAPISendMail=(lpfnMAPISendMail)GetProcAddress(m_hModMAPI,"MAPISendMail");
	if (!m_pMAPISendMail)  
	{
		m_szLastErrorMsg="Get MAPISendMail ProcAddress error";
		return FALSE;
	}
	m_pMAPIResolveName=(lpfnMAPIResolveName)GetProcAddress(m_hModMAPI,"MAPIResolveName");
	if (!m_pMAPIResolveName)  
	{
		m_szLastErrorMsg="Get MAPIResolveName ProcAddress error";
		return FALSE;
	}
	m_pMAPILogon=(lpfnMAPILogon)::GetProcAddress(m_hModMAPI,"MAPILogon");
	if (!m_pMAPILogon)  
	{
		m_szLastErrorMsg="Get MAPILogon ProcAddress error";
		return FALSE;
	}
	m_pMAPILogoff=(lpfnMAPILogoff)GetProcAddress(m_hModMAPI,"MAPILogoff");
	if (!m_pMAPILogoff)  
	{
		m_szLastErrorMsg="Get MAPILogoff ProcAddress error";
		return FALSE;
	}
	m_pMAPIFreeBuffer=(lpfnMAPIFreeBuffer)GetProcAddress(m_hModMAPI,"MAPIFreeBuffer");
	if (!m_pMAPIFreeBuffer)  
	{
		m_szLastErrorMsg="Get MAPIFreeBuffer ProcAddress error";
		return FALSE;
	}
	m_pMAPIAddress=(lpfnMAPIAddress)GetProcAddress(m_hModMAPI,"MAPIAddress");
	if (!m_pMAPIAddress)  
	{
		m_szLastErrorMsg="Get MAPIAddress ProcAddress error";
		return FALSE;
	}
	m_pMAPIFindNext=(lpfnMAPIFindNext)GetProcAddress(m_hModMAPI,"MAPIFindNext");
	if (!m_pMAPIFindNext)  
	{
		m_szLastErrorMsg="Get MAPIFindNext ProcAddress error";
		return FALSE;
	}
    m_pMAPIReadMail=(lpfnMAPIReadMail)GetProcAddress(m_hModMAPI,"MAPIReadMail");
	if (!m_pMAPIReadMail)  
	{
		m_szLastErrorMsg="Get MAPIReadMail ProcAddress error";
		return FALSE;
	}
    m_bHaveLogIn = TRUE;
	return TRUE;
}


CString CMyMAPI::GetLastError() const
{
	return m_szLastErrorMsg;
}

BOOL CMyMAPI::LogonIn(LPCTSTR lpszUserName,LPCTSTR lpszPassword,BOOL bNewSession /* = FALSE */)
{
	if (!m_hModMAPI)
	{
		m_szLastErrorMsg = "UnInit or Init fail";
		return FALSE;
	}
	if (m_lhSession)
	{
		m_szLastErrorMsg = "Haved LogIn";
		return FALSE;
	}
	if (lpszPassword == "" || lpszPassword == "")
	{
		m_szLastErrorMsg = "parameter error";
		return FALSE;
	}
	FLAGS flags = 0;
	if (bNewSession) flags |= MAPI_NEW_SESSION;
	ULONG lResult=m_pMAPILogon(0,(LPTSTR)lpszUserName,(LPTSTR)lpszPassword,flags,0,&m_lhSession);
	if (SUCCESS_SUCCESS!=lResult)
	{
		switch(lResult)
		{
		case MAPI_E_FAILURE:
			m_szLastErrorMsg="LogonIn error: MAPI_E_FAILURE";
			return FALSE;
		case MAPI_E_INSUFFICIENT_MEMORY:
			m_szLastErrorMsg="LogonIn error: MAPI_E_INSUFFICIENT_MEMORY";
			return FALSE;
		case MAPI_E_LOGIN_FAILURE:
			m_szLastErrorMsg="LogonIn error: MAPI_E_LOGIN_FAILURE";
			return FALSE;
		case MAPI_E_TOO_MANY_SESSIONS:
			m_szLastErrorMsg="LogonIn error: MAPI_E_TOO_MANY_SESSIONSMAPI_E_FAILURE";
			return FALSE;
		case MAPI_E_USER_ABORT:
			m_szLastErrorMsg="LogonIn error: MAPI_E_USER_ABORT";
			return FALSE;
		default: 
			m_szLastErrorMsg="LogonIn error: unkonw error";
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CMyMAPI::Logoff()
{
	if (!m_bHaveLogIn)
	{
		m_szLastErrorMsg = "UnLogIn";
		return FALSE;
	}

	if (m_lhSession)
	{
		// Call the MAPILogoff() function
		ULONG lResult= m_pMAPILogoff(m_lhSession, 0, 0, 0);
		if (SUCCESS_SUCCESS!=lResult)
		{
			switch(lResult)
			{
			case MAPI_E_FAILURE:
				m_szLastErrorMsg = "Logoff error: MAPI_E_FAILURE";
				return FALSE;
			case MAPI_E_INSUFFICIENT_MEMORY:
				m_szLastErrorMsg = "Logoff error: MAPI_E_INSUFFICIENT_MEMORY";
				return FALSE;
			case MAPI_E_INVALID_SESSION:
				m_szLastErrorMsg = "Logoff error: MAPI_E_INVALID_SESSION";
				return FALSE;
			default:
				m_szLastErrorMsg = "Logoff error: unkonw error";
				return FALSE;
			}
		}
		m_lhSession = NULL;
		return TRUE;
	}
	return FALSE;
}

BOOL CMyMAPI::FindMail(LPTSTR lpszMessageID,LPTSTR lpszMessageType /* = NULL */,LPTSTR lpszSeedMessageID /* = NULL */, FLAGS flFlags /* = MAPI_LONG_MSGID | MAPI_UNREAD_ONLY | MAPI_GUARANTEE_FIFO */)
{
	if (!m_bHaveLogIn)
	{
		m_szLastErrorMsg = "UnLogIn";
		return FALSE;
	}
	if (m_lhSession)
	{
		ULONG lResult=m_pMAPIFindNext(m_lhSession, NULL, lpszMessageType,lpszSeedMessageID,flFlags,0, lpszMessageID);
		if (SUCCESS_SUCCESS!=lResult)
		{
			switch(lResult)
			{
			case MAPI_E_FAILURE:
				m_szLastErrorMsg = "FindMail error: MAPI_E_FAILURE";
				return FALSE;
			case MAPI_E_INSUFFICIENT_MEMORY:
				m_szLastErrorMsg = "FindMail error: MAPI_E_INSUFFICIENT_MEMORY";
				return FALSE;
			case MAPI_E_INVALID_MESSAGE:
				m_szLastErrorMsg = "FindMail error: MAPI_E_INVALID_MESSAGE";
				return FALSE;
			case MAPI_E_INVALID_SESSION:
				m_szLastErrorMsg = "FindMail error: MAPI_E_INVALID_SESSION";
				return FALSE;
			case MAPI_E_NO_MESSAGES:
				m_szLastErrorMsg = "FindMail error: MAPI_E_NO_MESSAGES";
				return FALSE;
			default:
				m_szLastErrorMsg = "FindMail error: unkonw error";
				return FALSE;
			}
		}
	}
    return TRUE;
}

BOOL CMyMAPI::ReadMail(lpMapiMessage FAR * lppMessage,LPTSTR lpszMessageID,FLAGS flFlags /* = MAPI_ENVELOPE_ONLY  */,BOOL bMarkAsRead /* = FALSE */)
{
	if (!m_bHaveLogIn)
	{
		m_szLastErrorMsg = "UnLogIn";
		return FALSE;
	}
	FLAGS flag=flFlags;
	if (!bMarkAsRead)	flFlags |=MAPI_PEEK;

	ULONG lResult=m_pMAPIReadMail(m_lhSession, NULL, lpszMessageID,flag, 0, lppMessage);
	if (SUCCESS_SUCCESS!=lResult)
	{
		switch(lResult)
		{
		case MAPI_E_ATTACHMENT_WRITE_FAILURE:
			m_szLastErrorMsg = "ReadMail error: MAPI_E_ATTACHMENT_WRITE_FAILURE";
			return FALSE;
		case MAPI_E_DISK_FULL:
			m_szLastErrorMsg = "ReadMail error: MAPI_E_DISK_FULL";
			return FALSE;
		case MAPI_E_FAILURE:
			m_szLastErrorMsg = "ReadMail error: MAPI_E_FAILURE";
			return FALSE;
		case MAPI_E_INSUFFICIENT_MEMORY:
			m_szLastErrorMsg = "ReadMail error: MAPI_E_INSUFFICIENT_MEMORY";
			return FALSE;
		case MAPI_E_INVALID_MESSAGE:
			m_szLastErrorMsg = "ReadMail error: MAPI_E_INVALID_MESSAGE";
			return FALSE;
		case MAPI_E_INVALID_SESSION:
			m_szLastErrorMsg = "ReadMail error: MAPI_E_INVALID_SESSION";
			return FALSE;
		case MAPI_E_TOO_MANY_FILES:
			m_szLastErrorMsg = "ReadMail error: MAPI_E_TOO_MANY_FILES";
			return FALSE;
		case MAPI_E_TOO_MANY_RECIPIENTS:
			m_szLastErrorMsg = "ReadMail error: MAPI_E_TOO_MANY_RECIPIENTS";
			return FALSE;
		default:
			m_szLastErrorMsg = "ReadMail error: unknow error";
			return FALSE;
		}
	}
	return TRUE;
}
 
BOOL CMyMAPI::FreeBuffer(LPVOID pBuffer)
{
	if (pBuffer == NULL)
	{
		m_szLastErrorMsg = "pBuffer point the NULL";
		return FALSE;
	}
	ULONG lResult = m_pMAPIFreeBuffer(pBuffer);
	if (SUCCESS_SUCCESS!=lResult)
	{
		m_szLastErrorMsg="FreeBuffer error:MAPI_E_FAILURE";
		return FALSE;
	}
	return TRUE;
}
BOOL CMyMAPI::SendMail(LPTSTR Address,LPTSTR Subject, LPTSTR BodyText)
{
	MapiRecipDesc Recipients;
	Recipients.ulReserved = 0;
	Recipients.ulRecipClass = MAPI_TO;   
	Recipients.lpszName = Address;   
	Recipients.lpszAddress = Address;   
	Recipients.ulEIDSize = 0;   
	Recipients.lpEntryID = 0;  
	
	MapiMessage Msg; 	
	memset(&Msg, 0, sizeof(Msg));   
	Msg.lpszSubject = (char*)Subject;   
	Msg.lpszNoteText= (char*)BodyText;   
	Msg.nRecipCount = 1;
	Msg.lpRecips=&Recipients;
	if (!ResolveName(&Msg.lpRecips,Address))
		return FALSE;

	ULONG lResult; 	
	lResult  = m_pMAPISendMail(m_lhSession, 0, &Msg, 0, 0);   
	if(lResult != SUCCESS_SUCCESS)
	{
		switch(lResult)
		{
		case MAPI_E_AMBIGUOUS_RECIPIENT:
			m_szLastErrorMsg = "SendMail error: MAPI_E_AMBIGUOUS_RECIPIENT";
			return FALSE;
		case MAPI_E_ATTACHMENT_NOT_FOUND:
			m_szLastErrorMsg = "SendMail error: MAPI_E_ATTACHMENT_NOT_FOUND";
			return FALSE;
		case MAPI_E_ATTACHMENT_OPEN_FAILURE:
			m_szLastErrorMsg = "SendMail error: MAPI_E_ATTACHMENT_OPEN_FAILURE";
			return FALSE;
		case MAPI_E_BAD_RECIPTYPE:
			m_szLastErrorMsg = "SendMail error: MAPI_E_BAD_RECIPTYPE";
			return FALSE;
		case MAPI_E_FAILURE:
			m_szLastErrorMsg = "SendMail error: MAPI_E_FAILURE";
			return FALSE;
		case MAPI_E_INSUFFICIENT_MEMORY:
			m_szLastErrorMsg = "SendMail error: MAPI_E_INSUFFICIENT_MEMORY";
			return FALSE;
		case MAPI_E_INVALID_RECIPS:
			m_szLastErrorMsg = "SendMail error: MAPI_E_INVALID_RECIPS";
			return FALSE;
		case MAPI_E_LOGIN_FAILURE:
			m_szLastErrorMsg = "SendMail error: MAPI_E_LOGIN_FAILURE";
			return FALSE;
		case MAPI_E_TEXT_TOO_LARGE:
			m_szLastErrorMsg = "SendMail error: MAPI_E_TEXT_TOO_LARGE";
			return FALSE;
		case MAPI_E_TOO_MANY_FILES:
			m_szLastErrorMsg = "SendMail error: MAPI_E_TOO_MANY_FILES";
			return FALSE;
		case MAPI_E_TOO_MANY_RECIPIENTS:
			m_szLastErrorMsg = "SendMail error: MAPI_E_TOO_MANY_RECIPIENTS";
			return FALSE;
		case MAPI_E_UNKNOWN_RECIPIENT:
			m_szLastErrorMsg = "SendMail error: MAPI_E_UNKNOWN_RECIPIENT";
			return FALSE;
		case MAPI_E_USER_ABORT:
			m_szLastErrorMsg = "SendMail error: MAPI_E_USER_ABORT";
			return FALSE;
		default:
			m_szLastErrorMsg = "SendMail error: unknow error";
			return FALSE;
		}		
	}
	return TRUE;
} 

BOOL CMyMAPI::ResolveName(lpMapiRecipDesc FAR *lppRecip,LPTSTR lpszName,FLAGS flFlags/* =MAPI_DIALOG | MAPI_LOGON_UI */)
{
	if (!m_bHaveLogIn)
	{
		m_szLastErrorMsg = "UnLogIn";
		return FALSE;
	}
	
	ULONG lResult=m_pMAPIResolveName(m_lhSession, 0, lpszName,flFlags, 0, lppRecip);
	if (SUCCESS_SUCCESS!=lResult)
	{
		switch(lResult)
		{
		case MAPI_E_AMBIGUOUS_RECIPIENT:
			m_szLastErrorMsg = "ResolveName error: MAPI_E_AMBIGUOUS_RECIPIENT";
			return FALSE;

		case MAPI_E_FAILURE:
			m_szLastErrorMsg = "ResolveName error: MAPI_E_FAILURE";
			return FALSE;
		case MAPI_E_INSUFFICIENT_MEMORY:
			m_szLastErrorMsg = "ResolveName error: MAPI_E_INSUFFICIENT_MEMORY";
			return FALSE;
		case MAPI_E_LOGIN_FAILURE:
			m_szLastErrorMsg = "ResolveName error: MAPI_E_LOGIN_FAILURE";
			return FALSE;
		case MAPI_E_NOT_SUPPORTED:
			m_szLastErrorMsg = "ResolveName error: MAPI_E_NOT_SUPPORTED";
			return FALSE;
		case MAPI_E_USER_ABORT:
			m_szLastErrorMsg = "ResolveName error: MAPI_E_USER_ABORT";
			return FALSE;
		default:
			m_szLastErrorMsg = "ResolveName error: unknow error";
			return FALSE;
		}
	}
	return TRUE;

}

 
