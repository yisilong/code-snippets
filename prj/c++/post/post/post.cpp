#include "post.h"
#include <process.h>
#include <fstream>
#include <sstream>
#include "rapidxml/rapidxml.hpp"
#include "tools.h"
#include "MyDate.h"
using namespace rapidxml;
CPost::CPost(string strXmlRpc,string strUserName,string strPassword,int nBlogID)
{
	m_strXmlRpc = strXmlRpc;
	m_strUserName = strUserName;
	m_strPassword = strPassword;
	stringstream s(ios_base::in | ios_base::out);
	s << nBlogID;
	s >> m_strBlogID;
}

CPost::~CPost()
{
// 	for (vector<TAG_DATA*>::iterator it = m_tagsVec.begin(); it != m_tagsVec.end(); ++it)
// 	{
// 		delete *it;
// 	}
	for (vector<CATEGORY_DATA*>::iterator it = m_categorysVec.begin(); it != m_categorysVec.end(); ++it)
	{
		delete *it;
	}
}

bool CPost::FetchUrl(string strUrl,string& strPostData,string& strContent,size_t& nByteLen)
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
	char ch[100];
	sprintf_s(ch,100,"\r\nContent-Length: %d",strPostData.length());
	string strHeader = "POST ";
	strHeader += strRequest + " HTTP/1.1";
	strHeader += "\r\nHost: ";
	strHeader += strHost.c_str();
	strHeader += "\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:15.0) Gecko/20100101 Firefox/15.0.1";
	strHeader += "\r\nContent-Type: text/xml";
	strHeader += ch;
	strHeader += "\r\nAccept: */*";
	strHeader += "\r\nConnection: close\r\n\r\n";
	strHeader += strPostData;

	int nPort = 80;
	string strHostName =  strHost;//"127.0.0.1";//

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
// 	ofstream outF("f.xml",ios::binary | ios::out | ios::trunc);
// 	outF.write(strContent.c_str(),nByteLen);
// 	outF.close();
	return true;
}

bool CPost::ReadFile( string strFileName,string& strContent,size_t& nByteRead )
{
	fstream inStream(strFileName,ios::in | ios::binary);
	inStream.seekg (0, ios::end);
	int length = inStream.tellg();
	inStream.seekg (0, ios::beg);
	if (length <= 0)
	{
		return false;
	}
	char* pData = new char[length+1];
	CSafeDeleteArray<char> safeDel(pData);
	inStream.read(pData,length);
	pData[length] = '\0';
	inStream.close();
	strContent.assign(pData,pData+length);
	nByteRead = length;
	return true;
}

bool CPost::WriteFile( string strFileName,string& strContent,size_t nByteWrite )
{
	fstream outStream(strFileName,ios::out | ios::binary | ios::trunc);
	outStream.write(strContent.data(),nByteWrite);
	outStream.close();
	return true;
}

// bool CPost::GetTags( vector<TAG_DATA*>& vec )
// {
// 	string strPostData = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
// 	strPostData += "<methodCall>\n";
// 	strPostData += "\t<methodName>wp.getTags</methodName>\n";
// 	strPostData += "\t<params>\n";
// 	strPostData += "\t\t<param><value><int>" + m_strBlogID + "</int></value></param>\n";
// 	strPostData += "\t\t<param><value><string>" + m_strUserName + "</string></value></param>\n";
// 	strPostData += "\t\t<param><value><string>" + m_strPassword + "</string></value></param>\n";
// 	strPostData += "\t<params>\n";
// 	strPostData += "<methodCall>\n";
// 	
// 	size_t nByteLen;
// 	string strContent;
// 	if (!FetchUrl(m_strXmlRpc,strPostData,strContent,nByteLen))
// 	{
// 		cout << "tags 列表页面下载失败" << endl;
// 		return false;
// 	}
// 	xml_document<> xml;
// 	try
// 	{
// 		char* pData = const_cast<char*>(strContent.c_str());
// 		xml.parse<0>(pData);
// 	}
// 	catch (...)
// 	{
// 		cout << "tags 列表页面解析失败" << endl;
// 		return false;
// 	}
// 
// 	char* pArry[6] = { "methodResponse","params","param","value","array","data" };
// 	
// 	xml_node<>* pNode = &xml;
// 	for (int i= 0; i < 6; i++)
// 	{
// 		pNode = pNode->first_node(pArry[i]);
// 		if (pNode == NULL)
// 		{
// 			return false;
// 		}
// 	}
// 
// 	pNode= pNode->first_node("value");
// 	while (pNode != NULL)
// 	{
// 		xml_node<>* pTmpNode = pNode->first_node("struct");
// 		if (pTmpNode == NULL) return false;
// 		pTmpNode = pTmpNode->first_node("member");
// 		if (pTmpNode == NULL) return false;
// 		TAG_DATA* pTagData = new TAG_DATA;
// 		CSafeDelete<TAG_DATA> safeDel(pTagData);
// 		while (pTmpNode != NULL)
// 		{
// 			xml_node<>* pNameNode = pTmpNode->first_node("name");
// 			xml_node<>* pValueNode = pTmpNode->first_node("value");
// 			if (pNameNode == NULL || pValueNode == NULL)
// 			{
// 				return false;
// 			}
// 			pValueNode = pValueNode->first_node("string");
// 			if (pValueNode == NULL ) return false;
// 			pTagData->SetAttribute(pNameNode->value(),pValueNode->value());
// 			pTmpNode = pTmpNode->next_sibling("member");
// 		}
// 		safeDel.Detach();
// 		vec.push_back(pTagData);
// 		pNode = pNode->next_sibling("value");
// 	}
// 
// 	return true;
// }

