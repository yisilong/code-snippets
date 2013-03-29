///////////////////////////////////////////////////////////////////////////////
//
// ListBoxXP.cpp : implementation file
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ListBoxXP.h"
#include "Draw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////
CListBoxXP::CListBoxXP ()
{
	fnt.SetFaceName("tahoma");
	fnt.SetHeight(13);
}

////////////////////////////////////////////////////////////////////////////////
CListBoxXP::~CListBoxXP ()
{
}

////////////////////////////////////////////////////////////////////////////////
void CListBoxXP::PreSubclassWindow ()
{
    ModifyStyleEx (WS_EX_CLIENTEDGE, 0);
    ModifyStyle (0, WS_BORDER, SWP_FRAMECHANGED);
	ModifyStyle(0,LBS_OWNERDRAWVARIABLE|LBS_HASSTRINGS);
    m_MouseMgr.Init (m_hWnd, MMS_NCPAINT);
	SetFont(&fnt);
}

////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CListBoxXP, CListBox)
    //{{AFX_MSG_MAP(CListBoxXP)
    ON_WM_MOUSEMOVE()
    ON_WM_NCPAINT()
	ON_WM_KILLFOCUS()
	ON_CONTROL_REFLECT(LBN_DBLCLK, OnDblclk)
	ON_WM_SETFOCUS()
	ON_CONTROL_REFLECT(LBN_SELCHANGE, OnSelchange)
	ON_WM_VSCROLL()
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
    ON_WM_MOUSEOUT()
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
void CListBoxXP::OnMouseMove (UINT, CPoint p)
{
    Default();
	BOOL o=0;
    m_MouseMgr.OnMouseMove();
	if(IsWindow(hHelper))
		return;
	if(itm==(int)ItemFromPoint(p,o))
		return;
	if(itm>=0)
	{
		RECT r;
		GetItemRect(itm,&r);
		InvalidateRect(&r);
	}
	itm=ItemFromPoint(p,o);
	if(o)
	{
		itm=-1;
		return;
	}
	RECT r;
	GetItemRect(itm,&r);
	InvalidateRect(&r);
}

///////////////////////////////////////////////////////////////////////////////
void CListBoxXP::OnMouseOut ()
{
    m_MouseMgr.OnMouseOut();
}

///////////////////////////////////////////////////////////////////////////////
void CListBoxXP::OnNcPaint ()
{
    Default();

    CWindowDC cDC (this);
    CWindowRect rc (this);
    CPenDC pen (cDC, ::GetSysColor (m_MouseMgr.MouseOver() ? COLOR_HIGHLIGHT : COLOR_3DSHADOW));

    cDC.SelectObject (::GetStockObject (NULL_BRUSH));
    cDC.Rectangle (0, 0, rc.Width(), rc.Height());
}

void CListBoxXP::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CString szBuf="";
	COLORREF col;
	if(((int)lpDrawItemStruct->itemID==edited)&&(IsWindow(hHelper)))
		return;
//	if((lpDrawItemStruct->itemAction & ODA_DRAWENTIRE)||(lpDrawItemStruct->itemAction & ODA_SELECT)||(lpDrawItemStruct->itemAction & ODA_FOCUS))
	{
		GetText(lpDrawItemStruct->itemID,szBuf);
		 SetTextColor(lpDrawItemStruct->hDC,RGB(0,0,0));
		 RECT r;
		 memcpy((void*)&r,&lpDrawItemStruct->rcItem,sizeof(RECT));
		 if(itm==(int)lpDrawItemStruct->itemID)
		 {
			HBRUSH br;
			br=::CreateSolidBrush(LightenColor(GetSysColor(COLOR_HIGHLIGHT),0.9));
			::FillRect(lpDrawItemStruct->hDC,&lpDrawItemStruct->rcItem,br);
			::DeleteObject(br);
		 }
		 else
		 {
			 FillRect(lpDrawItemStruct->hDC,&lpDrawItemStruct->rcItem,(HBRUSH)GetStockObject(WHITE_BRUSH));
		 }
		 r.left+=5;
		 r.top+=2;
		 col=RGB(255,255,255);
		 if(lpDrawItemStruct->itemState & ODS_SELECTED)
		 {
			 col=RGB(220,220,220);
			HBRUSH br;
			br=::CreateSolidBrush(LightenColor(GetSysColor(COLOR_HIGHLIGHT),0.75));
			::FillRect(lpDrawItemStruct->hDC,&lpDrawItemStruct->rcItem,br);
			::DeleteObject(br);
		 }
		 if(lpDrawItemStruct->itemState & ODS_FOCUS)
		 {
			HBRUSH br;
			br=::CreateSolidBrush(LightenColor(GetSysColor(COLOR_HIGHLIGHT),0.5));
			::FrameRect(lpDrawItemStruct->hDC,&lpDrawItemStruct->rcItem,br);
			::DeleteObject(br);
		 }
		 SetBkMode(lpDrawItemStruct->hDC,TRANSPARENT);
		 r.left-=3;
		 r.top-=2;
		 if(CountLines(szBuf)>1)
			 DrawText(lpDrawItemStruct->hDC,szBuf,strlen(szBuf),&r,DT_LEFT|DT_TOP|DT_WORDBREAK);
		 else
			 DrawText(lpDrawItemStruct->hDC,szBuf,strlen(szBuf),&r,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}
}

