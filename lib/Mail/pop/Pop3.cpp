// Pop3.cpp: implementation of the CPop3 class.
//
//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//	2003.1.1 功能基本完成
//
//
//
//
//
//
//
//
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <process.h>
#include "Pop3Monitor.h"
#include "Pop3.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//CPop3::CPop3(HWND hWnd)
CPop3::CPop3()
{
	m_bSocketOK = false;
	m_bConnected = false;
	m_bReceiving = false;
	m_sPop3Socket = INVALID_SOCKET;
	m_hThreadHandle = NULL;
	m_dwThreadId = 0;
	m_dwTimeout = 20000;
	m_lReceiveBufSize = RECEIVEBUFSIZE;
	m_szReceiveBuf = new CHAR[m_lReceiveBufSize+1];
	m_fpTmp = NULL;
#ifndef USE_CSTRING
	m_szReceive = NULL;
	m_lReceive = 0;
#endif
	m_hEventWaitingRespond = CreateEvent(NULL , TRUE , FALSE , "Waiting for POP3 Server Respond");
	ResetEvent(m_hEventWaitingRespond);
	m_hEventExitThread = CreateEvent(NULL , TRUE , FALSE , "Waiting Thread Exit");
	ResetEvent(m_hEventExitThread);
}

CPop3::~CPop3()
{
	CloseHandle(m_hEventWaitingRespond);
	CloseHandle(m_hEventExitThread);
#ifndef USE_CSTRING
	if( NULL != m_szReceive )
		delete []m_szReceive;
#endif
	if( NULL != m_szReceiveBuf )
		delete []m_szReceiveBuf;
}

BOOL CPop3::SetReceiveBufSize(long lSize)
{
	if( NULL != m_szReceiveBuf )
	{
		delete []m_szReceiveBuf;
		m_szReceiveBuf = NULL;
	}
	m_lReceiveBufSize = lSize;
	m_szReceiveBuf = new CHAR[m_lReceiveBufSize+1];
	return true;
}

BOOL CPop3::GetReceiveBufSize(long *lSize)
{
	*lSize = m_lReceiveBufSize;
	return true;
}

BOOL CPop3::SetTimeOut(DWORD dwTimeOut)
{
	m_dwTimeout = dwTimeOut;
	return true;
}

BOOL CPop3::GetTimeOut(DWORD *dwTimeOut)
{
	*dwTimeOut = m_dwTimeout;
	return true;
}


BOOL CPop3::GetLastError(CString *msg)
{
	if( !m_strLastError.IsEmpty() )
		*msg = m_strLastError;
	return true;
}

BOOL CPop3::SetLastError(CString msg)
{
	m_strLastError = msg;
	return true;
}