bool CPost::GetCategorys(vector<CATEGORY_DATA*>& vec)
{
	string strPostData = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	strPostData += "<methodCall>\n";
	strPostData += "\t<methodName>wp.getCategories</methodName>\n";
	strPostData += "\t<params>\n";
	strPostData += "\t\t<param><value><int>" + m_strBlogID + "</int></value></param>\n";
	strPostData += "\t\t<param><value><string>" + m_strUserName + "</string></value></param>\n";
	strPostData += "\t\t<param><value><string>" + m_strPassword + "</string></value></param>\n";;
	strPostData += "\t</params>\n";
	strPostData += "</methodCall>\n";

	size_t nByteLen;
	string strContent;
	if (!FetchUrl(m_strXmlRpc,strPostData,strContent,nByteLen))
	{
		cout << "categorys 列表页面下载失败" << endl;
		return false;
	}
	//WriteFile("f.xml",strContent,nByteLen);
	xml_document<> xml;
	try
	{
		char* pData = const_cast<char*>(strContent.c_str());
		xml.parse<0>(pData);
	}
	catch (...)
	{
		cout << "categorys 列表页面解析失败" << endl;
		return false;
	}

	char* pArry[6] = { "methodResponse","params","param","value","array","data" };

	xml_node<>* pNode = &xml;
	for (int i= 0; i < 6; i++)
	{
		pNode = pNode->first_node(pArry[i]);
		if (pNode == NULL)
		{
			return false;
		}
	}

	pNode= pNode->first_node("value");
	while (pNode != NULL)
	{
		xml_node<>* pTmpNode = pNode->first_node("struct");
		if (pTmpNode == NULL) return false;
		pTmpNode = pTmpNode->first_node("member");
		if (pTmpNode == NULL) return false;
		CATEGORY_DATA* pData = new CATEGORY_DATA;
		CSafeDelete<CATEGORY_DATA> safeDel(pData);
		while (pTmpNode != NULL)
		{
			xml_node<>* pNameNode = pTmpNode->first_node("name");
			xml_node<>* pValueNode = pTmpNode->first_node("value");
			if (pNameNode == NULL || pValueNode == NULL)
			{
				return false;
			}
			pValueNode = pValueNode->first_node("string");
			if (pValueNode == NULL ) return false;
			pData->SetAttribute(pNameNode->value(),pValueNode->value());
			pTmpNode = pTmpNode->next_sibling("member");
		}
		safeDel.Detach();
		vec.push_back(pData);
		pNode = pNode->next_sibling("value");
	}
	if (vec.empty()) // wordpress 不可能没有分类
	{
		return false;
	}
	return true;
}

