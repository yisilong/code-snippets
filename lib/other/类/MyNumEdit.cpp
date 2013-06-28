//download by www.codefans.net
#include "stdafx.h"
#include "MyNumEdit.h"
#include<comdef.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
IMPLEMENT_DYNAMIC(CMyNumEdit,CEdit)
BEGIN_MESSAGE_MAP(CMyNumEdit, CEdit)
	//{{AFX_MSG_MAP(CMyNumEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CUT, OnCut)
	ON_MESSAGE(WM_PASTE, OnPaste)
	ON_MESSAGE(WM_CLEAR, OnClear)
END_MESSAGE_MAP()
CMyNumEdit::CMyNumEdit()
{
	m_intLeft=10;
	m_intRight=0;
	m_Point=-1;
	blnAuto=FALSE;
}
CMyNumEdit::~CMyNumEdit()
{
}
/////////////////////////////////////////////////////////////////////////////
// CMyNumEdit message handlers
void CMyNumEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CString strValue;
	int i,j;
	GetSel(i,j);
	GetWindowText(strValue);
	m_Point=strValue.Find('.');
	int m=strValue.Find('.');
	if(strValue.GetLength()==1&&strValue.GetAt(0)=='0')
	{
		if(nChar>'0'&&nChar<='9')
			{
				strValue.SetAt(0,nChar);
				SetWindowText(strValue);
				SetSel(1,1);
				return;
			}
	}
	if(i<=m&&m>=m_intLeft&&nChar!=8)
	{
		MessageBeep((UINT)-1);
		return;	
	}
	if(i==0 && nChar=='0' && m_intRight==0) 
	{
		MessageBeep((UINT)-1);
		return;
	}
	else if(i==0&&nChar=='.'&&m_intRight!=0)
	{
		MessageBeep((UINT)-1);
		return;	
	}
	else if(i==1 &&m_intRight!=0)
	{
		if(m_Point<0 && strValue.GetAt(0)=='0')
		{
			if(nChar>'0'&&nChar<='9')
			{
				strValue.SetAt(0,nChar);
				SetWindowText(strValue);
				SetSel(1,1);
				return;
			}
			else if(nChar=='0')
			{
				MessageBeep((UINT)-1);
				return;	
			}
		}
		else if(m_Point>=0&&nChar=='.')
		{
				MessageBeep((UINT)-1);
				return;	
		}
	}
	GetWindowText(strValue);
	if(nChar!=8)
	{
		if(m_intRight==0)
		{
			if(strValue.GetLength()<m_intLeft)
			{
				if(!isdigit(nChar))
				{
					MessageBeep((UINT)-1);
					return;
				}
			}
			else
			{
				MessageBeep((UINT)-1);
				return;
			}
		}
		else
		{
			if(m_Point<0)		
			{
				if(i<m_intLeft)
				{
					if(nChar=='.') m_Point=i;
					if(!isdigit(nChar) && nChar!='.')
					{
						MessageBeep((UINT)-1);
						return;
					}
				}
				else if(i>m_intLeft)
				{
					MessageBeep((UINT)-1);
					return;
				}
				else
				{
					if(nChar=='.')
						m_Point=i;
					else
					{
						MessageBeep((UINT)-1);
						return;
					}
				}
			}
			else if(m_Point>=0)
			{
				if(nChar=='.')
				{
					MessageBeep((UINT)-1);
					return;
				}
				GetSel(i,j);
				m=strValue.Find('.');
				if(i>m)
				{
					if(strValue.GetLength()-m_Point-1>=m_intRight)
					{
						MessageBeep((UINT)-1);
						return;
					}	
				}
				else
				{
					if(m>m_intLeft)
					{
						MessageBeep((UINT)-1);
						return;
					}
				}
				if(!isdigit(nChar) && nChar!='.')
				{
					MessageBeep((UINT)-1);
					return;
				}
			}			
		}
	}
	else 
	{
		int i,j;
		GetSel(i,j);
		i=i-1; 
		GetWindowText(strValue);
		if(i>=0 && strValue.GetAt(i)=='.') 
		{
			m_Point=-1;
			if(strValue.GetLength()>=m_intLeft)
			{
				strValue=strValue.Left(i+1);
				SetWindowText(strValue);
				SetSel(strValue.GetLength(),strValue.GetLength());
			}
		}
	}
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}
void CMyNumEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}
BOOL CMyNumEdit::CheckFloat(char nchar,int StartPos,int EndPos)
{	
	return TRUE;
}
BOOL CMyNumEdit::CheckInt(char nchar,int StartPos,int EndPos)
{
	return TRUE;
}
LONG CMyNumEdit::OnCut(UINT, LONG)
{
	return 0;
}
// Clears the current selection.
LONG CMyNumEdit::OnClear(UINT wParam, LONG lParam)
{
	return 0;
	CMyNumEdit::OnClear(wParam,lParam);
}
// Pastes the text from the clipboard onto the current selection.
LONG CMyNumEdit::OnPaste(UINT, LONG)
{
	return 0;
}
void CMyNumEdit::SetNumType(int intLeft,int intRight)
{
	m_intLeft=intLeft;
	m_intRight=intRight;
}
CString CMyNumEdit::GetText()
{
	GetWindowText(m_strText);
	SetText(m_strText);
	return m_strText;
}
void CMyNumEdit::SetText(CString strText)
{
	CString strValue;
	strText.TrimLeft();
	strText.TrimRight();
	int i,j;
	if(strText=="") strText="0";
	if(!isdigit(strText.GetAt(0))) strText="0";
	if(m_intRight==0)
	{
		i=strText.Find('.');
		if(i>0) strText=strText.Left(i);
		if(strText.GetLength()>=m_intLeft)
		{
			strValue=strText.Left(m_intLeft);
			for(i=0;i<m_intLeft;i++)
			{
				if(!isdigit(strValue.GetAt(i)))
					strValue.SetAt(i,'0');
			}
		}
		else
		{
			strValue=strText;
			for(i=0;i<strValue.GetLength();i++)
			{
				if(!isdigit(strValue.GetAt(i)))
					strValue.SetAt(i,'0');
			}
		}
		for(i=0;i<=strValue.GetLength();i++)
		{
			if((strValue.GetAt(0)=='0'||(!isdigit(strValue.GetAt(0)))))
			{
				strValue=strValue.Right(strValue.GetLength()-1);
				i=0;
			}
		}
		strValue.TrimLeft();
		strValue.TrimRight();
		if(strValue=="") strValue="0";
	}
	else
	{
		i=strText.Find('.');
		if(i>m_intLeft)
		{
			strValue=strText.Left(m_intLeft);
			for(i=0;i<m_intLeft;i++)
			{
				if(!isdigit(strValue.GetAt(i)))
					strValue.SetAt(i,'0');
			}
			for(i=0;i<=strValue.GetLength();i++)
			{
				if((strValue.GetAt(0)=='0'||(!isdigit(strValue.GetAt(0)))))
				{
					strValue=strValue.Right(strValue.GetLength()-1);
					i=0;
				}
			}
			strValue.TrimLeft();
			strValue.TrimRight();
			if(strValue=="") strValue="0";
			strValue=strValue+".";
			for (i=m_intRight;i>0;i--)
			{
				strValue=strValue+"0";
			}
		}
		else if(i>=0 && i<=m_intLeft)
		{
			strValue=strText.Left(i);
			for(j=0;j<i;j++)
			{
				if(!isdigit(strValue.GetAt(j)))
					strValue.SetAt(j,0);
			}
			for(i=0;i<=strValue.GetLength();i++)
			{
				if((strValue.GetAt(0)=='0'||(!isdigit(strValue.GetAt(0)))))
				{
					strValue=strValue.Right(strValue.GetLength()-1);
					i=0;
				}
			}
			strValue.TrimLeft();
			strValue.TrimRight();
			if(strValue=="") strValue="0";
			strValue=strValue+".";
			i=strText.Find('.');
			for(j=i+1;j<i+m_intRight+1;j++)
			{
				if(j<strText.GetLength()&&isdigit(strText.GetAt(j)))
					strValue=strValue+strText.Mid(j,1);
				else
					strValue=strValue+"0";
			}
		}
		if(i<0)
		{
			if(strText.GetLength()<=m_intLeft) 
			{
				strValue=strText;
				for(i=0;i<=strValue.GetLength();i++)
				{
					if((strValue.GetAt(0)=='0'||(!isdigit(strValue.GetAt(0)))))
					{
						strValue=strValue.Right(strValue.GetLength()-1);
						i=0;
					}
				}
				strValue.TrimLeft();
				strValue.TrimRight();
				if(strValue=="") strValue="0";
				strValue=strValue+".";
				for(j=m_intRight;j>0;j--)
				{
					strValue=strValue+"0";
				}
			}
			else
			{
				strValue=strText.Left(m_intLeft);
				for(i=0;i<=strValue.GetLength();i++)
				{
					if((strValue.GetAt(0)=='0'||(!isdigit(strValue.GetAt(0)))))
					{
						strValue=strValue.Right(strValue.GetLength()-1);
						i=0;
					}
				}
				strValue.TrimLeft();
				strValue.TrimRight();
				if(strValue=="") strValue="0";
				strValue=strValue+".";
				for(j=m_intRight;j>0;j--)
				{
					strValue=strValue+"0";
				}				
			}
		}
		CString strMy=strValue.Left(strValue.GetLength());
		i=strMy.Find('.');
		if(i<=0)
		{
			strMy.TrimLeft();
			strMy.TrimRight();
			if(strMy=="") strMy="0";
			strMy.Format("%s.",strMy);
			for(j=m_intRight;j>0;j--)
			{
				strMy.Format("%s0",strMy);
			}
			strValue=strMy;
		}
		for(i=0;i<strValue.GetLength();i++)
		{
			if(!isdigit(strValue.GetAt(i))&&strValue.GetAt(i)!='.')
				strValue.SetAt(i,'0');
		}

	}
	if(blnAuto)
	{
		CString str;
		i=strValue.Find('.');
		if(i<m_intLeft&&i>0)
		{
			for(;i<m_intLeft;i++)
			{
				strValue="0"+strValue;
			}
		}
		else if(i==-1&&m_intRight==0)
		{
			i=strValue.GetLength();
			for(;i<m_intLeft;i++)
			{
				strValue="0"+strValue;
			}
		}
	}
	m_Point=strValue.Find('.');
	m_strText=strValue;
	SetWindowText(m_strText);
}
void CMyNumEdit::OnKillFocus(CWnd* pNewWnd)
{
	GetWindowText(m_strText);
	SetText(m_strText);
	CEdit::OnKillFocus(pNewWnd);
}