BOOL CPop3::Create(LPCSTR pszHostName, int nPort)
{
	struct sockaddr_in server; 
	struct hostent *hp;
	unsigned int addr;
	WSADATA wsaData;
	CString strResult;
	int nStart , nEnd;

	if( m_bSocketOK )
		Close();
	m_bSocketOK = false;
	m_bConnected = false;
	m_hThreadHandle = NULL;
	ResetEvent(m_hEventWaitingRespond);
	ResetEvent(m_hEventExitThread);

	BOOL ret = WSAStartup(MAKEWORD(2,2), &wsaData); 
	if(ret != 0) 
	{ 
		SetLastError("Initialize Socket Fail!");
		return false;  
	} 
	if( m_sPop3Socket != INVALID_SOCKET )
	{
		shutdown (m_sPop3Socket, 0x00);
		closesocket(m_sPop3Socket);
	}
	m_sPop3Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_sPop3Socket == INVALID_SOCKET)
    {
		SetLastError("Create Socket Fail!");
		goto error_return;
    }
	if(inet_addr(pszHostName)==INADDR_NONE)
	{
		hp=gethostbyname(pszHostName);
		if( NULL == hp )
		{
			SetLastError("Hostname Error!");
			goto error_return;
		}
	}
	else
	{
		addr=inet_addr(pszHostName);
		hp=gethostbyaddr((char*)&addr,sizeof(addr),AF_INET);
		if( NULL == hp )
		{
			SetLastError("Hostname Error!");
			goto error_return;
		}
	}

    server.sin_family = AF_INET;
	server.sin_port = htons(nPort); 
	server.sin_addr.s_addr=*((unsigned long*)hp->h_addr);
    if(connect(m_sPop3Socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
    {
		SetLastError("Connect POP3 Server Fail!");
		goto error_return;
    }
	else
	{
		SetLastError("Connect POP3 Server Succeed!");
	}

	//create thread to get network event
    m_hThreadHandle = (HANDLE) _beginthreadex(NULL, 0, 
				(PBEGINTHREADEX_THREADFUNC)MessageLoopThread, 
				(LPVOID)this, 0, (PBEGINTHREADEX_THREADID)&m_dwThreadId );

	m_bSocketOK = true;

	if( !GetSocketResult(&strResult , COMMAND_END_FLAG) )
		return false;

	nStart = strResult.Find('<' , 0);
	nEnd = strResult.Find('>' , nStart);

	if( nStart >= 0 && nEnd > nStart )
		m_strTimeStamp = strResult.Mid(nStart , nEnd-nStart+1 );
	else
		m_strTimeStamp = "";

	return true;

error_return:
	shutdown (m_sPop3Socket, 0x00);
	closesocket(m_sPop3Socket);
	m_sPop3Socket = INVALID_SOCKET;
	return false;
}

BOOL CPop3::Close()
{
	DisConnect();
	WaitForThreadExit();
	// Disable receiving on ServerSock.
	shutdown( m_sPop3Socket, 0x00 );
	// Close the socket.
	closesocket( m_sPop3Socket );
	WSACleanup();
	m_bSocketOK = false;
	m_bConnected = false;
	return true;
}

void CPop3::WaitForThreadExit()
{
	if( NULL == m_hThreadHandle )
		return;
	SetEvent(m_hEventExitThread);
	WaitForSingleObject(m_hThreadHandle , INFINITE);
	CloseHandle(m_hThreadHandle);
	m_hThreadHandle = NULL;
}

BOOL CPop3::Connect(LPCSTR pszUser, LPCSTR pszPassword , BOOL bAPOPAuthentication)
{
	char msg[255];
	int ret;
	CString strResult;
	CString strMD5Source;
	CString strMD5Dst;

	m_bConnected = false;
	if( false == m_bSocketOK )
	{
		SetLastError("Socket not Create!");
		return false;
	}

	if( bAPOPAuthentication )
	{
		if( m_strTimeStamp.IsEmpty() )
		{
			SetLastError("Apop error!");
			return false;
		}
		strMD5Source = m_strTimeStamp+pszPassword;
		strMD5Dst = MD5_GetMD5( (BYTE*)(const char*)strMD5Source , strMD5Source.GetLength() );
		sprintf(msg , "apop %s %s\r\n" , pszUser , strMD5Dst);
		ret = send(m_sPop3Socket , msg , strlen(msg) , NULL);
		if(ret == SOCKET_ERROR)
		{
			SetLastError("Socket error!");
			m_bSocketOK = false;
			m_bConnected = false;
			return false;
		}
		if( !GetSocketResult(&strResult , COMMAND_END_FLAG) )
			return false;
		if( 0 == strResult.Find('-' , 0) )
		{
			SetLastError("Username or Password error!");
			return false;
		}
		m_bConnected = true;

	}
	else
	{
		sprintf(msg , "user %s\r\n" , pszUser);
		ret = send(m_sPop3Socket , msg , strlen(msg) , NULL);
		if(ret == SOCKET_ERROR)
		{
			SetLastError("Socket error!");
			m_bSocketOK = false;
			m_bConnected = false;
			return false;
		}
		if( !GetSocketResult(&strResult , COMMAND_END_FLAG) )
			return false;
		if( 0 == strResult.Find('-' , 0) )
		{
			SetLastError("User Name error!");
			return false;
		}

		sprintf(msg , "pass %s\r\n", pszPassword);
		ret = send(m_sPop3Socket , msg , strlen(msg) , NULL);
		if(ret == SOCKET_ERROR)
		{
			SetLastError("Socket error!");
			m_bSocketOK = false;
			m_bConnected = false;
			return false;
		}
		if( !GetSocketResult(&strResult , COMMAND_END_FLAG) )
			return false;
		if( 0 == strResult.Find('-' , 0) )
		{
			SetLastError("User or Password error!");
			return false;
		}

		m_bConnected = true;

	}
	return true;
}

BOOL CPop3::DisConnect()
{
	CString strResult;
	char msg[255];
	int ret;
	BOOL bRet = false;

	if( false == m_bSocketOK )
	{
		SetLastError("Socket not Create!");
		return false;
	}
	if( false == m_bConnected )
	{
		SetLastError("Not Connected!");
		return false;
	}

	sprintf(msg , "quit\r\n");
	ret = send(m_sPop3Socket , msg , strlen(msg) , NULL);
	if(ret == SOCKET_ERROR)
	{
		SetLastError("Socket error!");
		bRet = false;
	}
	else
		bRet = true;
	m_bSocketOK = false;
	m_bConnected = false;
	return bRet;
}

DWORD WINAPI CPop3::MessageLoopThread(LPVOID param)
{
	int iRet;
#ifndef USE_CSTRING
	CHAR *cTmp;
#endif
	CPop3 *pto = (CPop3*)param;
	if((pto->m_hEvent[0]=WSACreateEvent())==WSA_INVALID_EVENT)
	{
		pto->SetLastError("WSACreateEvent error!");
		return 0;
	}
	pto->m_hEvent[1] = pto->m_hEventExitThread;

	//if(WSAEventSelect( pto->m_sPop3Socket,pto->m_hEvent[0],FD_READ|FD_CLOSE|FD_ACCEPT|FD_CONNECT)!=SOCKET_ERROR)
	if(WSAEventSelect( pto->m_sPop3Socket,pto->m_hEvent[0],FD_READ|FD_CLOSE)!=SOCKET_ERROR)
	{
		pto->SetLastError("Event Select ok!");
	}
	else
	{
		pto->SetLastError("Event Select Error!");
		WSACloseEvent(pto->m_hEvent[0]);
		return 0;
	}
	
	while( true )
	{
		pto->m_bReceiving = false;
		DWORD m_Flag=WSAWaitForMultipleEvents(2,pto->m_hEvent,false,INFINITE,false);
		pto->m_bReceiving = true;
		switch(m_Flag)
		{
			case WSA_WAIT_EVENT_0:
				WSANETWORKEVENTS we;
				iRet=WSAEnumNetworkEvents(pto->m_sPop3Socket,pto->m_hEvent[0],&we);
				if( we.lNetworkEvents & FD_READ )
				{					
					if( !pto->m_bSocketOK )
					{
						WSACloseEvent(pto->m_hEvent[0]);
						_endthreadex( 0 );
					}
					while( true )
					{
						memset(pto->m_szReceiveBuf , 0 , pto->m_lReceiveBufSize);
						Sleep(3);
						iRet = recv(pto->m_sPop3Socket , pto->m_szReceiveBuf , pto->m_lReceiveBufSize , NULL);					
						if(iRet == SOCKET_ERROR)
						{
							pto->m_strLastError = "Socket Read Error!";
							goto error_return;
						}
						else if( pto->m_fpTmp != NULL )//write file mode
						{
							if( 1 > fwrite(pto->m_szReceiveBuf , iRet , 1 , pto->m_fpTmp) )
							{
								pto->m_strLastError = "Write tmp file Error!";
								goto error_return;
							}
							if( iRet < pto->m_lReceiveBufSize )
								break;
							else
								continue;
						}
#ifdef USE_CSTRING						
						else
						{
							pto->m_szReceiveBuf[iRet] = '\0';
							pto->m_strReceive += pto->m_szReceiveBuf;
							if( iRet < pto->m_lReceiveBufSize )
								break;
							else
								continue;
						}
#else
						else
						{
							if( NULL != pto->m_szReceive )
							{
								try 
								{
									cTmp = new CHAR[pto->m_lReceive];
							    }	
								catch( bad_alloc err)
								{
									pto->SetLastError("Memory Error!");
									goto error_return;
								}
								memcpy(cTmp , pto->m_szReceive , pto->m_lReceive);
								delete []pto->m_szReceive;
								try 
								{
									pto->m_szReceive = new CHAR[pto->m_lReceive+iRet+1];
							    }	
								catch( bad_alloc err)
								{
									pto->SetLastError("Memory Error!");
									goto error_return;
								}
								memcpy(pto->m_szReceive , cTmp , pto->m_lReceive);
								delete []cTmp;
								memcpy(pto->m_szReceive+pto->m_lReceive , pto->m_szReceiveBuf , iRet);
							}
							else
							{
								pto->m_szReceive = new CHAR[iRet+1];
								memcpy(pto->m_szReceive , pto->m_szReceiveBuf , iRet);
							}
							pto->m_lReceive += iRet;
							pto->m_szReceive[pto->m_lReceive] = '\0';
							if( iRet < pto->m_lReceiveBufSize )
								break;
							else
								continue;
						}
#endif
					}
					if( ( 0 == _memicmp( pto->m_szReceiveBuf + iRet - lstrlen(pto->m_strEnd) , pto->m_strEnd , lstrlen(pto->m_strEnd) ) )
						&& ( lstrlen(pto->m_szReceiveBuf) >= lstrlen(pto->m_strEnd) ) )
					{//read the end flag
						SetEvent(pto->m_hEventWaitingRespond);
					}
					else if( 0 == _memicmp(pto->m_szReceiveBuf , "-ERR" , 4) )
					{
						SetEvent(pto->m_hEventWaitingRespond);
					}
					pto->CheckMessage();
				}
				else if( we.lNetworkEvents & FD_CLOSE )
				{
					pto->SetLastError("Connection Closed!");
					goto error_return;
				}
				break;
			case WSA_WAIT_EVENT_0 + 1:
				pto->SetLastError("Exit Thread!");
				goto exitthread_return;
				break;
			case WSAENETDOWN:// The network subsystem has failed. 				
				pto->SetLastError("The network subsystem has failed!");				
				goto error_return;
				break;
		}//end switchs
	}//end while
	return 0;

error_return:	
	//::MessageBox(NULL , "close RETURN" , NULL , MB_OK);
	pto->m_bSocketOK = false;
	pto->m_bConnected = false;
	SetEvent(pto->m_hEventWaitingRespond);

exitthread_return:
	WSACloseEvent(pto->m_hEvent[0]);
	pto->m_bReceiving = false;
	_endthreadex( 0 );
	return 0;
}

BOOL CPop3::GetSocketResult(CString *msgRet, CString EndFlag)
{
	BOOL bRet = false;
	BOOL bEnd = false;
	m_strEnd = EndFlag;
	HANDLE hWait[1];

	hWait[0] = m_hEventWaitingRespond;

    while (true)
    {
        MSG msg ; 

        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
        { 
            if (msg.message == WM_QUIT)  
                return true; 
            DispatchMessage(&msg); 
        } // End of PeekMessage while loop.

		switch( MsgWaitForMultipleObjects(1, hWait, FALSE, m_dwTimeout, QS_ALLINPUT) )
		{
			case WAIT_OBJECT_0:
				if( m_fpTmp != NULL )
				{
					SetLastError("Receive to file OK!");
					bRet = true;
					bEnd = true;
					break;
				}
#ifdef USE_CSTRING
				else if( m_strReceive.IsEmpty() )
#else
				else if( lstrlen(m_szReceive) <= 0 )
#endif
				{
					bRet = false;
					SetLastError("Receive Buffer is empty!");
					bEnd = true;
					break;
				}
#ifdef USE_CSTRING
				*msgRet = m_strReceive;
#else
				*msgRet = m_szReceive;
#endif
				bRet = true;
				bEnd = true;
				break;
			case WAIT_TIMEOUT:
				if( m_bReceiving )
				{
					bEnd = false;
					break;
				}
				SetLastError("Connection TimeOut!");
				bRet = false;
				bEnd = true;
				break;
		}
		if( bEnd )
			break;
    }
#ifdef USE_CSTRING
	m_strReceive = "";
#else
	if( NULL != m_szReceive )
	{
		delete []m_szReceive;
		m_szReceive = NULL;
		m_lReceive = 0;
	}
#endif
	ResetEvent(m_hEventWaitingRespond);
	return bRet;
}

BOOL CPop3::CheckMessage()
{
	MSG msg;
	while (PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)) 
	{
		if (msg.message==WM_QUIT)
			return false;
		else 
		{
			GetMessage(&msg,NULL,0,0);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return true;
}

BOOL CPop3::GetMailList(CStringList *strResultList)
{
	CString strResult, strTemp;
	char msg[255];
	int ret,nStart,nEnd;

	if( false == m_bSocketOK )
	{
		SetLastError("Socket not Create!");
		return false;
	}

	if( false == m_bConnected )
	{
		SetLastError("Not Connected!");
		return false;
	}

	strcpy(msg , "list\r\n");
	ret = send(m_sPop3Socket , msg , strlen(msg) , NULL);
	if(ret == SOCKET_ERROR)
	{
		SetLastError("Socket error!");
		m_bSocketOK = false;
		m_bConnected = false;
		return false;
	}

	if( !GetSocketResult(&strResult , DATA_END_FLAG) )
		return false;
	if( 0 == strResult.Find('-' , 0) )
	{
		SetLastError("Get Mail List error!");
		return false;
	}

	nStart = nEnd = 0;
	strResultList->RemoveAll();
	
	nEnd = strResult.Find(COMMAND_END_FLAG , nStart);
	if( nEnd >= 0 )
		nStart = nEnd + 2;
	
	while( true )
	{
		nEnd = strResult.Find(COMMAND_END_FLAG , nStart);
		if( -1 == nEnd )
		{
			break;
		}
		else
		{
			strTemp = strResult.Mid(nStart , nEnd-nStart);
			nStart = nEnd + 2;
			if( 0 == strTemp.Compare(".") )
				continue;
			strResultList->AddTail(strTemp);
		}
	}

	return true;
}

BOOL CPop3::GetMailHeader(int nMailIndex, CString *strMailHeader)
{
	CString strResult, strTemp;
	char msg[255];
	int ret;

	if( false == m_bSocketOK )
	{
		SetLastError("Socket not Create!");
		return false;
	}
	if( false == m_bConnected )
	{
		SetLastError("Not Connected!");
		return false;
	}

	sprintf(msg , "top %d 1\r\n" , nMailIndex);
	ret = send(m_sPop3Socket , msg , strlen(msg) , NULL);
	if(ret == SOCKET_ERROR)
	{
		SetLastError("Socket error!");
		m_bSocketOK = false;
		m_bConnected = false;
		return false;
	}

	if( !GetSocketResult(&strResult , DATA_END_FLAG) )
		return false;
	if( 0 == strResult.Find('-' , 0) )
	{
		SetLastError("Get Mail Header error!");
		return false;
	}
	*strMailHeader = strResult;
	return true;

}

BOOL CPop3::GetMail(int nMailIndex, CString *strMail)
{
	CString strResult, strTemp;
	char msg[255];
	int ret;

	if( false == m_bSocketOK )
	{
		SetLastError("Socket not Create!");
		return false;
	}
	if( false == m_bConnected )
	{
		SetLastError("Not Connected!");
		return false;
	}

	sprintf(msg , "retr %d\r\n" , nMailIndex);
	ret = send(m_sPop3Socket , msg , strlen(msg) , 0);
	if(ret == SOCKET_ERROR)
	{
		SetLastError("Socket error!");
		m_bSocketOK = false;
		m_bConnected = false;
		return false;
	}

	if( !GetSocketResult(&strResult , DATA_END_FLAG) )
		return false;
	if( 0 == strResult.Find('-' , 0) )
	{
		SetLastError("Get Mail error!");
		return false;
	}
	*strMail = strResult;
	return true;

}

BOOL CPop3::GetMail(int nMailIndex, LPCSTR tmpfilename)
{
	CString strResult, strTemp;
	char msg[255];
	int ret;
	BOOL bRet = false;

	if( false == m_bSocketOK )
	{
		SetLastError("Socket not Create!");
		return false;
	}
	if( false == m_bConnected )
	{
		SetLastError("Not Connected!");
		return false;
	}

	m_fpTmp = fopen(tmpfilename , "wb");
	if( m_fpTmp == NULL )
	{
		SetLastError("Open Tmp File Error!");
		return false;
	}

	sprintf(msg , "retr %d\r\n" , nMailIndex);
	ret = send(m_sPop3Socket , msg , strlen(msg) , NULL);
	if(ret == SOCKET_ERROR)
	{
		SetLastError("Socket error!");
		m_bSocketOK = false;
		m_bConnected = false;
		return false;
	}

	if( !GetSocketResult(&strResult , DATA_END_FLAG) )
		bRet = false;
	else
		bRet = true;
	if( 0 == strResult.Find('-' , 0) )
	{
		SetLastError("Get Mail error!");
		bRet = false;
	}
	fclose(m_fpTmp);
	m_fpTmp = NULL;
	return bRet;
}


BOOL CPop3::GetStat(CString *strStat)
{
	CString strResult;
	char msg[255];
	int ret , nStart , nEnd;

	if( false == m_bSocketOK )
	{
		SetLastError("Socket not Create!");
		return false;
	}
	if( false == m_bConnected )
	{
		SetLastError("Not Connected!");
		return false;
	}

	strcpy(msg , "stat\r\n");
	ret = send(m_sPop3Socket , msg , strlen(msg) , NULL);
	if(ret == SOCKET_ERROR)
	{
		SetLastError("Socket error!");
		m_bSocketOK = false;
		m_bConnected = false;
		return false;
	}

	if( !GetSocketResult(&strResult , COMMAND_END_FLAG) )
		return false;
	if( 0 == strResult.Find('-' , 0) )
	{
		SetLastError("Get Stat error!");
		return false;
	}
	nStart = 4;
	nEnd = strResult.GetLength() - 2;
	*strStat = strResult.Mid(nStart , nEnd - nStart);

	return true;
}

BOOL CPop3::GetMailSize(int nMailIndex, long *lSize)
{
	CString strResult, strTemp;
	char msg[255];
	int ret , nStart , nEnd;

	if( false == m_bSocketOK )
	{
		SetLastError("Socket not Create!");
		return false;
	}
	if( false == m_bConnected )
	{
		SetLastError("Not Connected!");
		return false;
	}

	sprintf(msg , "list %d\r\n" , nMailIndex);
	ret = send(m_sPop3Socket , msg , strlen(msg) , NULL);
	if(ret == SOCKET_ERROR)
	{
		SetLastError("Socket error!");
		m_bSocketOK = false;
		m_bConnected = false;
		return false;
	}

	if( !GetSocketResult(&strResult , COMMAND_END_FLAG) )
		return false;
	if( 0 == strResult.Find('-' , 0) )
	{
		SetLastError("Get Size error!");
		return false;
	}
	nStart = strResult.Find(' ' , 4)+1;
	nEnd = strResult.GetLength()-2;
	strTemp = strResult.Mid(nStart , nEnd-nStart);
	*lSize = atol(strTemp.GetBuffer(strTemp.GetLength()));

	return true;

}

BOOL CPop3::GetMailSubject(int nMailIndex, CString *strSubject)
{
	CString strMailHeader;
	int nSubStart , nSubEnd;
	if( !GetMailHeader(nMailIndex , &strMailHeader) )
	{
		//SetLastError("Get Subject error!");
		return false;
	}
	nSubStart = strMailHeader.Find("Subject:");
	if( -1 == nSubStart )
	{
		SetLastError("No Subject!");
		return false;
	}
	nSubEnd = strMailHeader.Find("\r\n" , nSubStart);
	*strSubject = strMailHeader.Mid(nSubStart + 9 , nSubEnd - (nSubStart + 9) );
	return true;
}

BOOL CPop3::GetMailSender(int nMailIndex, CString *strSender)
{
	CString strMailHeader;
	int nSubStart , nSubEnd;
	if( !GetMailHeader(nMailIndex , &strMailHeader) )
	{
		//SetLastError("Get Sender error!");
		return false;
	}
	nSubStart = strMailHeader.Find("From:");
	if( -1 == nSubStart )
	{
		SetLastError("No Sender!");
		return false;
	}
	nSubEnd = strMailHeader.Find("\r\n" , nSubStart);
	*strSender = strMailHeader.Mid(nSubStart + 6 , nSubEnd - (nSubStart + 6) );
	return true;
}

BOOL CPop3::GetMailReceiver(int nMailIndex, CString *strReceiver)
{
	CString strMailHeader;
	int nSubStart , nSubEnd;
	if( !GetMailHeader(nMailIndex , &strMailHeader) )
	{
		//SetLastError("Get Receiver error!");
		return false;
	}
	nSubStart = strMailHeader.Find("\nTo:");
	if( -1 == nSubStart )
	{
		SetLastError("No Receiver!");
		return false;
	}
	nSubEnd = strMailHeader.Find("\r\n" , nSubStart);
	*strReceiver = strMailHeader.Mid(nSubStart + 5 , nSubEnd - (nSubStart + 5) );
	return true;
}

BOOL CPop3::GetMailDate(int nMailIndex, CString *strDate)
{
	CString strMailHeader;
	int nSubStart , nSubEnd;
	if( !GetMailHeader(nMailIndex , &strMailHeader) )
	{
		//SetLastError("Get Date error!");
		return false;
	}
	nSubStart = strMailHeader.Find("Date:");
	if( -1 == nSubStart )
	{
		SetLastError("No Date!");
		return false;
	}
	nSubEnd = strMailHeader.Find("\r\n" , nSubStart);
	*strDate = strMailHeader.Mid(nSubStart + 6 , nSubEnd - (nSubStart + 6) );
	return true;
}

BOOL CPop3::DeleteMail(int nMailIndex)
{
	CString strResult;
	char msg[255];
	int ret;

	if( false == m_bSocketOK )
	{
		SetLastError("Socket not Create!");
		return false;
	}
	if( false == m_bConnected )
	{
		SetLastError("Not Connected!");
		return false;
	}

	sprintf(msg , "dele %d\r\n" , nMailIndex);
	ret = send(m_sPop3Socket , msg , strlen(msg) , NULL);
	if(ret == SOCKET_ERROR)
	{
		SetLastError("Socket error!");
		m_bSocketOK = false;
		m_bConnected = false;
		return false;
	}

	if( !GetSocketResult(&strResult , COMMAND_END_FLAG) )
		return false;
	if( 0 == strResult.Find('-' , 0) )
	{
		SetLastError("Delete Mail error!");
		return false;
	}

	return true;

}

BOOL CPop3::ResetMail()
{
	CString strResult;
	char msg[255];
	int ret;

	if( false == m_bSocketOK )
	{
		SetLastError("Socket not Create!");
		return false;
	}
	if( false == m_bConnected )
	{
		SetLastError("Not Connected!");
		return false;
	}

	sprintf(msg , "rset\r\n");
	ret = send(m_sPop3Socket , msg , strlen(msg) , NULL);
	if(ret == SOCKET_ERROR)
	{
		SetLastError("Socket error!");
		m_bSocketOK = false;
		m_bConnected = false;
		return false;
	}

	if( !GetSocketResult(&strResult , COMMAND_END_FLAG) )
		return false;
	if( 0 == strResult.Find('-' , 0) )
	{
		SetLastError("Reset Mail error!");
		return false;
	}

	return true;

}


BOOL CPop3::IsConnected()
{
	if( m_bConnected )
		return true;
	else
		return false;
}


/*********************************************************************************************
/* Function for MD5 start
**********************************************************************************************/
CString CPop3::MD5_GetMD5(BYTE* pBuf, UINT nLength)
{
	//entry invariants
	AfxIsValidAddress(pBuf,nLength,FALSE);

	//calculate and return the checksum
	//CMD5Checksum MD5Checksum;
	MD5_Init();
	MD5_Update( pBuf, nLength );
	return MD5_Final();
}


DWORD CPop3::MD5_RotateLeft(DWORD x, int n)
{
	//check that DWORD is 4 bytes long - true in Visual C++ 6 and 32 bit Windows
	ASSERT( sizeof(x) == 4 );

	//rotate and return x
	return (x << n) | (x >> (32-n));
}

void CPop3::MD5_FF( DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T)
{
	DWORD F = (B & C) | (~B & D);
	A += F + X + T;
	A = MD5_RotateLeft(A, S);
	A += B;
}

void CPop3::MD5_GG( DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T)
{
	DWORD G = (B & D) | (C & ~D);
	A += G + X + T;
	A = MD5_RotateLeft(A, S);
	A += B;
}

void CPop3::MD5_HH( DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T)
{
	DWORD H = (B ^ C ^ D);
	A += H + X + T;
	A = MD5_RotateLeft(A, S);
	A += B;
}

void CPop3::MD5_II( DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T)
{
	DWORD I = (C ^ (B | ~D));
	A += I + X + T;
	A = MD5_RotateLeft(A, S);
	A += B;
}

void CPop3::MD5_ByteToDWord(DWORD* Output, BYTE* Input, UINT nLength)
{
	//entry invariants
	ASSERT( nLength % 4 == 0 );
	ASSERT( AfxIsValidAddress(Output, nLength/4, TRUE) );
 	ASSERT( AfxIsValidAddress(Input, nLength, FALSE) );

	//initialisations
	UINT i=0;	//index to Output array
	UINT j=0;	//index to Input array

	//transfer the data by shifting and copying
	for ( ; j < nLength; i++, j += 4)
	{
		Output[i] = (ULONG)Input[j]			| 
					(ULONG)Input[j+1] << 8	| 
					(ULONG)Input[j+2] << 16 | 
					(ULONG)Input[j+3] << 24;
	}
}

void CPop3::MD5_Transform(BYTE Block[64])
{
	//initialise local data with current checksum
	ULONG a = m_lMD5[0];
	ULONG b = m_lMD5[1];
	ULONG c = m_lMD5[2];
	ULONG d = m_lMD5[3];

	//copy BYTES from input 'Block' to an array of ULONGS 'X'
	ULONG X[16];
	MD5_ByteToDWord( X, Block, 64 );

	//Perform Round 1 of the transformation
	MD5_FF (a, b, c, d, X[ 0], MD5_S11, MD5_T01); 
	MD5_FF (d, a, b, c, X[ 1], MD5_S12, MD5_T02); 
	MD5_FF (c, d, a, b, X[ 2], MD5_S13, MD5_T03); 
	MD5_FF (b, c, d, a, X[ 3], MD5_S14, MD5_T04); 
	MD5_FF (a, b, c, d, X[ 4], MD5_S11, MD5_T05); 
	MD5_FF (d, a, b, c, X[ 5], MD5_S12, MD5_T06); 
	MD5_FF (c, d, a, b, X[ 6], MD5_S13, MD5_T07); 
	MD5_FF (b, c, d, a, X[ 7], MD5_S14, MD5_T08); 
	MD5_FF (a, b, c, d, X[ 8], MD5_S11, MD5_T09); 
	MD5_FF (d, a, b, c, X[ 9], MD5_S12, MD5_T10); 
	MD5_FF (c, d, a, b, X[10], MD5_S13, MD5_T11); 
	MD5_FF (b, c, d, a, X[11], MD5_S14, MD5_T12); 
	MD5_FF (a, b, c, d, X[12], MD5_S11, MD5_T13); 
	MD5_FF (d, a, b, c, X[13], MD5_S12, MD5_T14); 
	MD5_FF (c, d, a, b, X[14], MD5_S13, MD5_T15); 
	MD5_FF (b, c, d, a, X[15], MD5_S14, MD5_T16); 

	//Perform Round 2 of the transformation
	MD5_GG (a, b, c, d, X[ 1], MD5_S21, MD5_T17); 
	MD5_GG (d, a, b, c, X[ 6], MD5_S22, MD5_T18); 
	MD5_GG (c, d, a, b, X[11], MD5_S23, MD5_T19); 
	MD5_GG (b, c, d, a, X[ 0], MD5_S24, MD5_T20); 
	MD5_GG (a, b, c, d, X[ 5], MD5_S21, MD5_T21); 
	MD5_GG (d, a, b, c, X[10], MD5_S22, MD5_T22); 
	MD5_GG (c, d, a, b, X[15], MD5_S23, MD5_T23); 
	MD5_GG (b, c, d, a, X[ 4], MD5_S24, MD5_T24); 
	MD5_GG (a, b, c, d, X[ 9], MD5_S21, MD5_T25); 
	MD5_GG (d, a, b, c, X[14], MD5_S22, MD5_T26); 
	MD5_GG (c, d, a, b, X[ 3], MD5_S23, MD5_T27); 
	MD5_GG (b, c, d, a, X[ 8], MD5_S24, MD5_T28); 
	MD5_GG (a, b, c, d, X[13], MD5_S21, MD5_T29); 
	MD5_GG (d, a, b, c, X[ 2], MD5_S22, MD5_T30); 
	MD5_GG (c, d, a, b, X[ 7], MD5_S23, MD5_T31); 
	MD5_GG (b, c, d, a, X[12], MD5_S24, MD5_T32); 

	//Perform Round 3 of the transformation
	MD5_HH (a, b, c, d, X[ 5], MD5_S31, MD5_T33); 
	MD5_HH (d, a, b, c, X[ 8], MD5_S32, MD5_T34); 
	MD5_HH (c, d, a, b, X[11], MD5_S33, MD5_T35); 
	MD5_HH (b, c, d, a, X[14], MD5_S34, MD5_T36); 
	MD5_HH (a, b, c, d, X[ 1], MD5_S31, MD5_T37); 
	MD5_HH (d, a, b, c, X[ 4], MD5_S32, MD5_T38); 
	MD5_HH (c, d, a, b, X[ 7], MD5_S33, MD5_T39); 
	MD5_HH (b, c, d, a, X[10], MD5_S34, MD5_T40); 
	MD5_HH (a, b, c, d, X[13], MD5_S31, MD5_T41); 
	MD5_HH (d, a, b, c, X[ 0], MD5_S32, MD5_T42); 
	MD5_HH (c, d, a, b, X[ 3], MD5_S33, MD5_T43); 
	MD5_HH (b, c, d, a, X[ 6], MD5_S34, MD5_T44); 
	MD5_HH (a, b, c, d, X[ 9], MD5_S31, MD5_T45); 
	MD5_HH (d, a, b, c, X[12], MD5_S32, MD5_T46); 
	MD5_HH (c, d, a, b, X[15], MD5_S33, MD5_T47); 
	MD5_HH (b, c, d, a, X[ 2], MD5_S34, MD5_T48); 

	//Perform Round 4 of the transformation
	MD5_II (a, b, c, d, X[ 0], MD5_S41, MD5_T49); 
	MD5_II (d, a, b, c, X[ 7], MD5_S42, MD5_T50); 
	MD5_II (c, d, a, b, X[14], MD5_S43, MD5_T51); 
	MD5_II (b, c, d, a, X[ 5], MD5_S44, MD5_T52); 
	MD5_II (a, b, c, d, X[12], MD5_S41, MD5_T53); 
	MD5_II (d, a, b, c, X[ 3], MD5_S42, MD5_T54); 
	MD5_II (c, d, a, b, X[10], MD5_S43, MD5_T55); 
	MD5_II (b, c, d, a, X[ 1], MD5_S44, MD5_T56); 
	MD5_II (a, b, c, d, X[ 8], MD5_S41, MD5_T57); 
	MD5_II (d, a, b, c, X[15], MD5_S42, MD5_T58); 
	MD5_II (c, d, a, b, X[ 6], MD5_S43, MD5_T59); 
	MD5_II (b, c, d, a, X[13], MD5_S44, MD5_T60); 
	MD5_II (a, b, c, d, X[ 4], MD5_S41, MD5_T61); 
	MD5_II (d, a, b, c, X[11], MD5_S42, MD5_T62); 
	MD5_II (c, d, a, b, X[ 2], MD5_S43, MD5_T63); 
	MD5_II (b, c, d, a, X[ 9], MD5_S44, MD5_T64); 

	//add the transformed values to the current checksum
	m_lMD5[0] += a;
	m_lMD5[1] += b;
	m_lMD5[2] += c;
	m_lMD5[3] += d;
}

void CPop3::MD5_Init()
{
	// zero members
	memset( m_lpszBuffer, 0, 64 );
	m_nCount[0] = m_nCount[1] = 0;

	// Load magic state initialization constants
	m_lMD5[0] = MD5_INIT_STATE_0;
	m_lMD5[1] = MD5_INIT_STATE_1;
	m_lMD5[2] = MD5_INIT_STATE_2;
	m_lMD5[3] = MD5_INIT_STATE_3;
}

void CPop3::MD5_DWordToByte(BYTE* Output, DWORD* Input, UINT nLength )
{
	//entry invariants
	ASSERT( nLength % 4 == 0 );
	ASSERT( AfxIsValidAddress(Output, nLength, TRUE) );
	ASSERT( AfxIsValidAddress(Input, nLength/4, FALSE) );

	//transfer the data by shifting and copying
	UINT i = 0;
	UINT j = 0;
	for ( ; j < nLength; i++, j += 4) 
	{
		Output[j] =   (UCHAR)(Input[i] & 0xff);
		Output[j+1] = (UCHAR)((Input[i] >> 8) & 0xff);
		Output[j+2] = (UCHAR)((Input[i] >> 16) & 0xff);
		Output[j+3] = (UCHAR)((Input[i] >> 24) & 0xff);
	}
}

CString CPop3::MD5_Final()
{
	//Save number of bits
	BYTE Bits[8];
	MD5_DWordToByte( Bits, m_nCount, 8 );

	//Pad out to 56 mod 64.
	UINT nIndex = (UINT)((m_nCount[0] >> 3) & 0x3f);
	UINT nPadLen = (nIndex < 56) ? (56 - nIndex) : (120 - nIndex);
	MD5_Update( PADDING, nPadLen );

	//Append length (before padding)
	MD5_Update( Bits, 8 );

	//Store final state in 'lpszMD5'
	const int nMD5Size = 16;
	unsigned char lpszMD5[ nMD5Size ];
	MD5_DWordToByte( lpszMD5, m_lMD5, nMD5Size );

	//Convert the hexadecimal checksum to a CString
	CString strMD5;
	for ( int i=0; i < nMD5Size; i++) 
	{
		CString Str;
		if (lpszMD5[i] == 0) {
			Str = CString("00");
		}
		else if (lpszMD5[i] <= 15) 	{
			Str.Format("0%x",lpszMD5[i]);
		}
		else {
			Str.Format("%x",lpszMD5[i]);
		}

		ASSERT( Str.GetLength() == 2 );
		strMD5 += Str;
	}
	ASSERT( strMD5.GetLength() == 32 );
	return strMD5;
}

void CPop3::MD5_Update( BYTE* Input,	ULONG nInputLen )
{
	//Compute number of bytes mod 64
	UINT nIndex = (UINT)((m_nCount[0] >> 3) & 0x3F);

	//Update number of bits
	if ( ( m_nCount[0] += nInputLen << 3 )  <  ( nInputLen << 3) )
	{
		m_nCount[1]++;
	}
	m_nCount[1] += (nInputLen >> 29);

	//Transform as many times as possible.
	UINT i=0;		
	UINT nPartLen = 64 - nIndex;
	if (nInputLen >= nPartLen) 	
	{
		memcpy( &m_lpszBuffer[nIndex], Input, nPartLen );
		MD5_Transform( m_lpszBuffer );
		for (i = nPartLen; i + 63 < nInputLen; i += 64) 
		{
			MD5_Transform( &Input[i] );
		}
		nIndex = 0;
	} 
	else 
	{
		i = 0;
	}

	// Buffer remaining input
	memcpy( &m_lpszBuffer[nIndex], &Input[i], nInputLen-i);
}
/*********************************************************************************************
/* Function for MD5 end
**********************************************************************************************/