bool CPost::NewCategorys( vector<CATEGORY_DATA*>& vec )
{
	fstream outStream("NewCategorysFailed.txt",ios::out | ios::trunc | ios::binary);
	for (vector<CATEGORY_DATA*>::iterator it = vec.begin(); it != vec.end(); ++it)
	{
		CATEGORY_DATA* pData = *it;
		string strPostData = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		strPostData += "<methodCall>\n";
		strPostData += "\t<methodName>wp.newCategory</methodName>\n";
		strPostData += "\t<params>\n";
		strPostData += "\t\t<param><value><int>" + m_strBlogID + "</int></value></param>\n";
		strPostData += "\t\t<param><value><string>" + m_strUserName + "</string></value></param>\n";
		strPostData += "\t\t<param><value><string>" + m_strPassword + "</string></value></param>\n";
		strPostData += "\t\t<param><value><struct>\n";
		strPostData += "\t\t\t<member><name>name</name><value><string>" + pData->strCategoryName + "</string></value></member>\n";
		strPostData += "\t\t\t<member><name>parent_id</name><value><int>" + pData->strParentId + "</int></value></member>\n";			
		strPostData += "\t\t</struct></value></param>\n";
		strPostData += "\t</params>\n";
		strPostData += "</methodCall>\n";
		string strContent;
		size_t nByteLen;
		if (FetchUrl(m_strXmlRpc,strPostData,strContent,nByteLen))
		{
			int nId;
			if (CheckNewCategory(strContent,nId))
			{
				cout << "添加分类: " << pData->strCategoryName << "成功" << endl;
				continue;
			}
		}
		cout << "添加分类: " << pData->strCategoryName << "失败" << endl;
		outStream << pData->strCategoryName << endl;
	}
	outStream.close();
	return true;
}

bool CPost::NewPosts(vector<POST_DATA*>& vec)
{
	CMyDate now;
	string strImgPath = "likebeta.aliapp.com/ixxoo/" + now.GetFormat();
	string strTorrentPath = "messycode.aliapp.com/ixxoo/" + now.GetFormat();
	int nCount = 1;
	fstream outStream("NewPostsFailed.txt",ios::out | ios::trunc | ios::binary);
	for (vector<POST_DATA*>::iterator it = vec.begin(); it != vec.end(); ++it)
	{
		POST_DATA* pData = *it;
		string strTorrent = pData->strTorrent;
		string strImg = pData->strImg;
		if (!strTorrent.empty())
		{
			int nBegin = strTorrent.rfind("/");
			strTorrent = strTorrentPath + strTorrent.substr(nBegin);
		}
	
		string strCategory = string_to_utf8(pData->strCategory);
		string strTitle = string_to_utf8(pData->strTitle+"[磁性链接BT种子][高清MKV迅雷下载]");
		string strDescription;
		if (!strImg.empty())
		{
			int nBegin = strImg.rfind("/");
			strImg = strImgPath + strImg.substr(nBegin);
			strDescription = "<div id=\"content-img\"><img src=\"http://" + strImg + "\" /></div>";
		}
		
		strDescription += "<div id=\"content-text\">" + pData->strDescription + "</div>";
		strDescription += "<div id=\"content-dowload\">";
		if (!strTorrent.empty())
		{
			strDescription += "<a href=\"http://" + strTorrent + "\" target=\"_blank\" id=\"torrent-url\">种子</a>";
			if (!pData->strTorrentShorturl1.empty())
			{
				strDescription += "<a href=\"" + pData->strTorrentShorturl1 + "\" target=\"_blank\" id=\"torrent-url1\">种子</a>";
			}
			if (!pData->strTorrentShorturl2.empty())
			{
				strDescription += "<a href=\"" + pData->strTorrentShorturl2 + "\" target=\"_blank\" id=\"torrent-url2\">种子</a>";
			}
		}
		if (!pData->strMagnet.empty())
		{
			strDescription += "<a href=\"" + pData->strMagnet + "\" target=\"_blank\" id=\"magnet-url\">磁性链接</a>";
		}
		strDescription += "</div>";
		strDescription = string_to_utf8(strDescription);
		EscapeXml(strCategory);
		EscapeXml(strTitle);
		EscapeXml(strDescription);
		string strPostData = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		strPostData += "<methodCall>\n";
		strPostData += "\t<methodName>metaWeblog.newPost</methodName>\n";
		strPostData += "\t<params>\n";
		strPostData += "\t\t<param><value><int>" + m_strBlogID + "</int></value></param>\n";
		strPostData += "\t\t<param><value><string>" + m_strUserName + "</string></value></param>\n";
		strPostData += "\t\t<param><value><string>" + m_strPassword + "</string></value></param>\n";
		strPostData += "\t\t<param><value><struct>\n";
		strPostData += "\t\t\t<member><name>mt_keywords</name><value><string>" + strCategory + "</string></value></member>\n";
		strPostData += "\t\t\t<member><name>post_status</name><value><string>publish</string></value></member>\n";
		strPostData += "\t\t\t<member><name>sticky</name><value><boolean>0</boolean></value></member>\n";
		strPostData += "\t\t\t<member><name>categories</name><value><array><data><value><string>" + strCategory + "</string></value></data></array></value></member>\n";
		strPostData += "\t\t\t<member><name>title</name><value><string>" + strTitle + "</string></value></member>\n";
		strPostData += "\t\t\t<member><name>description</name><value><string>" + strDescription + "</string></value></member>\n";;
		strPostData += "\t\t</struct></value></param>\n";
		strPostData += "\t\t<param><value><boolean>1</boolean></value></param>\n";
		strPostData += "\t</params>\n";
		strPostData += "</methodCall>\n";
		string strContent;
		size_t nByteLen;
		if (!FetchUrl(m_strXmlRpc,strPostData,strContent,nByteLen))
		{
			outStream << pData->strID  << endl;
		}
		cout << nCount++ << endl;
	}

	return true;
}

