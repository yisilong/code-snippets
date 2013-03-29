#pragma once
#include <string>
class CMyDate
{
private:
	unsigned int m_nYear;
	unsigned int m_nMonth;
	unsigned int m_nDay;
public:
	CMyDate();
	CMyDate(unsigned int nYear,unsigned int nMonth,unsigned int nDay);
	~CMyDate();
	std::string GetFormat();
	bool AddDay();
	int DiffDays(CMyDate date);
private:
	bool IsLeapYear();
};
