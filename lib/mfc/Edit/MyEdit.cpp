// MyEdit.cpp : implementation file
//
#include "stdafx.h"
#include "MyEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CMyEdit::CMyEdit()
{
	m_bIsReadOnly = false;
	m_nFontSize = 12;
	m_bMouseOver = false;
	m_bFocus = false;	
	m_clrNormalFrmClr = RGB( 131 ,131 ,131 );
	m_clrFocusFrmClr = RGB( 255, 215, 0 );
	m_clrTextColor = RGB( 0,0,0 );
	m_clrNormalBkColor=RGB( 255, 255, 255 );
	m_clrFocusBkColor=RGB( 255, 250, 205);
    m_TextBkBrush.CreateSolidBrush( m_clrNormalBkColor );
    m_ftEditFont.CreatePointFont( m_nFontSize*10, _T( "Arial" )); 
	m_nVMargins = 0;
}

CMyEdit::~CMyEdit()
{
    m_TextBkBrush.DeleteObject();
	m_ftEditFont.DeleteObject();
}

BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	//{{AFX_MSG_MAP(CMyEdit)
    ON_WM_CTLCOLOR_REFLECT()
	ON_WM_MOUSEMOVE()
    ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_KILLFOCUS()
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
    ON_MESSAGE( WM_MOUSELEAVE, OnMouseLeave )
END_MESSAGE_MAP()
COLORREF CMyEdit::SetBkColor( COLORREF clrBkColor )
{
	COLORREF clrTemp = m_clrNormalBkColor;
	m_clrNormalBkColor = clrBkColor;
    m_TextBkBrush.DeleteObject();
    m_TextBkBrush.CreateSolidBrush( m_clrNormalBkColor );
	RedrawWindow();
	return clrTemp;
}

COLORREF CMyEdit::SetNormalFrmClr( COLORREF clrNormalFrmClr )   // Set normal frame color
{ 
	COLORREF clrTemp = m_clrNormalFrmClr;
	m_clrNormalFrmClr = clrNormalFrmClr;
	RedrawWindow();
	return clrTemp;
}	

COLORREF CMyEdit::SetTextColor( COLORREF clrTextColor )	// Set text color
{
	COLORREF clrTemp = m_clrTextColor;
	m_clrTextColor = clrTextColor; 
	RedrawWindow();
	return clrTemp;
}

COLORREF CMyEdit::GetTextColor() const   	// Get text color
{ 
	return m_clrTextColor; 
}

COLORREF CMyEdit::GetBkColor() const 	// Get text back color
{ 
	return m_clrNormalBkColor; 
}	

COLORREF CMyEdit::GetNormalFrmClr() const      // Get normal frame color
{ 
	return m_clrNormalFrmClr; 
} 
COLORREF CMyEdit::SetFocusFrmClr( COLORREF clrFocusFrmClr ) 	// Set focus frame color
{ 
	COLORREF clrTemp = m_clrFocusFrmClr;
	m_clrFocusFrmClr = clrFocusFrmClr;
	RedrawWindow();
	return clrTemp;
}

COLORREF CMyEdit::GetFocusFrmClr() const 	 // Get focus frame color
{ 
	return m_clrFocusFrmClr; 
}
/////////////////////////////////////////////////////////////////////////////
// CMyEdit message handlers
void   CMyEdit::Draw(CDC *pDC)
{	
	CRect rcEdit;
	GetClientRect( rcEdit );
	if (m_bIsReadOnly)
	{
		rcEdit.InflateRect(0,2,0,0);
	    pDC->FrameRect(rcEdit,&CBrush(RGB(131,131 ,131)));
	}
	else    // (!m_bIsReadOnly)
	{
	
		CBrush brFrame;
	    CBrush brush;
		if ( m_bMouseOver)
		{
			brFrame.CreateSolidBrush( m_clrFocusFrmClr );
		}
		else
		{
			brFrame.CreateSolidBrush( m_clrNormalFrmClr );
		}
		

		if (m_bFocus)
		{	
			brush.CreateSolidBrush(m_clrFocusBkColor);			
		}
		else
		{
			brush.CreateSolidBrush(m_clrNormalBkColor);
		}
     	for (int i=0; i<m_nVMargins;i++)
		{
			rcEdit.InflateRect(0,1,0,1);		
			pDC->FrameRect(&rcEdit,&brush);
		}
		rcEdit.InflateRect( 1,1,1,1 );	
		pDC->FrameRect( &rcEdit, &brFrame );
		brFrame.DeleteObject();
	}	

}

void  CMyEdit::SetHMargins(UINT nLeft, UINT nRight)               //设置水平间距
{
	CEdit::SetMargins(nLeft,nRight);
}
void  CMyEdit::SetVMargins(UINT nVMargins)                       //设置垂直间距
{

	m_nVMargins = nVMargins;
	RedrawWindow();
}
HBRUSH CMyEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
    Draw(pDC);	
	pDC->SetBkMode( TRANSPARENT );
    pDC->SetTextColor( m_clrTextColor );
    return m_TextBkBrush;
}