bool CPost::Run()
{
	vector<CATEGORY_DATA*> cateGoryvec;
	if (!GetCategorys(cateGoryvec))
	{
		cout << "获取网站分类失败" << endl;
		return false;
	}

	sqlite3* db;
	if (SQLITE_OK != sqlite3_open("btscg.db",&db))
	{
		cout << "打开数据库失败" << endl;
		return false;
	}
	vector<string> vec;
	if (SQLITE_OK != sqlite3_exec(db,"SELECT distinct(type) FROM movie;",LoadCategorysFromDB,&vec,NULL))
	{
		cout << "获取数据库type失败" << endl;
		sqlite3_close(db);
		return false;
	}

	for (vector<string>::iterator it = vec.begin();it != vec.end(); )
	{
		if (IsCategoryExist(cateGoryvec,*it))
		{
			it = vec.erase(it);
			continue;
		}
		else
		{
			CATEGORY_DATA* pData = new CATEGORY_DATA;
			pData->SetAttribute("categoryName",*it);
			m_categorysVec.push_back(pData);
			++it;
		}
	}

	for (vector<CATEGORY_DATA*>::iterator it = cateGoryvec.begin(); it != cateGoryvec.end(); ++it)
	{
		delete *it;
	}

	cout << "开始创建分类" << endl;
	if (!NewCategorys(m_categorysVec))
	{
		cout << "新建分类失败" << endl;
		sqlite3_close(db);
		return false;
	}
	cout << "完成创建分类" << endl;
	cout << "go on ?" << endl;
	if (getchar() != 'y')
	{
		sqlite3_close(db);
		return true;
	}

	CMyDate now;
	CMyDate tharDay(2012,10,23);
	int nBegin = now.DiffDays(tharDay) * 100;
	char ch[256] = {0};
	sprintf_s(ch,256,"select id,type,title,content,img,torrent,torrentshorturl1,torrentshorturl2,magnet from movie limit %d,100;",nBegin);
	vector<POST_DATA*> postVec;
	if (SQLITE_OK != sqlite3_exec(db,ch,LoadPostsFromDB,&postVec,NULL))
	{
		cout << "数据库查询数据失败" << endl;
		sqlite3_close(db);
		return false;
	}
	sqlite3_close(db);
	if (!NewPosts(postVec))
	{
		cout << "新建文章失败" << endl;
		return false;
	}
	for (vector<POST_DATA*>::iterator it = postVec.begin(); it != postVec.end(); ++it)
	{
		delete *it;
	}
	return true;
}

int CPost::LoadCategorysFromDB( void* pData,int nNum,char** ppValue,char** ppName )
{
	vector<string>* pVec = static_cast<vector<string>*>(pData);
	pVec->push_back(string_to_utf8(ppValue[0]));
	return 0;
}

