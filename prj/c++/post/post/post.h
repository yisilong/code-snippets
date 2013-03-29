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
typedef struct __tag_data
{
	string strTagId;
	string strName;
	string strCount;
	string strSlug;
// 	string strHtmlUrl;
// 	string strRssUrl;
	void SetAttribute(string strKey,string strValue);
}TAG_DATA;
typedef struct __category_data
{
	string strCategoryId;
	string strParentId;
	string strDescription;
	string strCategoryDescription;
	string strCategoryName;
// 	string strHtmlUrl;
// 	string strRssUrl;
	__category_data();
	void SetAttribute(string strKey,string strValue);
}CATEGORY_DATA;

typedef struct __post_data
{
	string strID;
	string strCategory;
	string strTitle;
	string strDescription;
	string strImg;
	string strTorrent;
	string strMagnet;
	string strTorrentShorturl1;
	string strTorrentShorturl2;
	void SetAttribute(string strKey,string strValue);
}POST_DATA;

class CPost
{
private:
//	vector<TAG_DATA*> m_tagsVec;
	vector<CATEGORY_DATA*> m_categorysVec;
	string m_strUserName;
	string m_strPassword;
	string m_strBlogID;
	string m_strXmlRpc;
public:
	CPost(string strXmlRpc,string strUserName,string strPassword,int nBlogID);
	~CPost();
	bool Run();
//	bool GetTags(vector<TAG_DATA*>& vec);
	bool GetCategorys(vector<CATEGORY_DATA*>& vec);
	bool NewCategorys(vector<CATEGORY_DATA*>& vec);
	bool NewPosts(vector<POST_DATA*>& vec);
private:
	bool FetchUrl(string strUrl,string& strPostData,string& strContent,size_t& nByteLen);
	bool ReadFile(string strFileName,string& strContent,size_t& nByteRead);
	bool WriteFile(string strFileName,string& strContent,size_t nByteWrite);
	static int LoadCategorysFromDB(void* pData,int nNum,char** ppValue,char** ppName);
	bool IsCategoryExist(vector<CATEGORY_DATA*>& vec, const string& strName);
	bool CheckNewCategory(string& strResult,int& nID);
	bool CheckNewPost(string& strResult,int& nID);

	static int LoadPostsFromDB(void* pData,int nNum,char** ppValue,char** ppName);
	bool EscapeXml(string& strSrc);
};