
#include "stdafx.h"
#include "MyDateEdit.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
IMPLEMENT_DYNAMIC(CMyDateEdit,CEdit)
BEGIN_MESSAGE_MAP(CMyDateEdit, CEdit)
	//{{AFX_MSG_MAP(CMyDateEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CUT, OnCut)
	ON_MESSAGE(WM_PASTE, OnPaste)
	ON_MESSAGE(WM_CLEAR, OnClear)
END_MESSAGE_MAP()
CMyDateEdit::CMyDateEdit()
{
	Reset();
	m_isDateTime=TRUE;
//	SetClassType(edit_datetime);
}

CMyDateEdit::~CMyDateEdit()
{
}
/////////////////////////////////////////////////////////////////////////////
// CMyDateEdit message handlers
void CMyDateEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CString str;
	GetWindowText(str);
	for (int i = 0; i<str.GetLength() && i<m_str.GetLength();i++)
		m_str.SetAt(i, str.GetAt(i));
	if(!m_bMaskKeyInProgress)
		if(!CheckChar(nChar)) return;
		if(m_bUseMask)  //要使用掩码
		{
			if(isdigit(nChar))  //是可打印字符
			{
				int startPos,endPos;
				GetSel(startPos,endPos);
				SetSel(startPos,endPos+1);
				if(m_strMask.GetAt(startPos)=='-'||m_strMask.GetAt(startPos)==':'||m_strMask.GetAt(startPos)==' ')
				{
					SetSel(startPos+1,startPos+1);
					SendChar(nChar);
					return;
				}
			}
			else if(nChar==VK_BACK)
			{
				int startPos,endPos;
				GetSel(startPos,endPos);
				if((startPos==endPos) && (startPos>=1) && (startPos<=m_str.GetLength()))
				{
					char c;
					c=m_strMask.GetAt(startPos-1);
					if(c=='-'||c==':'||c==' ') 
					{					
						SetSel(startPos-1,startPos-1);
						return;
					}
					////回退光标
					SetSel(startPos-1,startPos-1);
					SendChar(c);
						//再次退回
					SendMessage(WM_KEYDOWN,VK_LEFT,0);
				}
				else   //越界或者存在选择区域
					MessageBeep((UINT)-1);
				return;
			}

		}
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CMyDateEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{

	if(m_bUseMask)
	{
		switch(nChar)//忽略删除和插入键
		{
		case VK_DELETE:
		case VK_INSERT: return;
		}
	}
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}
BOOL CMyDateEdit::CheckChar(UINT nChar)
{
	int nTime = 0;
	//如果不使用掩码，则返回
	if(!m_bUseMask)	return TRUE;
	//如果是控制字符，则返回
	if(!isprint(nChar)) return TRUE;
	if(!isdigit(nChar))	
	{
		MessageBeep((UINT)-1);
		return FALSE;
	}
	//如果存在选择区域，则取消选择
	int startPos,endPos;
	GetSel(startPos,endPos);
	SetSel(-1,0);
	//重新选中原选择区域的第一个字符
	SetSel(startPos,startPos);
	GetSel(startPos,endPos);
	//确保字符串的长度不超过掩码的长度
	if(endPos>=m_strMask.GetLength())
	{
		MessageBeep((UINT)-1);
		return FALSE;
	}
	//时间格式
	if(m_isTime)
	{
		if(!CheckTime(nChar,startPos,endPos))
		{
			return FALSE;
		}
	}
	//日期格式
	if(m_isDate)
	{
		if(!CheckDate(nChar,startPos,startPos))
		{
			return FALSE;
		}
	}
	if(m_isDateTime)
	{
		if(!CheckDate(nChar,startPos,startPos))
		{
			return FALSE;
		}
		if(!CheckTime(nChar,startPos,startPos))
		{
			return FALSE;
		}
	}
    return TRUE;
}
BOOL CMyDateEdit::CheckDate(char nchar,int StartPos,int EndPos)
{	
	CString strText;
	BOOL Leap;	
	int Year,Month,Day;
	strText=GetText();
	Year=atoi(strText);
	Month=atoi(strText.Mid(5,2));
	Day=atoi(strText.Mid(8,2));
	if((Year%4)==0)
	{
		if(((Year%100)==0)&&((Year%400)!=0))
			Leap=FALSE;
		else
			Leap=TRUE;
	}
	else
		Leap=FALSE;
	if(StartPos==4||StartPos==5)
	{
			if(nchar>'1')
			{
				MessageBeep((UINT)-1);
				return FALSE;
			}
			else if(nchar=='1')
			{
				strText=GetText();
				if(strText.GetAt(6)>'2')
				{
					strText.SetAt(6,'2');
					SetText(strText);
					SetSel(StartPos,StartPos);
					return TRUE;
				}
			}
			return TRUE;
	}
	if(StartPos==6)
	{
		strText=GetText();
		if(strText.GetAt(5)=='1')
		{
			if(nchar>'2')
			{
				MessageBeep((UINT)-1);
				return FALSE;
			}
		}
			return TRUE;
	}
	strText=GetText();
	if(StartPos==7||StartPos==8)
	{
		if(Month!=2)
		{
			if(nchar>'3')
				{
					MessageBeep((UINT)-1);
					return FALSE;
				}
				else if(nchar=='3')
				{
					strText=GetText();
					if(strText.GetAt(9)>'0')
					{
						if(Month==4||Month==6||Month==9||Month==11)
						{
							strText.SetAt(9,'0');
						}
						else
							strText.SetAt(9,'1');
						SetText(strText);
						SetSel(StartPos,StartPos);
						return TRUE;
					}			
				}
		}
		else
		{
			if(nchar>'2')
			{
				MessageBeep((UINT)-1);
				return FALSE;
			}
			else if(nchar=='2')
			{			
				strText=GetText();
				if(strText.GetAt(9)>'8')
				{
					if(Leap)
					{
						strText.SetAt(9,'9');
					}
					else
						strText.SetAt(9,'8');
					SetText(strText);
					SetSel(StartPos,StartPos);
					return TRUE;
				}
			}
		}
		return TRUE;				
	}
	if(StartPos==9)
	{
		strText=GetText();
		if(strText.GetAt(8)=='3')
		{
			if(Month==4||Month==6||Month==9||Month==11)
			{
				if(nchar>'0')
				{
					MessageBeep((UINT)-1);
					return FALSE;
				}	
			}
			else if(Month!=2)
			{
				if(nchar>'1')
				{
					MessageBeep((UINT)-1);
					return FALSE;
				}
			}
		}
		else if(strText.GetAt(8)=='2')
		{
			if(Month==2)
			{
				if(Leap)
				{
					if(nchar>'9')
					{
						MessageBeep((UINT)-1);
						return FALSE;
					}
				}
				else
				{
					if(nchar>'8')
					{
						MessageBeep((UINT)-1);
						return FALSE;
					}					
				}
			}
		}
		return TRUE;
	}
	return TRUE;
}
BOOL CMyDateEdit::CheckTime(char nchar,int StartPos,int EndPos)
{
	CString strText;
	if(m_isTime)
	{
		if(StartPos==2||StartPos==3||StartPos==5||StartPos==6)
		{
			if (nchar>'5')
			{
				MessageBeep((UINT)-1);
				return FALSE;
			}
		}	
		if(StartPos==0)
		{
			if (nchar>'2')
			{
				MessageBeep((UINT)-1);
				return FALSE;
			}
			if(nchar=='2')
			{
				if(GetText().GetAt(1)>'3')
				{
					strText=GetText();
					strText.SetAt(1,'3');
					SetWindowText(strText);
					SetSel(StartPos,EndPos);
					return TRUE;
				}
			}
		}	
		if(StartPos==1)
		{
			if(GetText().GetAt(0)=='2')
			{
				if(nchar>'3')
				{
					MessageBeep((UINT)-1);
					return FALSE;			
				}

			}
		}
	}
	if(m_isDateTime)
	{
		if(StartPos==13||StartPos==14||StartPos==16||StartPos==17)
		{
			if (nchar>'5')
			{
				MessageBeep((UINT)-1);
				return FALSE;
			}
		}
		if(StartPos==10||StartPos==11)
		{
			if (nchar>'2')
			{
				MessageBeep((UINT)-1);
				return FALSE;
			}
			if(nchar=='2')
			{
				if(GetText().GetAt(12)>'3')
				{
					strText=GetText();
					strText.SetAt(12,'3');
					SetWindowText(strText);
					SetSel(StartPos,EndPos);
					return TRUE;
				}
			}
		}	
		if(StartPos==12)
		{
			if(GetText().GetAt(11)=='2')
			{
				if(nchar>'3')
				{
					MessageBeep((UINT)-1);
					return FALSE;			
				}

			}
		}
	}	
	return TRUE;
}
LONG CMyDateEdit::OnCut(UINT, LONG)
{
	return 0;
}
// Clears the current selection.
LONG CMyDateEdit::OnClear(UINT wParam, LONG lParam)
{
	return 0;
}
// Pastes the text from the clipboard onto the current selection.
LONG CMyDateEdit::OnPaste(UINT, LONG)
{
	return 0;
}
void CMyDateEdit::SetClassType(int intStyle)
{
 Reset();
 if(intStyle ==edit_date)       //日期
 {
  m_bUseMask = TRUE;
  m_isDate = TRUE; //added this
  m_strMask = _T("0000-00-00");    //掩码
  m_strLiteral = _T("____-__-__"); //掩码有效设置 有效_ 其它-
         m_str = _T("          "); //数据保存空间
   SetWindowText(_T("    -  -  "));
 }
 else if(intStyle ==edit_time)   //时间
 {
  m_bUseMask = TRUE;
  m_isTime = TRUE; 
       m_strMask = _T("00:00:00");
    m_strLiteral = _T("__:__:__");
           m_str = _T("        ");
     SetWindowText(_T("  :  :  "));
 }
 else if(intStyle ==edit_datetime) //日期+时间
 {
   m_bUseMask = TRUE;
   m_isDateTime = TRUE;
      m_strMask = _T("0000-00-00 00:00:00");
   m_strLiteral = _T("____-__-__ __:__:__");
          m_str = _T("                   ");
    SetWindowText(_T("    -  -     :  :  "));
 }
}
CString CMyDateEdit::GetText()
{
	GetWindowText(m_str);
	return m_str;
}
void CMyDateEdit::SetText(CString strText)
{
	int i,j;
	CString str=strText;
	str.TrimLeft();
	str.TrimRight();
	if(str=="") 
	{
		SetDefault();
		return;
	}
	if(m_isDate)
	{
		if(strText.GetLength()<10) return;
		CString strValue=strText.Left(10);
		for (i=0;i<strValue.GetLength();i++)
		{
			if(i==4||i==7)
			{
				strValue.SetAt(i,'-');
			}
			else if(!isdigit(strValue.GetAt(i))&&strValue.GetAt(i)!=' ')
			{
				strValue.SetAt(i,'0');
			}
		}
		SetWindowText(strValue);
		m_str=strValue;
	}
	if(m_isDateTime)
	{
		if(strText.GetLength()<19) return;
		CString strValue=strText.Left(19);
		for (i=0;i<10;i++)
		{
			if(i==4||i==7)
			{
				strValue.SetAt(i,'-');
			}
			else if(!isdigit(strValue.GetAt(i))&&strValue.GetAt(i)!=' ')
			{
				strValue.SetAt(i,'0');
			}
		}	
		strValue.SetAt(10,' ');
		for (j=11;j<19;j++)
		{
			if(j==13||j==16)
			{
				strValue.SetAt(j,':');
			}
			else if(!isdigit(strValue.GetAt(j))&&strValue.GetAt(j)!=' ')
			{
				strValue.SetAt(j,'0');
			}
		}
		m_str=strValue;
		SetWindowText(strValue);
	}
	if(m_isTime)
	{
		if(strText.GetLength()<8) return;
		CString strValue=strText.Left(8);
		for (j=0;j<8;j++)
		{
			if(j==2||j==5)
			{
				strValue.SetAt(j,':');
			}
			else if(!isdigit(strValue.GetAt(j))&&strValue.GetAt(j)!=' ')
			{
				strValue.SetAt(j,'0');

			}
		}
		m_str=strValue;
		SetWindowText(strValue);
	}

}
void CMyDateEdit::Reset()
{
	m_bUseMask=FALSE;
	m_strMask=_T("");
	m_strLiteral=_T("");
	m_bMaskKeyInProgress=FALSE;
	m_strMaskLiteral=_T("");
	m_str = _T("");
	m_isDate = FALSE;//是否是日期格式
	m_isTime = FALSE;//是否是时间格式
	m_isDateTime = FALSE;//是否是日期时间格式
}

