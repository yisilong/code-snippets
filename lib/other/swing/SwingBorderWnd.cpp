// SwingBorderWnd.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "SwingBorderWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSwingBorderWnd

CSwingBorderWnd::CSwingBorderWnd()
{
	nActiveBrush.CreateSolidBrush(RGB(110, 110, 110));
	nInactiveBrush.CreateSolidBrush(RGB(204, 204, 204));
	nDarkBorder.CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
	nWhiteBorder.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	nSelectedBorder.CreatePen(PS_SOLID, 1, RGB(153, 153, 204));
}

CSwingBorderWnd::~CSwingBorderWnd()
{
}


BEGIN_MESSAGE_MAP(CSwingBorderWnd, CWnd)
	//{{AFX_MSG_MAP(CSwingBorderWnd)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSwingBorderWnd message handlers

void CSwingBorderWnd::OnPaint() 
{
	CWnd::OnPaint();
	CPaintDC dc(this); // device context for painting
	
	CDC *pDC = GetWindowDC();
	CRect rectItem, pRect;
	GetWindowRect(pRect);
	GetClientRect(rectItem);

	rectItem.right = rectItem.left + pRect.Width();
	rectItem.bottom = rectItem.top + pRect.Height();

	CPoint TopLeft(rectItem.left, rectItem.top);
	CPoint BottomRight(rectItem.right - 1, rectItem.bottom - 1);
	CPoint TopRight(rectItem.right - 1, rectItem.top);
	CPoint BottomLeft(rectItem.left, rectItem.bottom - 1);

	pDC->SelectObject(&nDarkBorder);
	pDC->MoveTo(TopLeft);
	pDC->LineTo(TopRight);
	pDC->MoveTo(TopLeft);
	pDC->LineTo(BottomLeft);

	pDC->MoveTo(BottomLeft.x, BottomLeft.y - 1);
	pDC->LineTo(BottomRight.x, BottomRight.y - 1);
	pDC->MoveTo(BottomRight.x - 1, BottomRight.y);
	pDC->LineTo(TopRight.x - 1, TopRight.y);

	pDC->SelectObject(&nWhiteBorder);

	pDC->MoveTo(BottomLeft);
	pDC->LineTo(BottomRight);
	pDC->MoveTo(BottomRight);
	pDC->LineTo(TopRight);

	pDC->MoveTo(TopLeft.x + 1, TopLeft.y + 1);
	pDC->LineTo(TopRight.x - 1, TopRight.y + 1);
	pDC->MoveTo(TopLeft.x + 1, TopLeft.y + 1);
	pDC->LineTo(BottomLeft.x + 1, BottomLeft.y - 1);
	ReleaseDC(pDC);

	// TODO: Add your message handler code here
	
	// Do not call CWnd::OnPaint() for painting messages
}
