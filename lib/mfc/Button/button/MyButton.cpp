// MyButton.cpp : implementation file
//

#include "stdafx.h"
#include "MyButton.h"
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyButton

CMyButton::CMyButton()
{	
	m_bOver  = FALSE;	
	m_bTracked = FALSE;	
	m_strPathNormal = "img//DefaultBtnNormal.png";
	m_strPathHover = "img//DefaultBtnHover.png";
	m_bIsDrawImgBtn=FALSE;

	m_clrTodStartNormal=Color(255, 238, 238, 238);
	m_clrBottomEndNormal=Color(255, 208,208,208);
	m_clrBorderNormal=RGB(160,160,160);
	m_clrTodStartHover=Color(255,225,237,249);
	m_clrBottomEndHover=Color(200,224,236,248);
	m_clrBorderHover=RGB(162,203,236);
}

CMyButton::~CMyButton()
{
}


BEGIN_MESSAGE_MAP(CMyButton, CButton)
	//{{AFX_MSG_MAP(CMyButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_ERASEBKGND()
	ON_MESSAGE(BM_SETSTYLE, OnSetStyle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyButton message handlers

//添加Owner Draw属性
void CMyButton::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0, BS_OWNERDRAW);

	//设置按钮的有效区域,消除残角
	CRgn rgn;
	RECT rect;
	GetClientRect(&rect);

	//有效区域是一个角半径为5的圆角矩形
	rgn.CreateRoundRectRgn(rect.left,rect.top,rect.right,rect.bottom,5,5);
	SetWindowRgn(rgn,TRUE);
	rgn.DeleteObject();
	CButton::PreSubclassWindow();
}

#ifndef BS_TYPEMASK
#define BS_TYPEMASK SS_TYPEMASK
#endif
LRESULT CMyButton::OnSetStyle(WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(BM_SETSTYLE,
		(wParam & ~BS_TYPEMASK) | BS_OWNERDRAW, lParam);
} // End of OnSetStyle

void CMyButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bTracked)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracked = _TrackMouseEvent(&tme);
	}
	
	CButton::OnMouseMove(nFlags, point);
}


LRESULT CMyButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bOver = FALSE;
	m_bTracked = FALSE;
	Invalidate();
	return 0;
}

LRESULT CMyButton::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	m_bOver = TRUE;
	Invalidate();
	return 0;
}


void CMyButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//从lpDrawItemStruct获取控件的相关信息
	CRect rect =  lpDrawItemStruct->rcItem;
	CDC *pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
	int nSaveDC=pDC->SaveDC();
	UINT state = lpDrawItemStruct->itemState;

	TCHAR strText[MAX_PATH + 1];
	::GetWindowText(m_hWnd, strText, MAX_PATH);
	BOOL bFocus=m_bOver;   //按钮是否被选中（点击或tab或默认）
	CString strImgPath = "" ;
	
	//获取按钮的状态
	if (state & ODS_FOCUS || state & ODS_SELECTED || state & ODS_DEFAULT)
	{
		bFocus = TRUE;
	}
	
	if (m_bIsDrawImgBtn)
	{
		if (bFocus)  //有焦点或鼠标经过
		{
			strImgPath = m_strPathHover;
		}
		else
		{
			strImgPath = m_strPathNormal;
		}
		Graphics gs(pDC->m_hDC);
		Image img(strImgPath.AllocSysString());
		gs.DrawImage(&img,0,0,rect.Width(),rect.Height());
	}
	else
	{		
		Color clrStart,clrEnd;
		COLORREF clrBorder;
		if (bFocus)
		{	
			clrStart=m_clrTodStartHover;
			clrEnd=m_clrBottomEndHover;
			clrBorder=m_clrBorderHover;

		}
		else
		{
			clrStart=m_clrTodStartNormal;
			clrEnd=m_clrBottomEndNormal;	
			clrBorder=m_clrBorderNormal;
		}
		int cxBorder=GetSystemMetrics(SM_CXEDGE);
		int cyBorder=GetSystemMetrics(SM_CYEDGE);
		Graphics myGraphics(pDC->m_hDC);	
		LinearGradientBrush linGrBrush(
			Rect(0,0,rect.Width()/*-cxBorder*/,rect.Height()/*-cyBorder*/),
			clrStart,//Color(255, 238, 238, 238), 
			clrEnd,//Color(200, 208,208,208),   
			LinearGradientModeVertical);
		myGraphics.FillRectangle(&linGrBrush, 1,1,rect.Width()-cxBorder,rect.Height()-cyBorder); 
		
		// 	CRgn rgn;
		// 	rgn.CreateRoundRectRgn(rect.left,rect.top,rect.right,rect.bottom,2,2);
		// 	myGraphics.FillRegion(&linGrBrush,&Region((HRGN)rgn.m_hObject));
		
		CPen pen;
		pen.CreatePen(PS_SOLID,1,clrBorder);//RGB(162,203,236)
		pDC->SelectObject(&pen);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->RoundRect(&rect,CPoint(3,3));
		
		//	pDC->SetPixel(rect.right-2,rect.bottom-2,clrEnd.ToCOLORREF());
	}
	//显示按钮的文本
	if (strText!=NULL)
	{
		CFont* pFont = GetFont();
		CFont* pOldFont = pDC->SelectObject(pFont);
		CSize szExtent = pDC->GetTextExtent(strText, lstrlen(strText));
		CPoint pt( rect.CenterPoint().x - szExtent.cx / 2+1, rect.CenterPoint().y - szExtent.cy / 2+1);
		if (state & ODS_SELECTED) 
			pt.Offset(1, 1);
		int nMode = pDC->SetBkMode(TRANSPARENT);
		//输出首字母带下划线效果文本
		if (state & ODS_DISABLED)
			pDC->DrawState(pt, szExtent, strText, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
		else
			pDC->DrawState(pt, szExtent, strText, DSS_NORMAL, TRUE, 0, (HBRUSH)NULL);

		pDC->SelectObject(pOldFont);
		pDC->SetBkMode(nMode);
	}	
	
	pDC->RestoreDC(nSaveDC);
}

BOOL CMyButton::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
//	return CButton::OnEraseBkgnd(pDC);
}

BOOL CMyButton::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_LBUTTONDBLCLK)
	{
		pMsg->message = WM_LBUTTONDOWN;
	}
	return CButton::PreTranslateMessage(pMsg);
}

void CMyButton::SetBtnBkImgPath(CString strNormal,CString strHover)
{
	if (m_strPathNormal!=strNormal || m_strPathHover!=strHover)
	{
		m_strPathNormal=strNormal;
		m_strPathHover=strHover;
		Invalidate();
	}
}

void CMyButton::SetDrawImgBtn(BOOL bDrawImgBtn /*= TRUE*/)
{
	if (m_bIsDrawImgBtn!=bDrawImgBtn)
	{
		m_bIsDrawImgBtn = bDrawImgBtn;
		Invalidate();
	}
}

void CMyButton::SetNormalColors(Color clrTodStartNormal,Color clrBottomEndNormal,COLORREF clrBorderNormal)
{
	m_clrTodStartNormal=clrTodStartNormal;
	m_clrBottomEndNormal=clrBottomEndNormal;
	m_clrBorderNormal=clrBorderNormal;
	Invalidate();
}
void CMyButton::SetHoverColors(Color clrTodStartHover,Color clrBottomEndHover,COLORREF clrBorderHover)
{
	m_clrTodStartHover=clrTodStartHover;
	m_clrBottomEndHover=clrBottomEndHover;
	m_clrBorderHover=clrBorderHover;
	Invalidate();
}