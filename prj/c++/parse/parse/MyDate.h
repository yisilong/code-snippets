#pragma once
#include <string>
class CMyDate
{
private:
	unsigned int m_nYear;
	unsigned int m_nMonth;
	unsigned int m_nDay;
public:
	CMyDate(unsigned int nYear = 1970,unsigned int nMonth = 1,unsigned int nDay = 1);
	~CMyDate(void);
	std::string GetFormat();
	bool AddDay();
private:
	bool IsLeapYear();
};
