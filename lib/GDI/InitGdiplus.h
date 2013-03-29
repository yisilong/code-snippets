#ifndef ULONG_PTR
#define ULONG_PTR ULONG
#endif

#include ".\\Includes\\GdiPlus.h"
#pragma comment(lib,".\\Lib\\GdiPlus.lib")
using namespace Gdiplus;

#ifndef _INIT_GDIPLUS_
#define _INIT_GDIPLUS_

class CInitGDIPlus
{
public:
	CInitGDIPlus() throw();
    virtual	~CInitGDIPlus() throw();
	virtual bool Init() throw();
private:
    ULONG_PTR m_dwToken;	
};


inline CInitGDIPlus::CInitGDIPlus() throw()
{
	m_dwToken = 0;
}

inline CInitGDIPlus::~CInitGDIPlus() throw()
{
	if (m_dwToken !=0)
	{
		Gdiplus::GdiplusShutdown(m_dwToken);
	}
}
inline bool CInitGDIPlus::Init() throw()
{
	if (m_dwToken == 0)
	{
		Gdiplus::GdiplusStartupInput input;
        Gdiplus::GdiplusStartupOutput output;
		Gdiplus::Status status = Gdiplus::GdiplusStartup(&m_dwToken,&input,&output);
		if (status != Gdiplus::Ok)
		{
			return false;
		}
	}
	return true;
}
#endif