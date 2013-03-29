// PluckButton.cpp : implementation file
//

#include "stdafx.h"
#include "OwnDrawButton.h"
#include "PluckButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPluckButton

CPluckButton::CPluckButton()
{
	m_style = KEY_STYLE_DOWN;
	b_InRect = false;
}

CPluckButton::~CPluckButton()
{
}


BEGIN_MESSAGE_MAP(CPluckButton, CButton)
	//{{AFX_MSG_MAP(CPluckButton)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPluckButton message handlers

void CPluckButton::PreSubclassWindow() 
{
	CButton::PreSubclassWindow();
	ModifyStyle( 0, BS_OWNERDRAW );		//设置按钮属性为自画式
}

void CPluckButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC *pDC = CDC::FromHandle( lpDrawItemStruct->hDC );
	m_ButRect = lpDrawItemStruct->rcItem;			//获取按钮尺寸
	CPoint m_ptCentre = m_ButRect.CenterPoint();	//求按钮中心点
	switch( m_style )		//设置箭头坐标
	{
	case KEY_STYLE_DOWN:
					m_pt[0].x = m_ptCentre.x-3;
					m_pt[0].y = m_ptCentre.y-2;
					m_pt[1].x = m_ptCentre.x+4;
					m_pt[1].y = m_ptCentre.y-2;
					m_pt[2].x = m_ptCentre.x;
					m_pt[2].y = m_ptCentre.y+2;
					break;
	case KEY_STYLE_UP:
					m_pt[0].x = m_ptCentre.x-4;
					m_pt[0].y = m_ptCentre.y+2;
					m_pt[1].x = m_ptCentre.x+4;
					m_pt[1].y = m_ptCentre.y+2;
					m_pt[2].x = m_ptCentre.x;
					m_pt[2].y = m_ptCentre.y-3;
					break;
	case KEY_STYLE_LEFT:
					m_pt[0].x = m_ptCentre.x+2;
					m_pt[0].y = m_ptCentre.y-4;
					m_pt[1].x = m_ptCentre.x+2;
					m_pt[1].y = m_ptCentre.y+4;
					m_pt[2].x = m_ptCentre.x-2;
					m_pt[2].y = m_ptCentre.y;
					break;
	case KEY_STYLE_RIGHT:
					m_pt[0].x = m_ptCentre.x-1;
					m_pt[0].y = m_ptCentre.y-4;
					m_pt[1].x = m_ptCentre.x-1;
					m_pt[1].y = m_ptCentre.y+4;
					m_pt[2].x = m_ptCentre.x+3;
					m_pt[2].y = m_ptCentre.y;
					break;
	}
	int nSavedDC = pDC->SaveDC();
	VERIFY( pDC );
	if( !(::GetWindowLong(m_hWnd,GWL_STYLE) & WS_DISABLED) )
	{
		if( !b_InRect )		//鼠标不在按钮上
		{
			NormalButton( pDC );
		}
		else		//鼠标在按钮上
		{
			PassButton( pDC );
		}
	}

	pDC->RestoreDC( nSavedDC );
}

//鼠标移动消息函数
void CPluckButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	CButton::OnMouseMove(nFlags, point);

	if( !b_InRect || GetCapture()!=this )
	{
		b_InRect = true;
		SetCapture();
		Invalidate();
	}
	else
	{
		CRect rc;
		this->GetClientRect( &rc );
		if ( !rc.PtInRect(point) )
		{
			b_InRect = false;
			ReleaseCapture();
			Invalidate();
		}
	}
}

//鼠标按下消息函数
void CPluckButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	switch( m_style )	//重置按钮风格
	{
	case KEY_STYLE_DOWN:	m_style = KEY_STYLE_UP;		break;
	case KEY_STYLE_UP:		m_style = KEY_STYLE_DOWN;	break;
	case KEY_STYLE_LEFT:	m_style = KEY_STYLE_RIGHT;	break;
	case KEY_STYLE_RIGHT:	m_style = KEY_STYLE_LEFT;	break;
	}
	CButton::OnLButtonDown(nFlags, point);
	Invalidate();
}

//画正常按钮
void CPluckButton::NormalButton(CDC *pDC)
{
	CBrush Brush;
	Brush.CreateSolidBrush( RGB(178,189,200) );
	pDC->SelectObject( &Brush );
	pDC->Rectangle( &m_ButRect );		//画背景
	Brush.Detach();
	pDC->SelectStockObject( WHITE_BRUSH );
	CRgn rgn;
	rgn.CreatePolygonRgn( m_pt, 3, ALTERNATE );
	pDC->PaintRgn( &rgn );				//画箭头
}

//画鼠标经过时的按钮
void CPluckButton::PassButton(CDC *pDC)
{
	CBrush Brush;
	Brush.CreateSolidBrush( RGB(238,238,235) );
	pDC->SelectObject( &Brush );
	pDC->Rectangle( &m_ButRect );		//画背景
	Brush.Detach();
	pDC->SelectStockObject( BLACK_BRUSH );
	CRgn rgn;
	rgn.CreatePolygonRgn( m_pt, 3, ALTERNATE );
	pDC->PaintRgn( &rgn );				//画箭头
}

//////////////////////////////////////////////////////////////////////
//接口函数

//设置按钮风格
void CPluckButton::SetPluckButStyle(int Style)
{
	m_style = Style;
}

//获取按钮风格
int CPluckButton::GetPluckButStyle()
{
	return m_style;
}