// MyDialog.cpp : implementation file
//

#include "stdafx.h"
#include "btn.h"
#include "MyDialog.h"
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyDialog dialog


CMyDialog::CMyDialog(UINT nIDTemplate,CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	//{{AFX_DATA_INIT(CMyDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nTopBorder=30;
	m_nBottomBorder=2;
	m_nLeftBorder=2;
	m_nRightBorder=2;
	m_rcMin=0;
//	m_rcMax=0;
	m_rcClose=0;
	m_rcIcon=0;
	m_bMinHover=FALSE;
//	m_bMaxHover=FALSE;
	m_bCloseHover=FALSE;
	m_bTracked=FALSE;
	m_bHaveStoreBgDC=FALSE;
}


void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyDialog, CDialog)
	//{{AFX_MSG_MAP(CMyDialog)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDialog message handlers

HBRUSH CMyDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CMyDialog::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//	BOOL bResult=CDialog::OnEraseBkgnd(pDC);
	if (!m_bHaveStoreBgDC)
	{
		CRect rect;
		GetClientRect(rect);
		m_bHaveStoreBgDC=TRUE;
		Color 	clrStart=Color(230,230,230);
		Color 	clrEnd=Color(255,255,255);
		
		m_StoreBgDC.DeleteDC();
		m_StoreBgDC.CreateCompatibleDC(pDC);
		CBitmap bmp;
		bmp.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
		CBitmap *pOldBmp=m_StoreBgDC.SelectObject(&bmp);
		Graphics myGraphics(m_StoreBgDC.m_hDC);	
		LinearGradientBrush linGrBrush(
			Rect(0,0,rect.Width(),rect.Height()),
			clrStart,
			clrEnd, 
			LinearGradientModeVertical);
		myGraphics.FillRectangle(&linGrBrush, 0,31,rect.Width(),rect.Height()); 
		
		Color clrCaptionStart(219,219,220);
		Color clrCaptionEnd(190,192,193);
		LinearGradientBrush linGrBrush1(
			Rect(0,0,rect.Width(),30),
			clrCaptionStart,
			clrCaptionEnd, 
			LinearGradientModeVertical);
		myGraphics.FillRectangle(&linGrBrush1, 0,0,rect.Width(),30); 
		
		DrawCloseButton(&m_StoreBgDC,DTS_NORMAL);
		DrawMinButton(&m_StoreBgDC,DTS_NORMAL);
		
		CPen MarkPen(PS_SOLID,1,RGB(158,158,158));
		CBrush *pOldBrush=(CBrush *)m_StoreBgDC.SelectStockObject(NULL_BRUSH);
		CPen *pOldPen=m_StoreBgDC.SelectObject(&MarkPen);
		m_StoreBgDC.RoundRect(m_rcMin.left-1,m_rcMin.top-5,m_rcClose.right+1,m_rcClose.bottom+1,5,5);	
		
		CPen penBorder(PS_SOLID,1,RGB(150,150,150));
		m_StoreBgDC.SelectObject(&penBorder);	
		m_StoreBgDC.MoveTo(0,30);
		m_StoreBgDC.LineTo(rect.Width(),30);
		m_StoreBgDC.MoveTo(m_rcClose.left-1,1);
		m_StoreBgDC.LineTo(m_rcClose.left-1,21);
		rect.DeflateRect(0,0,1,1);
		m_StoreBgDC.RoundRect(rect,CPoint(7,5));
		m_StoreBgDC.SelectObject(pOldPen);
		m_StoreBgDC.SelectObject(pOldBrush);
		
		
		rect.bottom=30;
		HICON hIcon=GetIcon(FALSE);
		if (hIcon)   //画Icon
		{
			DrawIconEx(m_StoreBgDC.m_hDC,5,7,hIcon,16,16,0,NULL,DI_NORMAL | DI_COMPAT);
			rect.left+=21;
			m_rcIcon.SetRect(5,7,21,23);
		}
		CString str;
		GetWindowText(str);
		if (str!="") //写标题
		{
			rect.left+=5;
			rect.top+=4;
			m_StoreBgDC.SetBkMode(TRANSPARENT);
			COLORREF OldClr=m_StoreBgDC.SetTextColor(RGB(50,50,50));
			CFont font;
			font.CreatePointFont(100,"雅黑",&m_StoreBgDC);
			CFont *pOldFont=m_StoreBgDC.SelectObject(&font);
			m_StoreBgDC.DrawText(str,rect,DT_VCENTER | DT_SINGLELINE);
			m_StoreBgDC.SelectObject(pOldFont);
			m_StoreBgDC.SetTextColor(OldClr);
			font.DeleteObject();
		}
		/*		m_StoreBgDC.SelectObject(pOldBmp);*/
		bmp.DeleteObject();
	}
	CRect rect;
	GetClientRect(rect);
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&m_StoreBgDC,0,0,SRCCOPY);
	return TRUE;
}

