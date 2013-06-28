///////////////////////////////////////////////////////////////////////////////
//
// ListBoxXP.cpp : implementation file
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DialogXP.h"
#include "Draw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDialogXP::CDialogXP(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	fTitleBar.SetFaceName("tahoma");
	fTitleBar.SetHeight(13);
	fTitleBar.SetFontColor(RGB(0,0,0));
	//{{AFX_DATA_INIT(CDialogXP)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogXP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogXP)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDialogXP, CDialog)
    //{{AFX_MSG_MAP(CDialogXP)
    ON_WM_NCPAINT()
	ON_WM_NCLBUTTONUP()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCHITTEST()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
CDialogXP::~CDialogXP ()
{
}

////////////////////////////////////////////////////////////////////////////////
void CDialogXP::PreSubclassWindow ()
{
    ModifyStyleEx (WS_EX_CLIENTEDGE, 0);
    ModifyStyle (WS_SYSMENU, 0);
    ModifyStyle (0, WS_BORDER, SWP_FRAMECHANGED);
}


////////////////////////////////////////////////////////////////////////////////
void CDialogXP::OnNcPaint ()
{
    Default();
    CWindowDC cDC (this);
    CWindowRect rc (this);
    CPenDC pen (cDC, ::GetSysColor (COLOR_HIGHLIGHT));

    cDC.SelectObject (::GetStockObject (NULL_BRUSH));
    cDC.Rectangle (0, 0, rc.Width(), rc.Height());
}

void CDialogXP::OnNcLButtonUp(UINT nFlags, CPoint point) 
{
	switch(nFlags)
	{
		case HTBORDER:			// In the border of a window that does not have a sizing border 
			break;
		case HTBOTTOM:			// In the lower horizontal border of a resizable window (the user can click the mouse to resize the window vertically) 
			break;
		case HTBOTTOMLEFT:		// In the lower-left corner of a border of a resizable window (the user can click the mouse to resize the window diagonally) 
			break;
		case HTBOTTOMRIGHT:		// In the lower-right corner of a border of a resizable window (the user can click the mouse to resize the window diagonally) 
			break;
		case HTCAPTION:			// In a title bar 
			break;
		case HTCLIENT:			// In a client area 
			break;
		case HTCLOSE:			// In a close button 
			break;
		case HTERROR:			// On the screen background or on a dividing line between windows (same as HTNOWHERE, except that the DefWindowProc function produces a system beep to indicate an error) 
			break;
		case HTHELP:			// In a Help button 
			break;
		case HTHSCROLL:			// In a horizontal scroll bar 
			break;
		case HTLEFT:			// In the left border of a resizable window (the user can click the mouse to resize the window horizontally) 
			break;
		case HTMENU:			// In a menu 
			break;
		case HTMAXBUTTON:		// In Maximize button 
			break;
		case HTMINBUTTON:		// In Minimize button 
			break;
		case HTNOWHERE:			// On the screen background or on a dividing line between windows 
			break;
		case HTRIGHT:			// In the right border of a resizable window (the user can click the mouse to resize the window horizontally) 
			break;
		case HTSIZE:			// In a size box (same as HTGROWBOX) 
			break;
		case HTSYSMENU:			// In a System menu or:	In a Close button: In a child window 
			break;
		case HTTOP:				// In the upper horizontal border of a window 
			break;
		case HTTOPLEFT:			// In the upper-left corner of a window border 
			break;
		case HTTOPRIGHT:		// In the upper rig		case HT corner of a window border 
			break;
		case HTTRANSPARENT:		// In a window currently covered by another window: In the same thread (the message will be sent to underlying windows: In the same thread until one of them returns a code that is not HTTRANSPARENT) 
			break;
		case HTVSCROLL:			// In the vertical scroll bar 
			break;
		default:
			break;
	}
	CDialog::OnLButtonUp(nFlags, point);
}