bool CPost::IsCategoryExist(vector<CATEGORY_DATA*>& vec, const string& strName)
{
	for (vector<CATEGORY_DATA*>::iterator it = vec.begin(); it != vec.end(); ++it)
	{
		CATEGORY_DATA* pData = *it;
		if (pData->strCategoryName == strName)
		{
			return true;
		}
	}
	return false;
}

bool CPost::CheckNewCategory( string& strResult,int& nID )
{
	xml_document<> xml;
	try
	{
		char* pData = const_cast<char*>(strResult.c_str());
		xml.parse<0>(pData);
	}
	catch (...)
	{
		return false;
	}

	char* str[5] = {"methodResponse","params","param","value","int"};
	xml_node<>* pNode = &xml;
	for (int i = 0;i < 5;++i)
	{
		if (pNode == NULL)
		{
			return false;
		}
		pNode = pNode->first_node(str[i]);
	}
	stringstream s(pNode->value());
	s >> nID;
	return true;
}

bool CPost::EscapeXml( string& strSrc )
{
	static string strEscape[5] = {"&","<",">","\"","'"};
	static string strResult[5] = {"&amp;","&lt;","&gt;","&quot;","&apos;"};
	for (int i = 0; i < 5; ++i )
	{
		int nEnd = 0;
		int nBegin = strSrc.find(strEscape[i],nEnd);
		while (nBegin != string::npos)
		{
			nEnd = nBegin + strEscape[i].length();
			strSrc.replace(nBegin,nEnd - nBegin,strResult[i]);
			nEnd = nBegin + strResult[i].length();
			nBegin = strSrc.find(strEscape[i],nEnd);
		}
	}
	return true;
}

bool CPost::CheckNewPost( string& strResult,int& nID )
{
	
	return true;
}

int CPost::LoadPostsFromDB( void* pData,int nNum,char** ppValue,char** ppName )
{
	vector<POST_DATA*>* pVec = static_cast<vector<POST_DATA*>*>(pData);
	POST_DATA* pPostData = new POST_DATA;
	for ( int i = 0; i < nNum; ++i)
	{
		pPostData->SetAttribute(ppName[i],ppValue[i]);
	}
	pVec->push_back(pPostData);
	return 0;
}

void __tag_data::SetAttribute( string strKey,string strValue )
{
	if (strKey == "tag_id" )
	{
		strTagId = strValue;
	}
	else if (strKey == "name")
	{
		strName = strValue;
	}
	else if (strKey == "count")
	{
		strCount = strValue;		
	}
	else if (strKey == "slug")
	{
		strSlug = strValue;
	}
// 	else if (strKey == "html_url")
// 	{
// 		strHtmlUrl = strValue;
// 	}
// 	else if (strKey == "rss_url")
// 	{
// 		strRssUrl = strValue;
// 	}
}

void __category_data::SetAttribute( string strKey,string strValue )
{
	if (strKey == "categoryId" )
	{
		strCategoryId = strValue;
	}
	else if (strKey == "parentId")
	{
		strParentId = strValue;
	}
	else if (strKey == "description")
	{
		strDescription = strValue;		
	}
	else if (strKey == "categoryDescription")
	{
		strCategoryDescription = strValue;
	}
	else if (strKey == "categoryName")
	{
		strCategoryName = strValue;
	}
// 	else if (strKey == "html_url")
// 	{
// 		strHtmlUrl = strValue;
// 	}
// 	else if (strKey == "rss_url")
// 	{
// 		strRssUrl = strValue;
// 	}
}

__category_data::__category_data():strParentId("0")
{

}

void __post_data::SetAttribute( string strKey,string strValue )
{
	if (strKey == "type" )
	{
		strCategory = strValue;
	}
	else if (strKey == "title")
	{
		strTitle = strValue;
	}
	else if (strKey == "content")
	{
		strDescription = strValue;		
	}
	else if (strKey == "img")
	{
		strImg = strValue;
	}
	else if (strKey == "torrent")
	{
		strTorrent = strValue;		
	}
	else if (strKey == "torrentshorturl1")
	{
		strTorrentShorturl1 = strValue;
	}
	else if (strKey == "torrentshorturl2")
	{
		strTorrentShorturl2 = strValue;
	}
	else if (strKey == "magnet")
	{
		strMagnet = strValue;		
	}
	else if (strKey == "id")
	{
		strID = strValue;
	}
}
