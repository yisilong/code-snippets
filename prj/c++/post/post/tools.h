#include <string>
#include <Windows.h>
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

static bool utf8_to_string(std::string strUtf8,std::string& strOut)
{
	int nSize = MultiByteToWideChar(CP_UTF8,0,strUtf8.c_str(),-1,NULL,0);
	wchar_t* pWChar = new wchar_t[nSize] ;
	CSafeDeleteArray<wchar_t> safeDel(pWChar);
	MultiByteToWideChar(CP_UTF8,0,strUtf8.c_str(),-1,pWChar,nSize);

	nSize = WideCharToMultiByte(CP_ACP,0,pWChar,-1,NULL,0,NULL,FALSE);;
	char* pChar = new char[nSize];
	CSafeDeleteArray<char> safeDel1(pChar);
	WideCharToMultiByte(CP_ACP,0,pWChar,-1,pChar,nSize,NULL,FALSE);
	strOut = pChar;
	return true;
}

static std::string utf8_to_string(std::string strUtf8)
{
	int nSize = MultiByteToWideChar(CP_UTF8,0,strUtf8.c_str(),-1,NULL,0);
	wchar_t* pWChar = new wchar_t[nSize] ;
	CSafeDeleteArray<wchar_t> safeDel(pWChar);
	MultiByteToWideChar(CP_UTF8,0,strUtf8.c_str(),-1,pWChar,nSize);

	nSize = WideCharToMultiByte(CP_ACP,0,pWChar,-1,NULL,0,NULL,FALSE);;
	char* pChar = new char[nSize];
	CSafeDeleteArray<char> safeDel1(pChar);
	WideCharToMultiByte(CP_ACP,0,pWChar,-1,pChar,nSize,NULL,FALSE);
	return std::string(pChar);
}

static std::string string_to_utf8(std::string str)
{
	int nSize = MultiByteToWideChar(CP_ACP,0,str.c_str(),-1,NULL,0);
	wchar_t* pWChar = new wchar_t[nSize+1] ;
	pWChar[0] = L'\0';
	CSafeDeleteArray<wchar_t> safeDel(pWChar);
	MultiByteToWideChar(CP_ACP,0,str.c_str(),-1,pWChar,nSize);

	nSize = WideCharToMultiByte(CP_UTF8,0,pWChar,-1,NULL,0,NULL,FALSE);;
	char* pChar = new char[nSize+1];
	pChar[nSize] = '\0';
	CSafeDeleteArray<char> safeDel1(pChar);
	WideCharToMultiByte(CP_UTF8,0,pWChar,-1,pChar,nSize,NULL,FALSE);
	return std::string(pChar);
}