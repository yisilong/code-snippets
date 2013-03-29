#include "MyDate.h"
#include <sstream>
CMyDate::CMyDate(unsigned int nYear /*= 1970*/,unsigned int nMonth /*= 1*/,unsigned int nDay /*= 1*/)
{
	m_nYear = nYear;
	m_nMonth = nMonth;
	m_nDay = nDay;
}

CMyDate::~CMyDate(void)
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
	s << m_nMonth << "\\";
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