BOOL CMyDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	ModifyStyle(0,WS_SYSMENU | WS_MINIMIZEBOX);
	DWORD dwStyle = GetStyle();//获取旧样式  
	DWORD dwNewStyle = WS_OVERLAPPED | WS_VISIBLE| WS_SYSMENU |WS_MINIMIZEBOX
		|WS_MAXIMIZEBOX|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;  
	dwNewStyle&=dwStyle;//按位与将旧样式去掉  
	SetWindowLong(m_hWnd,GWL_STYLE,dwNewStyle);//设置成新的样式  
	DWORD dwExStyle = GetExStyle();//获取旧扩展样式  
	DWORD dwNewExStyle = WS_EX_LEFT |WS_EX_LTRREADING |WS_EX_RIGHTSCROLLBAR;  
	dwNewExStyle&=dwExStyle;//按位与将旧扩展样式去掉  
	SetWindowLong(m_hWnd,GWL_EXSTYLE,dwNewExStyle);//设置新的扩展样式  
	//告诉windows：我的样式改变了，窗口位置和大小保持原来不变！  

	CRect rcWnd;
	GetClientRect(rcWnd);
	rcWnd.InflateRect(m_nLeftBorder,m_nTopBorder,m_nRightBorder,m_nBottomBorder);
	MoveWindow(0,0,rcWnd.Width(),rcWnd.Height(),TRUE);
	CRgn rgn;
	rgn.CreateRoundRectRgn(0,0,rcWnd.Width(),rcWnd.Height(),5,5);
	SetWindowRgn(rgn,FALSE);
	CWnd *pClildWnd=GetWindow(GW_CHILD);
	CRect rcChlid;
	while (pClildWnd)
	{
		pClildWnd->GetWindowRect(rcChlid);
		pClildWnd->MoveWindow(rcChlid.left+m_nLeftBorder,rcChlid.top+m_nTopBorder,rcChlid.Width(),rcChlid.Height(),TRUE);
		pClildWnd=pClildWnd->GetWindow(GW_HWNDNEXT);
	}
	CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMyDialog::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_rcClose.PtInRect(point))
	{
		DrawCloseButton(GetDC(),DTS_CLICKED);
		m_bCloseHover=FALSE;
		return;
	}
	else if (m_rcMin.PtInRect(point))
	{
		DrawMinButton(GetDC(),DTS_CLICKED);
		m_bMinHover=FALSE;
		return;
	}
	if (point.y<=30)
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CMyDialog::DrawMinButton(CDC *pDC,int enumStatus)
{
	Color clrStart,clrEnd,clrBorder,clrCloseMark;
	CRect rc;
	GetClientRect(rc);
	if (enumStatus==DTS_NORMAL)
	{
		clrStart=Color(231,231,231);
		clrEnd=Color(227,227,227);
		clrBorder=Color(158,158,158);
		clrCloseMark=Color(128,128,128);

		clrStart=Color(219,219,220);
	    clrEnd=Color(190,192,193);
	}
	else if (enumStatus==DTS_HOVER)
	{
		clrStart=Color(241,247,252);
		clrEnd=Color(218,234,250);
		clrBorder=Color(158,158,158);
		clrCloseMark=Color(128,128,128);
	}
	else// if (enumStatus==DTS_CLICKED)
	{
		clrStart=Color(218,234,250);
		clrEnd=Color(181,213,248);
		clrBorder=Color(158,158,158);
		clrCloseMark=Color(128,128,128);
	}
	m_rcMin.SetRect(rc.right-62,1,rc.right-37,21);
	GraphicsPath path;
	int nRadius=4;
	Point point[5];
	point[0]=Point(m_rcMin.left,m_rcMin.bottom-nRadius);
	point[1]=Point(m_rcMin.left,m_rcMin.top);
	point[2]=Point(m_rcMin.right,m_rcMin.top);
	point[3]=Point(m_rcMin.right,m_rcMin.bottom);
	point[4]=Point(m_rcMin.left+nRadius,m_rcMin.bottom);
	path.AddLines(point,5);
	path.AddArc(m_rcMin.left,m_rcMin.bottom-2*nRadius,2*nRadius,2*nRadius,90,90);
	LinearGradientBrush linGrBrush(
		Rect(m_rcMin.left,m_rcMin.top,m_rcMin.Width(),m_rcMin.Height()),
		clrStart,
		clrEnd, 
		LinearGradientModeVertical);
	Graphics gs(pDC->m_hDC);
	gs.FillPath(&linGrBrush,&path);	
	
	CPoint ptCenter=m_rcMin.CenterPoint();
	if (enumStatus==DTS_CLICKED)
	{
		ptCenter.Offset(1,1);
	}
	CPen penMark(PS_SOLID,2,clrCloseMark.ToCOLORREF());
	CPen *pOldPen=pDC->SelectObject(&penMark);
	pDC->MoveTo(ptCenter.x-5,ptCenter.y+3);
	pDC->LineTo(ptCenter.x+5,ptCenter.y+3);
	pDC->MoveTo(ptCenter.x+5,ptCenter.y+3);
	pDC->LineTo(ptCenter.x-5,ptCenter.y+3);

// 	CPen penBorder(PS_SOLID,1,clrBorder.ToCOLORREF());
// 	pDC->SelectObject(&penBorder);
// 	pDC->MoveTo(m_rcMin.right+1,m_rcMin.top);
// 	pDC->LineTo(m_rcMin.right+1,m_rcMin.bottom);
	pDC->SelectObject(pOldPen);
}

