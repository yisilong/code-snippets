#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include "MyDate.h"
#include "sqlite3.h"
#pragma comment(lib,"sqlite3.lib")
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
using namespace std;
#include <Dbghelp.h>
#pragma comment(lib,"Dbghelp.lib")
bool insurePathExits(string strPath);
int main()
{
	sqlite3* pDB;
	if (SQLITE_OK != sqlite3_open("data.db",&pDB))
	{
		cout << "count'n open data.db\n";
		return 0;
	}

	CMyDate date(2012,10,24);
	int nTotal = 8897 - 100;
	int nStart = 100;
	char ch[101] = {0};
	while (nTotal > 0)
	{
		sprintf_s(ch,100,"SELECT id,torrent,torrentdata,img,imgdata from data limit %d,100;",nStart);
		sqlite3_stmt* stmt;
		if (SQLITE_OK != sqlite3_prepare(pDB,ch,strlen(ch),&stmt,NULL))
		{
			cout << "sqlite3_prepare: " << sqlite3_errmsg(pDB) << endl;
			return 0;
		}
		string strImgDir = "img\\" + date.GetFormat() + "\\";
		string strTorrentDir = "torrent\\" + date.GetFormat() + "\\";
		insurePathExits(strImgDir);
		insurePathExits(strTorrentDir);
		fstream s(strImgDir + "img.txt",ios::out | ios::binary | ios::trunc);
		fstream ss(strTorrentDir + "torrent.txt",ios::out | ios::binary | ios::trunc);
		while (SQLITE_ROW == sqlite3_step(stmt))
		{		
			int nId = sqlite3_column_int(stmt,0);// id
			
			const unsigned char* szTorrent = sqlite3_column_text(stmt,1);// torrent
			
			const void* pTdata = sqlite3_column_blob(stmt,2);// torrentdata
			int nTLen =  sqlite3_column_bytes(stmt,2);
			
			const unsigned char* szImg = sqlite3_column_text(stmt,3);// img
			
			const void* pIdata = sqlite3_column_blob(stmt,4);// imgdata
			int nILen = sqlite3_column_bytes(stmt,4);

			string strImgName = (char*)szImg;
			int nStart = strImgName.rfind("/");
			strImgName = strImgName.substr(nStart+1);
			strImgName = strImgDir + strImgName;

			string strTorrentName = (char*)szTorrent;
			nStart = strTorrentName.rfind("/");
			strTorrentName = strTorrentName.substr(nStart+1);
			strTorrentName = strTorrentDir + strTorrentName;

			fstream outStream(strImgName,ios::binary | ios::out | ios::trunc);
			outStream.write((char*)pIdata,nILen);
			outStream.close();
			outStream.open(strTorrentName,ios::binary | ios::out | ios::trunc);
			outStream.write((char*)pTdata,nTLen);
			outStream.close();

			s << szImg << endl;
			ss << szTorrent << endl;
		}
		sqlite3_finalize(stmt);
		s.close();
		ss.close();
		date.AddDay();
		nStart += 100;
		nTotal -= 100;
		cout << "nTotal: " << nTotal << endl;
		cout << "nStart: " << nStart << endl;
		cout << ch << endl << endl;
	}
	sqlite3_close(pDB);
	system("pause");
	return 0;
}

bool insurePathExits(string strPath)
{
// 	if (!PathFileExistsA(strPath.c_str()))
// 	{
// 
// 	}
	MakeSureDirectoryPathExists(strPath.c_str());
	return true;
}