void CListBoxXP::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	HDC hDC;
	TEXTMETRIC tm;
	char szBuf[512]="";
	::SendMessage(this->m_hWnd, LB_GETTEXT,(WPARAM)lpMeasureItemStruct->itemID,(LPARAM)szBuf);
	hDC =::GetDC(this->m_hWnd);
	CRect r;
	GetClientRect(r);
	::GetTextMetrics(hDC, &tm);
	lpMeasureItemStruct->itemHeight=(tm.tmHeight)*CountLines(szBuf);
	::ReleaseDC(this->m_hWnd, hDC);
}

UINT CListBoxXP::CountLines(const char *szBuf)
{
	const char * fnd=strchr(szBuf,'\n');
	int cnt=0;
	while(fnd)
	{
		cnt++;
		fnd=strchr(fnd+1,'\n');
	}
	cnt++;
	return cnt;
}

void CListBoxXP::EnableItemEditing(BOOL bYes)
{
	edItm=bYes;
}

void CListBoxXP::OnKillFocus(CWnd* pNewWnd) 
{
	CListBox::OnKillFocus(pNewWnd);
}

void CListBoxXP::OnDblclk() 
{
	BOOL o=0;
	CPoint p;
	GetCursorPos(&p);
	ScreenToClient(&p);
	int Itm=(int)ItemFromPoint(p,o);
	if(itm>=0)
	{
		RECT r;
		GetItemRect(itm,&r);
		InvalidateRect(&r);
		char txt[4096]="";
		GetText(itm,txt);
		edited=itm;
		hHelper=::CreateWindow("edit",(const char *)txt,WS_VISIBLE|WS_CHILD|WS_BORDER|ES_LEFT|WS_VSCROLL|ES_MULTILINE|ES_WANTRETURN,r.left,r.top,r.right-r.left,r.bottom-r.top,this->m_hWnd,NULL,GetModuleHandle(NULL),NULL);
		::SendMessage(hHelper,WM_SETFONT,(WPARAM)(HFONT)fnt,MAKELPARAM(1,0));
		::SetFocus(hHelper);
	}
}

void CListBoxXP::OnSetFocus(CWnd* pOldWnd) 
{
	if(!pOldWnd)
	{
		CListBox::OnSetFocus(pOldWnd);
		return;
	}
	if(pOldWnd->m_hWnd==hHelper)
	{
		char txt[4096]="";
		::GetWindowText(hHelper,txt,sizeof(txt));
		DeleteString(edited);
		InsertString(edited,txt);
		OnItemRenamed(edited);
		edited=-1;;
		::DestroyWindow(hHelper);
	}
	else
	if(pOldWnd->m_hWnd==hAdd)
	{
		char txt[4096]="";
		int ni=0;
		::GetWindowText(hAdd,txt,sizeof(txt));
		if(addBefore>=0)
		{
			ni=InsertString(addBefore,txt);
		}
		else
		{
			ni=AddString(txt);
		}
		OnItemAdded(ni);
		addBefore=0;
		::DestroyWindow(hAdd);
	}
	CListBox::OnSetFocus(pOldWnd);
}