void CMyDialog::DrawCloseButton(CDC *pDC,int enumStatus)
{
	Color clrStart,clrEnd,clrBorder,clrCloseMark;
	CRect rc;
	GetClientRect(rc);
	if (enumStatus==DTS_NORMAL)
	{
		clrStart=Color(231,231,231);
		clrEnd=Color(227,227,227);
		clrBorder=Color(158,158,158);
		clrCloseMark=Color(128,128,128);
		
		clrStart=Color(219,219,220);
	    clrEnd=Color(190,192,193);
	}
	else if (enumStatus==DTS_HOVER)
	{
		clrStart=Color(245,28,6);
		clrEnd=Color(207,9,7);
		clrBorder=Color(187,5,0);
		clrCloseMark=Color(255,255,255);
	}
	else// if (enumStatus==DTS_CLICKED)
	{
		clrStart=Color(207,9,7);	
		clrEnd=Color(245,28,6);
		clrBorder=Color(187,5,0);
		clrCloseMark=Color(255,255,255);
	}
	m_rcClose.SetRect(rc.right-36,1,rc.right-6,21);
	GraphicsPath path;
	int nRadius=4;
	Point point[5];
	point[0]=Point(m_rcClose.right,m_rcClose.bottom-nRadius);
	point[1]=Point(m_rcClose.right,m_rcClose.top);
	point[2]=Point(m_rcClose.left,m_rcClose.top);
	point[3]=Point(m_rcClose.left,m_rcClose.bottom);
	point[4]=Point(m_rcClose.right-nRadius,m_rcClose.bottom);
	path.AddLines(point,5);
	path.AddArc(m_rcClose.right-2*nRadius,m_rcClose.bottom-2*nRadius,2*nRadius,2*nRadius,0,90);
	LinearGradientBrush linGrBrush(
		Rect(m_rcClose.left,m_rcClose.top,m_rcClose.Width(),m_rcClose.Height()),
		clrStart,
		clrEnd, 
		LinearGradientModeVertical);
	Graphics gs(pDC->m_hDC);
	gs.FillPath(&linGrBrush,&path);	
	CPoint ptCenter=m_rcClose.CenterPoint();
	if (enumStatus==DTS_CLICKED)
	{
		ptCenter.Offset(1,1);
	}
	CPen penMark(PS_SOLID,2,clrCloseMark.ToCOLORREF());
	CPen *pOldPen=pDC->SelectObject(&penMark);
	pDC->MoveTo(ptCenter.x-5,ptCenter.y-5);
	pDC->LineTo(ptCenter.x+5,ptCenter.y+5);
	pDC->MoveTo(ptCenter.x-5,ptCenter.y+5);
	pDC->LineTo(ptCenter.x+5,ptCenter.y-5);

// 	CPen penBorder(PS_SOLID,1,clrBorder.ToCOLORREF());
// 	pDC->SelectObject(&penBorder);
// 	pDC->MoveTo(m_rcClose.left-1,m_rcClose.top);
// 	pDC->LineTo(m_rcClose.left-1,m_rcClose.bottom);
	pDC->SelectObject(pOldPen);
}

