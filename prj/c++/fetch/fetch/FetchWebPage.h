#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;
#include <Windows.h>
#include "sqlite3.h"
#pragma comment(lib,"sqlite3.lib")
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib")
#include "Base64.h"
class CFetchWebPage
{
private:
	vector<string> m_UrlsVec;
	vector<string>::size_type m_nBaseIndex;
	CRITICAL_SECTION m_csIndex;
	CRITICAL_SECTION m_csDb;
	CRITICAL_SECTION m_csFile;
	int m_nInterval;
	CBase64 m_base64;
public:
	CFetchWebPage();
	~CFetchWebPage();
	bool Run();
private:
	bool FetchUrl(string strUrl,string& strContent,size_t& nByteLen);
	bool ParseFormContent(string& strContent,string& strSql);
	bool ParseListContent(string& strList);
	static int myCallback(void* para,int ncolumn,char** column_value,char** column_name);
	bool ParsePageContent(sqlite3* db,string& strContent,const string& strUrl,string& strSql,string& strPrint);
	bool FilterContent(string& strText);
	static unsigned int WINAPI myThread(void* pVoid);
	unsigned int thread_run();
	vector<string>::size_type GetBaseIndex();
};