void CDialogXP::OnNcLButtonDown(UINT nFlags, CPoint point) 
{
	if(MouseOnClose())
	{
		CDialog::OnCancel();
		return;
	}
	if(MouseOnMinimize())
	{
		ShowWindow(SW_MINIMIZE);
		return;
	}
	if(MouseOnMaximize())
	{
		ShowWindow(IsZoomed() ? SW_RESTORE : SW_MAXIMIZE);
		return;
	}
	switch(nFlags)
	{
		case HTBORDER:			// In the border of a window that does not have a sizing border 
			break;
		case HTBOTTOM:			// In the lower horizontal border of a resizable window (the user can click the mouse to resize the window vertically) 
			break;
		case HTBOTTOMLEFT:		// In the lower-left corner of a border of a resizable window (the user can click the mouse to resize the window diagonally) 
			break;
		case HTBOTTOMRIGHT:		// In the lower-right corner of a border of a resizable window (the user can click the mouse to resize the window diagonally) 
			break;
		case HTCAPTION:			// In a title bar 
			break;
		case HTCLIENT:			// In a client area 
			break;
		case HTCLOSE:			// In a close button 
			break;
		case HTERROR:			// On the screen background or on a dividing line between windows (same as HTNOWHERE, except that the DefWindowProc function produces a system beep to indicate an error) 
			break;
		case HTHELP:			// In a Help button 
			break;
		case HTHSCROLL:			// In a horizontal scroll bar 
			break;
		case HTLEFT:			// In the left border of a resizable window (the user can click the mouse to resize the window horizontally) 
			break;
		case HTMENU:			// In a menu 
			break;
		case HTMAXBUTTON:		// In Maximize button 
			break;
		case HTMINBUTTON:		// In Minimize button 
			break;
		case HTNOWHERE:			// On the screen background or on a dividing line between windows 
			break;
		case HTRIGHT:			// In the right border of a resizable window (the user can click the mouse to resize the window horizontally) 
			break;
		case HTSIZE:			// In a size box (same as HTGROWBOX) 
			break;
		case HTSYSMENU:			// In a System menu or:	In a Close button: In a child window 
			break;
		case HTTOP:				// In the upper horizontal border of a window 
			break;
		case HTTOPLEFT:			// In the upper-left corner of a window border 
			break;
		case HTTOPRIGHT:		// In the upper rig		case HT corner of a window border 
			break;
		case HTTRANSPARENT:		// In a window currently covered by another window: In the same thread (the message will be sent to underlying windows: In the same thread until one of them returns a code that is not HTTRANSPARENT) 
			break;
		case HTVSCROLL:			// In the vertical scroll bar 
			break;
		default:
			break;
	}
	CDialog::OnLButtonDown(nFlags, point);
}

CSize CDialogXP::GetCaptionButtonSize()
{
	CSize sz;
	sz.cx=GetSystemMetrics(SM_CXSIZE);
	sz.cy=GetSystemMetrics(SM_CYSIZE);
	return sz;
}

CRect CDialogXP::GetCaptionRect()
{
	CRect rc;
	DWORD dwStyle = this->GetStyle();
	CSize szFrame = (dwStyle & WS_THICKFRAME) ?
		CSize(GetSystemMetrics(SM_CXSIZEFRAME),
			   GetSystemMetrics(SM_CYSIZEFRAME)) :
		CSize(GetSystemMetrics(SM_CXFIXEDFRAME),
				GetSystemMetrics(SM_CYFIXEDFRAME));

	int cxIcon = GetSystemMetrics(SM_CXSIZE); // width of caption icon/button

	// Compute rectangle
	GetWindowRect(rc);				// window rect in screen coords
	rc-= CPoint(rc.left, rc.top);		// shift origin to (0,0)
	rc.left  += szFrame.cx;			// frame
	rc.right -= szFrame.cx;			// frame
	rc.top   += szFrame.cy;			// top = end of frame
	rc.bottom = rc.top + GetSystemMetrics(SM_CYCAPTION)  // height of caption
		- GetSystemMetrics(SM_CYBORDER);				  // minus gray shadow border
	return rc;
}