void CMyDateEdit::SendChar(UINT nChar)
{
	m_bMaskKeyInProgress=TRUE;
#ifdef WIN32
	AfxCallWndProc(this,m_hWnd,WM_CHAR,nChar,1);
#else
	SendMessage(WM_CHAR,nChar,1);
#endif
	m_bMaskKeyInProgress=FALSE;
}
void CMyDateEdit::SetDefault()
{
 if (m_isDate)
  SetClassType(edit_date);
 else if (m_isTime)
  SetClassType(edit_time);
 else if (m_isDateTime)
 SetClassType(edit_datetime);
}
void CMyDateEdit::SetNow()
{
 COleDateTime tNow = COleDateTime::GetCurrentTime();
 if (m_isDate)
  SetWindowText(tNow.Format("%Y-%m-%d"));
 else if (m_isTime)
  SetWindowText(tNow.Format("%H:%M:%S"));
 else if (m_isDateTime)
  SetWindowText(tNow.Format("%Y-%m-%d %H:%M:%S"));
}
void CMyDateEdit::SetTimeValue(COleDateTime tm)
{
 if (m_isDate)
  SetWindowText(tm.Format("%Y-%m-%d"));
 else if (m_isTime)
  SetWindowText(tm.Format("%H:%M:%S"));
 else if (m_isDateTime)
  SetWindowText(tm.Format("%Y-%m-%d %H:%M:%S"));
}
void CMyDateEdit::SetDetectText(CString strText)
{
	CString strYear,strMonth,strDay,strHour,strMin,strSec;
	COleDateTime tm;
	int i=-1;
	int Year;
	BOOL Leap=FALSE;
	if(m_isDate && strText.GetLength()>=10)
	{
		strYear=strText.Mid(0,4);
		strMonth=strText.Mid(5,2);
		strDay=strText.Mid(8,2);
		strHour="00";
		strMin="00";
		strSec="00";
	}
	if(m_isDateTime && strText.GetLength()>=19)
	{
		strYear=strText.Mid(0,4);
		strMonth=strText.Mid(5,2);
		strDay=strText.Mid(8,2);
		strHour=strText.Mid(11,2);
		strMin=strText.Mid(14,2);
		strSec=strText.Mid(17,2);
	}
	if(m_isTime&& strText.GetLength()>=8)
	{
		strYear="1960";
		strMonth="01";
		strDay="01";
		strHour=strText.Mid(0,2);
		strMin=strText.Mid(3,2);
		strSec=strText.Mid(6,7);
	}
	if(atoi(strYear)<1900 ||atoi(strYear)>2100) strYear="1900";
	if(atoi(strMonth)>12||atoi(strMonth)<1) strMonth="01";
	if(atoi(strDay)>31||atoi(strDay)<1) strDay="01";
	if(atoi(strHour)>23||atoi(strHour)<0) strHour="00";
	if(atoi(strMin)>59||atoi(strMin)<0) strMin="00";
	if(atoi(strSec)>59||atoi(strSec)<0) strSec="00";
	Year=atoi(strYear);
	if((Year%4)==0)
	{
		if(((Year%100)==0)&&((Year%400)!=0))
			Leap=FALSE;
		else
			Leap=TRUE;
	}
	else
		Leap=FALSE;
	if(atoi(strMonth)==1||atoi(strMonth)==3||atoi(strMonth)==5||atoi(strMonth)==7
		||atoi(strMonth)==8||atoi(strMonth)==10||atoi(strMonth)==12)
		strDay=atoi(strDay)>31?"31":strDay;
	if(atoi(strMonth)==4||atoi(strMonth)==6||atoi(strMonth)==9||atoi(strMonth)==11)
		strDay=atoi(strDay)>30?"30":strDay;
	if(atoi(strMonth)==2)
	{
		if(Leap)
			strDay=atoi(strDay)>29?"29":strDay;
		else
			strDay=atoi(strDay)>28?"28":strDay;
	}
	i=tm.SetDateTime(atoi(strYear),atoi(strMonth),atoi(strDay),atoi(strHour),atoi(strMin),atoi(strSec));
	if(i==0)
	{
		SetTimeValue(tm);
		return;
	}
	else
	{
		SetDefault();
	}
}
CString CMyDateEdit::GetDetectText()
{
	GetWindowText(m_str);
	SetDetectText(m_str);
	GetWindowText(m_str);
	return m_str;
}
void CMyDateEdit::OnKillFocus(CWnd* pNewWnd)
{
	GetWindowText(m_str);
	SetDetectText(m_str);
	CEdit::OnKillFocus(pNewWnd);
}
