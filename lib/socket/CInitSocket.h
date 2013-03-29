#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32")

class CInitSock
{
public:
	CInitSock(BYTE minorVer=2,BYTE majorVer=2)
	{
		WSADATA wsaData;
    	WORD sockVersion=MAKEWORD(minorVer,majorVer);
		if (WSAStartup(sockVersion,&wsaData)!=0)
		{
			exit(0);
		}
		if ( LOBYTE( wsaData.wVersion ) != minorVer || HIBYTE( wsaData.wVersion ) != majorVer ) 
		{
			WSACleanup( );
			exit(0);
		}
	}
	~CInitSock()
	{
		::WSACleanup();
	}
};