void CListBoxXP::OnSelchange() 
{
//	::DestroyWindow(hHelper);
}

void CListBoxXP::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(IsWindow(hHelper))
	{
		char txt[4096]="";
		::GetWindowText(hHelper,txt,sizeof(txt));
		DeleteString(edited);
		InsertString(edited,txt);
		SetCurSel(edited);
		OnItemRenamed(edited);
		::DestroyWindow(hHelper);
		edited=-1;;
	}
	if(IsWindow(hAdd))
	{
		char txt[4096]="";
		int ni=0;
		::GetWindowText(hAdd,txt,sizeof(txt));
		if(addBefore>=0)
		{
			ni=InsertString(addBefore,txt);
		}
		else
		{
			ni=AddString(txt);
		}
		OnItemAdded(ni);
		addBefore=0;
		::DestroyWindow(hAdd);
	}
	CListBox::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CListBoxXP::OnItemRenamed(UINT Item)
{
}

int CListBoxXP::VKeyToItem(UINT nKey, UINT nIndex) 
{
	if(nKey==VK_DELETE)
	{
		AfxMessageBox("asdfas");
		return -2;
	}
	return -1;
}

int CListBoxXP::CharToItem(UINT nKey, UINT nIndex) 
{
	if(nKey==VK_DELETE)
	{
		AfxMessageBox("asdfas");
		return -2;
	}
	return -1;
}

void CListBoxXP::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar==VK_DELETE)
	{
		if(GetSelCount())
		{
			for(int i=0;i<GetCount();i++)
			{
				if(GetSel(i))
				{
					DeleteString(i);
					i--;
				}
			}
		}
	}
	else
	if(nChar==VK_INSERT)
	{
		RECT r;
		addBefore=GetCurSel();
		if(addBefore>=0)
			GetItemRect(addBefore,&r);
		else
		{
			GetClientRect(&r);
			r.bottom=r.top+25;
		}
		hAdd=::CreateWindow("edit",(const char *)"new item",WS_VISIBLE|WS_CHILD|WS_BORDER|ES_LEFT|WS_VSCROLL|ES_MULTILINE|ES_WANTRETURN,r.left,r.top,r.right-r.left,r.bottom-r.top,this->m_hWnd,NULL,GetModuleHandle(NULL),NULL);
		::SendMessage(hAdd,WM_SETFONT,(WPARAM)(HFONT)fnt,MAKELPARAM(1,0));
		::SetFocus(hAdd);
	}
	else
	if(nChar==VK_RETURN)
	{
		int i=GetCurSel();
		if(i<GetCount()-1)
		{
			CString txt1,txt2;
			GetText(i-1,txt1);
			GetText(i,txt2);
			DeleteString(i-1);
			DeleteString(i-1);
			int n=InsertString(i,txt2);
			InsertString(i-1,txt1);
			SetCurSel(n+1);
			SetSel(n+1);
		}
	}
	else
	if(nChar==VK_BACK)
	{
		int i=GetCurSel();
		if(i>0)
		{
			CString txt1,txt2;
			GetText(i-1,txt1);
			GetText(i,txt2);
			DeleteString(i-1);
			DeleteString(i-1);
			int n=InsertString(i,txt1);
			InsertString(i-1,txt2);
			SetCurSel(n-1);
			SetSel(n-1);
		}
	}
	
	CListBox::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CListBoxXP::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{

	CListBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CListBoxXP::OnItemAdded(UINT Item)
{

}

void CListBoxXP::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(IsWindow(hHelper))
	{
		char txt[4096]="";
		::GetWindowText(hHelper,txt,sizeof(txt));
		DeleteString(edited);
		InsertString(edited,txt);
		SetCurSel(edited);
		OnItemRenamed(edited);
		::DestroyWindow(hHelper);
		edited=-1;;
	}
	if(IsWindow(hAdd))
	{
		char txt[4096]="";
		int ni=0;
		::GetWindowText(hAdd,txt,sizeof(txt));
		if(addBefore>=0)
		{
			ni=InsertString(addBefore,txt);
		}
		else
		{
			ni=AddString(txt);
		}
		OnItemAdded(ni);
		addBefore=0;
		::DestroyWindow(hAdd);
	}
	CListBox::OnHScroll(nSBCode, nPos, pScrollBar);
}