void CDialogXP::PaintCaption(BOOL bEntire)
{
    Default();
    CWindowDC cDC (this);
    CWindowRect rc (this);
    CPenDC pen (cDC, ::GetSysColor (COLOR_HIGHLIGHT));

	CRect r=GetCaptionRect();
	if(bEntire)
	{
		CBrush br;
		br.CreateSolidBrush(LightenColor(GetSysColor(COLOR_BTNFACE),0.75));
		cDC.FillRect(r,&br);
		br.DeleteObject();
		br.CreateSolidBrush(DarkenColor(GetSysColor(COLOR_HIGHLIGHT),0.75));
		cDC.FrameRect(r,&br);
		br.DeleteObject();
	}
	UINT wd=PaintCaptionButtons(CSize(r.Width(),r.Height()),cDC);
	if(bEntire)
	{
		if(GetIcon(0))
			r.left+=20;
		else
			r.left+=4;
		r.right-=wd;
		cDC.SelectObject(fTitleBar);
		CString str;
		GetWindowText(str);
		cDC.SetTextColor(fTitleBar.GetFontColor());
		if(GetIcon(0))
			DrawIconEx(cDC.m_hDC,GetFrameSize().cx,4,GetIcon(0),16,16,0,NULL,DI_NORMAL);
		cDC.DrawText(str,r,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}
}

LRESULT CDialogXP::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message)
	{
		case WM_NCPAINT:
		case WM_NCACTIVATE:
		case WM_ACTIVATEAPP:
		case WM_PAINT:
		case WM_SYSCOMMAND:
		case WM_INITMENU:
		case WM_WINDOWPOSCHANGING:
		case WM_WINDOWPOSCHANGED:
		case WM_ACTIVATE:
		case WM_CAPTURECHANGED:
		case 877: // WM_FLOATSTATUS (in Afxpriv.h)
			PaintCaption(1);
		break;
		default:
			break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

UINT CDialogXP::OnNcHitTest(CPoint point) 
{
	PaintCaption(0);
	return CDialog::OnNcHitTest(point);
}

UINT CDialogXP::PaintCaptionButtons(CSize szRect,CWindowDC& dc,UINT ms)
{
	DWORD dwStyle = this->GetStyle();
	if (!(dwStyle & WS_CAPTION))
		return 0;

	int cxIcon = GetSystemMetrics(SM_CXSIZE);
	int cyIcon = GetSystemMetrics(SM_CYSIZE);

	// Draw caption buttons. These are all drawn inside a rectangle
	// of dimensions SM_CXSIZE by SM_CYSIZE
	CRect rc(0, 0, cxIcon, cyIcon);
	rc += CPoint(szRect.cx-cxIcon, 3);	// move right

	// Close box has a 2 pixel border on all sides but left, which is zero
	rc.DeflateRect(0,2);
	rc.right -= 2;
	CPoint pc;
	GetCursorPos(&pc);
	ScreenToClient(&pc);
	bool less=0;
	if(pc.y<0)
	{
		less=1;
		pc.y+=GetCaptionSize().cy;
		pc.y+=GetFrameSize().cy;
		pc.x+=GetFrameSize().cx;
	}

	if((rc.PtInRect(pc))&&(less))
		DrawCloseButton(dc,rc,ms+1);
	else
		DrawCloseButton(dc,rc,0);

	BOOL bMaxBox = dwStyle & WS_MAXIMIZEBOX;
	if (bMaxBox || (this->GetExStyle() & WS_EX_CONTEXTHELP))
	{
		rc -= CPoint(cxIcon, 0);
		if((rc.PtInRect(pc))&&(less))
			DrawMaximizeButton(dc,rc,ms+1);
		else
			DrawMaximizeButton(dc,rc,0);
	}

	if (dwStyle & WS_MINIMIZEBOX) {
		rc -= CPoint(cxIcon,0);
		if((rc.PtInRect(pc))&&(less))
			DrawMinimizeButton(dc,rc,ms+1);
		else
			DrawMinimizeButton(dc,rc,0);
	}
	return szRect.cx - rc.left - 2;
}

void CDialogXP::DrawCloseButton(CWindowDC &dc, CRect rc, UINT state)
{
	CBrush br;
	if(state==0)
		br.CreateSolidBrush(GetSysColor (COLOR_HIGHLIGHT));
	else
		br.CreateSolidBrush(LightenColor(GetSysColor (COLOR_HIGHLIGHT),0.5));
	dc.FillRect(rc,&br);
	br.DeleteObject();
	br.CreateSolidBrush(GetSysColor (COLOR_INFOBK));
	dc.FrameRect(rc,&br);
	br.DeleteObject();
	char txt[5]="";
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(255,255,255));
	dc.DrawText(txt,rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

void CDialogXP::DrawMinimizeButton(CWindowDC &dc, CRect rc, UINT state)
{
	CBrush br;
	if(state==0)
		br.CreateSolidBrush(GetSysColor (COLOR_HIGHLIGHT));
	else
		br.CreateSolidBrush(LightenColor(GetSysColor (COLOR_HIGHLIGHT),0.5));
	dc.FillRect(rc,&br);
	br.DeleteObject();
	br.CreateSolidBrush(GetSysColor (COLOR_INFOBK));
	dc.FrameRect(rc,&br);
	br.DeleteObject();
	char txt[5]="";
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(255,255,255));
	dc.DrawText(txt,rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

void CDialogXP::DrawRestoreButton(CWindowDC &dc, CRect rc, UINT state)
{
	CBrush br;
	if(state==0)
		br.CreateSolidBrush(GetSysColor (COLOR_HIGHLIGHT));
	else
		br.CreateSolidBrush(LightenColor(GetSysColor (COLOR_HIGHLIGHT),0.5));
	dc.FillRect(rc,&br);
	br.DeleteObject();
	br.CreateSolidBrush(GetSysColor (COLOR_INFOBK));
	dc.FrameRect(rc,&br);
	br.DeleteObject();
	char txt[5]="";
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(255,255,255));
	dc.DrawText(txt,rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

void CDialogXP::DrawMaximizeButton(CWindowDC &dc, CRect rc, UINT state)
{
	CBrush br;
	if(state==0)
		br.CreateSolidBrush(GetSysColor (COLOR_HIGHLIGHT));
	else
		br.CreateSolidBrush(LightenColor(GetSysColor (COLOR_HIGHLIGHT),0.5));
	dc.FillRect(rc,&br);
	br.DeleteObject();
	br.CreateSolidBrush(GetSysColor (COLOR_INFOBK));
	dc.FrameRect(rc,&br);
	br.DeleteObject();
	char txt[5]="";
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(255,255,255));
	dc.DrawText(txt,rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

BOOL CDialogXP::MouseOnClose()
{
	CSize szRect=GetCaptionSize();
	DWORD dwStyle = this->GetStyle();
	if (!(dwStyle & WS_CAPTION))
		return 0;

	int cxIcon = GetSystemMetrics(SM_CXSIZE);
	int cyIcon = GetSystemMetrics(SM_CYSIZE);

	CRect rc(0, 0, cxIcon, cyIcon);
	rc += CPoint(szRect.cx-cxIcon, 3);	// move right

	rc.DeflateRect(0,2);
	rc.right -= 2;
	CPoint pc;
	GetCursorPos(&pc);
	ScreenToClient(&pc);
	if(pc.y<0)
	{
		pc.y+=GetCaptionSize().cy;
		pc.y+=GetFrameSize().cy;
		pc.x+=GetFrameSize().cx;
	}
	else
		return 0;

	return rc.PtInRect(pc);
}

BOOL CDialogXP::MouseOnMinimize()
{
	CSize szRect=GetCaptionSize();
	DWORD dwStyle = this->GetStyle();
	if (!(dwStyle & WS_CAPTION))
		return 0;
	int cxIcon = GetSystemMetrics(SM_CXSIZE);
	int cyIcon = GetSystemMetrics(SM_CYSIZE);
	CRect rc(0, 0, cxIcon, cyIcon);
	rc += CPoint(szRect.cx-cxIcon, 3);	// move right
	rc.DeflateRect(0,2);
	rc.right -= 2;
	CPoint pc;
	GetCursorPos(&pc);
	ScreenToClient(&pc);
	if(pc.y<0)
	{
		pc.y+=GetCaptionSize().cy;
		pc.y+=GetFrameSize().cy;
		pc.x+=GetFrameSize().cx;
	}
	else
		return 0;

	BOOL bMaxBox = dwStyle & WS_MAXIMIZEBOX;
	if (bMaxBox || (this->GetExStyle() & WS_EX_CONTEXTHELP))
	{
		rc -= CPoint(cxIcon, 0);
	}

	if (dwStyle & WS_MINIMIZEBOX) {
		rc -= CPoint(cxIcon-2,0);
	}
	return rc.PtInRect(pc);
}

BOOL CDialogXP::MouseOnMaximize()
{
	CSize szRect=GetCaptionSize();
	DWORD dwStyle = this->GetStyle();
	if (!(dwStyle & WS_CAPTION))
		return 0;
	int cxIcon = GetSystemMetrics(SM_CXSIZE);
	int cyIcon = GetSystemMetrics(SM_CYSIZE);
	CRect rc(0, 0, cxIcon, cyIcon);
	rc += CPoint(szRect.cx-cxIcon, 3);	// move right
	rc.DeflateRect(0,2);
	rc.right -= 2;
	CPoint pc;
	GetCursorPos(&pc);
	ScreenToClient(&pc);
	if(pc.y<0)
	{
		pc.y+=GetCaptionSize().cy;
		pc.y+=GetFrameSize().cy;
		pc.x+=GetFrameSize().cx;
	}
	else
		return 0;

	BOOL bMaxBox = dwStyle & WS_MAXIMIZEBOX;
	if (bMaxBox || (this->GetExStyle() & WS_EX_CONTEXTHELP))
	{
		rc -= CPoint(cxIcon, 0);
	}
	return rc.PtInRect(pc);
}

CSize CDialogXP::GetCaptionSize()
{
	return CSize(GetCaptionRect().Width(),GetCaptionRect().Height());
}

CSize CDialogXP::GetFrameSize()
{
	DWORD dwStyle = this->GetStyle();
	CSize szFrame = (dwStyle & WS_THICKFRAME) ?
		CSize(GetSystemMetrics(SM_CXSIZEFRAME),
			   GetSystemMetrics(SM_CYSIZEFRAME)) :
		CSize(GetSystemMetrics(SM_CXFIXEDFRAME),
				GetSystemMetrics(SM_CYFIXEDFRAME));
		return szFrame;
}

void CDialogXP::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if(IsIconic())
	{
		dc.TextOut(0,0,"Xxxxxxx");
	}
}

BOOL CDialogXP::OnEraseBkgnd(CDC* pDC) 
{
	RECT r;
	GetClientRect(&r);
	CBrush br;
	br.CreateSolidBrush(LightenColor(GetSysColor(COLOR_HIGHLIGHT),0.9));
	pDC->FillRect(&r,&br);
	br.DeleteObject();
	return 1;
}
