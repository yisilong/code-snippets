#include "StdAfx.h"
#include "MyTime.h"
//download by www.codefans.net
CMyTime::CMyTime()
{
	ValueTime=CTime::GetCurrentTime();
}
CString CMyTime::GetAllString(BOOL blnChinese)
{
	CString str;
	if(blnChinese)
		str=ValueTime.Format("%Y年%m月%d日 %H时%M分%S秒");
	else
		str=ValueTime.Format("%Y-%m-%d %H:%M:%S");
	return str;
}
CString CMyTime::GetDateString(BOOL blnChinese)
{
	CString str;
	if(blnChinese)
		str=ValueTime.Format("%Y年%m月%d日");
	else
		str=ValueTime.Format("%Y-%m-%d");
	return str;
}
CString CMyTime::GetTimeString(BOOL blnChinese)
{
	CString str;
	if(blnChinese)
		str=ValueTime.Format("%H时%M分%S秒");
	else
		str=ValueTime.Format("%H:%M:%S");
	return str;
}
CString CMyTime::GetSimpleString()
{
	CString str=ValueTime.Format("%Y%m%d%H%M%S");
	return str;
}
CString CMyTime::GetWeek()
{
    CString str;
	int i=ValueTime.GetDayOfWeek();
	CString strWeek[7]={"日","一","二","三","四","五","六"};
	str="星期"+strWeek[i-1];
	return str;
}
void CMyTime::SetAllString(CString Value,BOOL blnSimple)
{
	CString strYear,strMonth,strDay,strHour,strMin,strSec;
	if(blnSimple==FALSE)
	{
		strYear=Value.Mid(0,4);
		strMonth=Value.Mid(4,2);
		strDay=Value.Mid(6,2);
		if(Value.GetLength()>8)
		{
			strHour=Value.Mid(8,2);
			strMin=Value.Mid(10,2);
			strSec=Value.Mid(12,2);
		}
		else
		{
			strHour="0";
			strMin="0";
			strSec="0";
		}
	}
	else
	{
		strYear=Value.Mid(0,4);
		strMonth=Value.Mid(5,2);
		strDay=Value.Mid(8,2);
		if(Value.GetLength()>10)
		{
			strHour=Value.Mid(11,2);
			strMin=Value.Mid(14,2);
			strSec=Value.Mid(17,2);
		}
		else
		{
			strHour="0";
			strMin="0";
			strSec="0";
		}
	}
	CTime TValue(atoi(strYear),atoi(strMonth),atoi(strDay),atoi(strHour),atoi(strMin),atoi(strSec));
	ValueTime=TValue;
}
void CMyTime::SetNow()
{
	ValueTime=CTime::GetCurrentTime();
}