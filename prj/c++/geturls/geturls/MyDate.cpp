#include "MyDate.h"
#include <sstream>
#include <Windows.h>
CMyDate::CMyDate()
{
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	m_nYear = tm.wYear;
	m_nMonth = tm.wMonth;
	m_nDay = tm.wDay;
}

CMyDate::CMyDate( unsigned int nYear,unsigned int nMonth,unsigned int nDay )
{
	m_nYear = nYear;
	m_nMonth = nMonth;
	m_nDay = nDay;
}

CMyDate::~CMyDate()
{
}

bool CMyDate::IsLeapYear()
{
	return (m_nYear%4 == 0 && m_nYear%100 != 0) || (m_nYear%400 == 0);
}

std::string CMyDate::GetFormat()
{
	using namespace std;
	stringstream s(stringstream::in | stringstream::out);
	s << m_nYear;
	if (m_nMonth < 10)
	{
		s << 0;
	}
	s << m_nMonth << "/";
	if (m_nDay < 10)
	{
		s << 0;
	}
	s << m_nDay;
	return s.str();
}

bool CMyDate::AddDay()
{
	int nMaxDay = 31;
	if (m_nMonth == 4 || m_nMonth == 6 || m_nMonth == 9 || m_nMonth == 11)
	{
		nMaxDay = 30;
	}
	else if (m_nMonth == 2)
	{
		nMaxDay = IsLeapYear() ? 29 : 28;
	}

	if (nMaxDay == m_nDay)
	{
		m_nMonth ++;
		m_nDay = 1;
		if (m_nMonth > 12)
		{
			m_nMonth = 1;
			m_nYear++;
		}
	}
	else
	{
		m_nDay ++;
	}
	return true;
}

int CMyDate::DiffDays( CMyDate date )
{
	CMyDate* pBig;
	CMyDate* pSmall;
	CMyDate me(*this);
	if (me.GetFormat() > date.GetFormat())
	{
		pBig = &me;
		pSmall = &date;
	}
	else if (me.GetFormat() < date.GetFormat())
	{
		pBig = &date;
		pSmall = &me;
	}
	int nDays = 0;
	while (memcmp(pSmall,pBig,sizeof(*this)) != 0)
	{
		nDays++;
		pSmall->AddDay();
	}
	return nDays;
}
