#include "MyDate.h"
#include <iostream>
#include <Windows.h>
#include <string>
#include <sstream>
#include <fstream>
#include <process.h>
#include <vector>
using namespace std;
#include "sqlite3.h"
#pragma comment(lib,"sqlite3.lib")
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib")
unsigned int __stdcall myThread(void* pParam);
int myCallBack(void* pParam,int nNum,char** pValues,char** pNames);
bool FetchUrl(string strUrl,string& strContent,size_t& nByteLen);
vector<string> vecId;
vector<string> vecTorrent;
vector<string> vecTorrent1;
vector<string> vecTorrent2;
const int nThreadNum = 10;
CRITICAL_SECTION g_csIndex;
CRITICAL_SECTION g_csCout;
int main()
{
	WSADATA wsaData = {0};
	if(WSAStartup(MAKEWORD(2,2), &wsaData))
	{
		cout << "套接字初始化失败!" << endl;
		return 0;
	}
	sqlite3* db;
	if (SQLITE_OK != sqlite3_open("btscg.db",&db))
	{
		cout << "sqlite_open failed" << endl;
		return 0;
	}
	
	if (SQLITE_OK != sqlite3_exec(db,"select id,torrent,torrentshorturl1,torrentshorturl2 from movie;",myCallBack,NULL,NULL))
	{
		sqlite3_close(db);
		cout << "获取记录集失败" << endl;
		return 0;
	}
	sqlite3_close(db);
	
	// 矩阵链接
	int nHundred = 0;
	CMyDate data(2012,10,23);
	int nSize = vecId.size();
	for (int i = 0; i < nSize; ++i,++nHundred)
	{
		if (!vecTorrent[i].empty())
		{
			int nBegin = vecTorrent[i].rfind("/");
			vecTorrent[i] = "http://messycode.aliapp.com/ixxoo/" + data.GetFormat() + vecTorrent[i].substr(nBegin);
		}
		if (nHundred == 100)
		{
			data.AddDay();
			nHundred = 0;
		}
	}

	// 多线程执行任务
	int nStep = (nSize + nThreadNum - 1)/nThreadNum;
	HANDLE handles[nThreadNum];
	InitializeCriticalSection(&g_csIndex);
	InitializeCriticalSection(&g_csCout);
	for (int i = 0; i < nThreadNum; i++)
	{
		handles[i] = (HANDLE)_beginthreadex(NULL,0,myThread,NULL,0,NULL);
	}
	DWORD dwRes = WaitForMultipleObjects(nThreadNum,handles,TRUE,INFINITE);
	DeleteCriticalSection(&g_csCout);
	DeleteCriticalSection(&g_csIndex);
	for (int i = 0; i < nThreadNum; i++)
	{
		CloseHandle(handles[i]);
	}

	cout << "任务完成" << endl;
	WSACleanup();
	system("pause");
	return 0;
}

int myCallBack(void* pParam,int nNum,char** pValues,char** pNames)
{
	vecId.push_back(pValues[0]);
	vecTorrent.push_back(pValues[1]);
	vecTorrent1.push_back(pValues[2]);
	vecTorrent2.push_back(pValues[3]);
	return 0;
}

