#pragma once
#include <Windows.h>
#include <string>
template< typename T >
class CSafeDelete
{
public:
	CSafeDelete() : m_p(NULL){};
	CSafeDelete(T* p) : m_p(p) { };
	~CSafeDelete() { if( m_p != NULL ) delete m_p; };
	T* Attach(T* p) { m_p = p;return m_p;};
	T* Detach() { T* t = m_p; m_p = NULL; return t; };
	bool HasAttach(){ return m_p != NULL; };
private:
	T* m_p;
};

template< typename T >
class CSafeDeleteArray
{
public:
	CSafeDeleteArray() : m_p(NULL){};
	CSafeDeleteArray(T* p) : m_p(p) { };
	~CSafeDeleteArray() { if( m_p != NULL ) delete []m_p; };
	T* Attach(T* p) { m_p = p;return m_p;};
	T* Detach() { T* t = m_p; m_p = NULL; return t; };
	bool HasAttach(){ return m_p != NULL; };
private:
	T* m_p;
};

static LPWSTR AnsiToUnicode(LPSTR szAnsi)
{
	int nSize = MultiByteToWideChar(CP_ACP,0,szAnsi,-1,NULL,0);
	WCHAR* pData = new WCHAR[nSize+1];
	pData[nSize] = L'\0';
	MultiByteToWideChar(CP_ACP,0,szAnsi,-1,pData,nSize);
	return pData;
}

static std::string UnicodeToAnsi(LPCTSTR szUnicode)
{
	if (szUnicode == NULL)
	{
		return "";
	}
#ifndef UNICODE
	int nSize = strlen(szUnicode);
	char* pData = new char[nSize+1];
	CSafeDeleteArray<char> safeDel(pData);
	pData[nSize] = '\0';
	memcpy_s(pData,nSize,szUnicode,nSize);
	return std::string(pData);
#else 
	int nSize = WideCharToMultiByte(CP_ACP,0,szUnicode,-1,NULL,0,NULL,FALSE);
	char* pData = new char[nSize+1];
	CSafeDeleteArray<char> safeDel(pData);
	pData[nSize] = '\0';
	WideCharToMultiByte(CP_ACP,0,szUnicode,-1,pData,nSize,NULL,FALSE);
	return std::string(pData);
#endif
}

#ifdef UNICODE
	#define PrintDebugString(tcharMsg) OutputDebugString(tcharMsg)
#else
	#define PrintDebugString(tcharMsg)
#endif

#define IsNULL(p) (static_cast<bool>(p == NULL))
#define IsNotNULL(p) (static_cast<bool>(p != NULL))
#define IsZero(n) (static_cast<bool>(n == 0))
#define IsNotZero(n) (static_cast<bool>(n != 0))