void CMyEdit::OnMouseMove( UINT nFlags, CPoint point ) 
{
    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof( TRACKMOUSEEVENT );
    tme.dwFlags = TME_LEAVE;
    tme.hwndTrack = m_hWnd;
    if ( !m_bMouseOver )
    {
        if( _TrackMouseEvent( &tme ))
        {
            m_bMouseOver = true;
            RedrawWindow();
        }
    }
	CEdit::OnMouseMove( nFlags, point );
}

LRESULT CMyEdit::OnMouseLeave( WPARAM, LPARAM )
{
    if ( m_bMouseOver && !m_bFocus )
    {
        m_bMouseOver = false;
        RedrawWindow();
    }
	return 0;
}	

void CMyEdit::OnSetFocus(CWnd* pNewWnd) 
{
    CEdit::OnSetFocus(pNewWnd);
	if (!m_bIsReadOnly)
	{   
		m_bFocus = true;
		m_bMouseOver = true;
		m_TextBkBrush.DeleteObject(); 
		m_TextBkBrush.CreateSolidBrush( m_clrFocusBkColor );
		RedrawWindow();
	}
}

void CMyEdit::PreSubclassWindow() 
{
    CEdit::PreSubclassWindow();
	CRect rect;
	GetClientRect(&rect);
	m_ftEditFont.DeleteObject();
    m_ftEditFont.CreatePointFont( rect.Height()*5, _T( "Arial" )); 
	SetFont( &m_ftEditFont );
	if ((GetStyle()&ES_READONLY))
	{
		m_bIsReadOnly = true;
		SetBkColor(GetSysColor(COLOR_3DFACE));
		m_TextBkBrush.DeleteObject();
		m_TextBkBrush.CreateSolidBrush(GetSysColor(COLOR_3DFACE));
	}
}

BOOL CMyEdit::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
//	return CEdit::OnEraseBkgnd(pDC);
}

void CMyEdit::SetEditFont(int nSize, LPCSTR lpszFontName /* _T( "Arial" ) */)
{
    if ( 0 >= nSize || 20<= nSize)
    {
        nSize = m_nFontSize;
    }
    m_ftEditFont.DeleteObject();
    m_ftEditFont.CreatePointFont( nSize * 10, lpszFontName );
    SetFont( &m_ftEditFont );
}
void CMyEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	if (!m_bIsReadOnly)
	{ 
		m_bFocus = false;
		m_bMouseOver = false;
		m_TextBkBrush.DeleteObject(); 
		m_TextBkBrush.CreateSolidBrush( m_clrNormalBkColor );
		RedrawWindow();
	}
}


BOOL CMyEdit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
	{
		//edit按下enter或者esc时,焦点传递到,四种方法
	//	keybd_event(VK_TAB,0,0,0);
	//	::SetFocus(::GetNextWindow(m_hWnd,GW_HWNDNEXT));
    //	::SetFocus(::GetWindow(m_hWnd,GW_HWNDNEXT));
		::SetFocus(::GetNextDlgTabItem(::GetParent(m_hWnd),m_hWnd,FALSE));
	}
	ModifyStyle(0,WS_EX_ACCEPTFILES,0);
	return CEdit::PreTranslateMessage(pMsg);
}
BOOL CMyEdit::ModifyStyle( DWORD dwRemove, DWORD dwAdd, UINT nFlags/* = 0*/ )
{
	if (dwRemove & ES_READONLY)
	{
		m_bIsReadOnly = false;
	}
    if (dwAdd & ES_READONLY)
    {
		m_bIsReadOnly = true;
    }
	return CWnd::ModifyStyle(dwRemove,dwAdd,nFlags);
}
BOOL  CMyEdit::ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags/* = 0*/)
{
	if (dwRemove & ES_READONLY)
	{
		m_bIsReadOnly = false;
	}
    if (dwAdd & ES_READONLY)
    {
		m_bIsReadOnly = true;
    }
	return CWnd::ModifyStyleEx(dwRemove,dwAdd,nFlags);
}

void CMyEdit::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	char *lpszfilename=new char[512];
	int nfilecount;
	int i;
	nfilecount=::DragQueryFile(hDropInfo,0xffffffff,NULL,512);   
	for (i=0; i < nfilecount; i++)
	{ 
		UINT nchars=::DragQueryFile(hDropInfo,i,&lpszfilename[0],512); 
		CString str(&lpszfilename[0],nchars); 
		SetWindowText(str);		
	} 
	::DragFinish(hDropInfo); //释放内存 
    delete []lpszfilename;
	CEdit::OnDropFiles(hDropInfo);
}

void CMyEdit::EnableDropFiles(BOOL bEnable /* = TRUE */)
{
	if (bEnable)
	{
		ModifyStyleEx(0,WS_EX_ACCEPTFILES,0);
	}
}