bool FetchUrl(string strUrl,string& strContent,size_t& nByteLen)
{
	size_t nStart = strUrl.find("http://");
	if (nStart != string::npos)
	{
		strUrl = strUrl.replace(0,strlen("http://"),"");
	}

	string strHost;
	string strRequest;
	nStart = strUrl.find_first_of('/');
	if (nStart == string::npos)
	{
		strHost = strUrl;
		strRequest = "/";
	}
	else
	{
		strHost = strUrl.substr(0,nStart);
		strRequest = strUrl.substr(nStart);
	}

	// connect to server
	SOCKET sock = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		cout<<"socket create failed" << endl;
		return false;
	}
	string strHeader = "GET ";
	strHeader += strRequest + " HTTP/1.1";
	strHeader += "\r\nHost: ";
	strHeader += strHost.c_str();
	strHeader += "\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:15.0) Gecko/20100101 Firefox/15.0.1";
	strHeader += "\r\nAccept: */*";
	strHeader += "\r\nConnection: close\r\n\r\n";

	int nPort = 8087;
	string strHostName = "127.0.0.1";//strHost;//

 	PHOSTENT pURL = gethostbyname(strHostName.c_str());

	SOCKADDR_IN addrUrl;
	addrUrl.sin_addr.s_addr = *((unsigned long*)pURL->h_addr);
	addrUrl.sin_family = AF_INET;
	addrUrl.sin_port = htons(nPort);
	if (SOCKET_ERROR == connect(sock,reinterpret_cast<SOCKADDR*>(&addrUrl),sizeof(addrUrl)))
	{
		cout << "socket connect failed" << endl;
		closesocket(sock);
		return false;
	}

	if (SOCKET_ERROR == send(sock,strHeader.c_str(),strHeader.length(),0))
	{
		cout << "socket send failed" << endl;
		closesocket(sock);
		return false;
	}

	string strSrcHtml;
	size_t nTotleRead = 0;
	int nRead = 0;
	char szTmpRecv[4096+1] = {0};
	while ((nRead = recv(sock,szTmpRecv,4096,0)) > 0)
	{
		strSrcHtml.append(szTmpRecv,szTmpRecv+nRead);
		memset(szTmpRecv,'\0',sizeof(szTmpRecv));
		nTotleRead += nRead;
	}
	if (nTotleRead == 0)
	{
		return false;
	}

	// 成功
	nStart = strSrcHtml.find("HTTP/1.1 200");
	if (nStart == string::npos)
	{
		return false;
	}
	nStart = strSrcHtml.find("\r\n\r\n");
	if (nStart == string::npos)
	{
		return false;
	}
	nStart += strlen("\r\n\r\n");
	strContent.assign(strSrcHtml.begin() + nStart,strSrcHtml.end());
	nByteLen = nTotleRead - nStart;
	return true;
}

unsigned int __stdcall myThread(void* pParam)
{
	static int nTotalSize = vecId.size();
	static int nTasksPer = (nTotalSize + nThreadNum - 1) / nThreadNum;
	static int sBegin = -1;
	EnterCriticalSection(&g_csIndex);
	sBegin++;
	int nBegin = sBegin*nTasksPer;
	LeaveCriticalSection(&g_csIndex);
	int nEnd = nBegin + nTasksPer;
	if (nEnd > nTotalSize)
	{
		nEnd = nTotalSize;
	}
	stringstream s;
	s << "failed" << nBegin << ".txt";
	fstream outStream(s.str(),ios::out | ios::binary | ios::trunc);
	string strContent;
	size_t nByteLen;
	string strApi[2] = {"http://api.adf.ly/api.php?key=6e8b842d19d03237221d0ee23058cd1f&uid=2688008&advert_type=int&domain=adf.ly&url=","http://api.adf.ly/api.php?key=6e8b842d19d03237221d0ee23058cd1f&uid=2688008&advert_type=int&domain=q.gs&url="};
	string strSql = "begin;";
	for (int i= nBegin; i < nEnd; ++i)
	{
		string str[2];
		bool bFailed = false;
		if (!vecTorrent[i].empty())
		{
			if (vecTorrent1[i].empty())
			{
				if (!FetchUrl(strApi[0] + vecTorrent[i],strContent,nByteLen) || strContent == "error")
				{
					strContent = "";
					bFailed = true;
				}
				str[0] = strContent;			
			}

			if (vecTorrent2[i].empty())
			{
				if (!FetchUrl(strApi[1] + vecTorrent[i],strContent,nByteLen) || strContent == "error")
				{
					strContent = "";
					bFailed = true;
				}
				str[1] = strContent;			
			}
		}
		if (bFailed)
		{
			outStream << vecId[i] << endl;
		}

		if (!str[0].empty())
		{
			strSql += "update movie set torrentshorturl1='" + str[0] + "' where id=" + vecId[i] + ";";
		}
		if (!str[1].empty())
		{
			strSql += "update movie set torrentshorturl2='" + str[1] + "' where id=" + vecId[i] + ";";
		}
		EnterCriticalSection(&g_csCout);
		cout << i << endl;
		LeaveCriticalSection(&g_csCout);
	}
	strSql += "commit;";
	outStream.close();

	s << ".sql";
	outStream.open(s.str(),ios::out | ios::binary | ios::trunc);
	outStream << strSql;
	outStream.close();

	// 更新数据库
	EnterCriticalSection(&g_csIndex);
	sqlite3* db;
	if (SQLITE_OK != sqlite3_open("btscg.db",&db))
	{
		cout << nBegin << "-" << nEnd << ": sqlite_open failed" << endl;
	}
	if (SQLITE_OK != sqlite3_exec(db,strSql.c_str(),NULL,NULL,NULL))
	{
		cout << nBegin << "-" << nEnd << "更新失败" << endl;
	}
	sqlite3_close(db);
	LeaveCriticalSection(&g_csIndex);
	return 0;
}