void CMyDialog::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bTracked)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = 1;
		m_bTracked = _TrackMouseEvent(&tme);
		m_bTracked=TRUE;
	}
	if (point.y<=30)
	{
		if (m_rcClose.PtInRect(point))
		{
			if (!m_bCloseHover)
			{
				m_bCloseHover=TRUE;
				DrawCloseButton(GetDC(),DTS_HOVER);
				m_bMinHover=FALSE;
				DrawMinButton(GetDC(),DTS_NORMAL);
			}
		}
		else if (m_rcMin.PtInRect(point))
		{
			if (!m_bMinHover)
			{
				m_bMinHover=TRUE;
				DrawMinButton(GetDC(),DTS_HOVER);
				m_bCloseHover=FALSE;
				DrawCloseButton(GetDC(),DTS_NORMAL);
			}
		}
		else
		{
			if (m_bCloseHover)
			{
				m_bCloseHover=FALSE;
				DrawCloseButton(GetDC(),DTS_NORMAL);
			}
			if (m_bMinHover)
			{
				m_bMinHover=FALSE;
				DrawMinButton(GetDC(),DTS_NORMAL);
			}
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}
LRESULT CMyDialog::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if (m_bCloseHover)
	{
		m_bCloseHover=FALSE;
		DrawCloseButton(GetDC(),DTS_NORMAL);
	}
	if (m_bMinHover)
	{
		m_bMinHover=FALSE;
		DrawMinButton(GetDC(),DTS_NORMAL);
	}
	m_bTracked=FALSE;
	return 0;
}

void CMyDialog::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_rcClose.PtInRect(point))
	{
		PostMessage(WM_CLOSE,0,0);
	}
	else if (m_rcMin.PtInRect(point))
	{
		ShowWindow(SW_MINIMIZE);
	}
	CDialog::OnLButtonUp(nFlags, point);
}

void CMyDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	// Do not call CDialog::OnPaint() for painting messages
}

void CMyDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (cx!=0 || cy!=0)
	{
		m_bHaveStoreBgDC=FALSE;
	}
}
