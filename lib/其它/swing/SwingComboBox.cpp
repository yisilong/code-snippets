// SwingComboBox.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "SwingComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSwingComboBox

CSwingComboBox::CSwingComboBox()
{
	nActiveBrush.CreateSolidBrush(RGB(110, 110, 110));
	nInactiveBrush.CreateSolidBrush(RGB(204, 204, 204));
	nDarkBorder.CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
	nWhiteBorder.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	nSelectedBorder.CreatePen(PS_SOLID, 1, RGB(153, 153, 204));
}

CSwingComboBox::~CSwingComboBox()
{
}


BEGIN_MESSAGE_MAP(CSwingComboBox, CComboBox)
	//{{AFX_MSG_MAP(CSwingComboBox)
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSwingComboBox message handlers

void CSwingComboBox::OnNcPaint() 
{
//	CSwingComboBox::OnNcPaint();
	// TODO: Add your message handler code here
	
	// Do not call CComboBox::OnNcPaint() for painting messages
}

void CSwingComboBox::OnPaint() 
{
	CComboBox::OnPaint();

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

	pDC = GetDC();
	GetClientRect(pRect);
	TopLeft.x = pRect.right - 18;
	TopLeft.y = pRect.top + 2;
	TopRight.x = pRect.right - 3;
	TopRight.y = pRect.top + 2;

	BottomLeft.x = pRect.right - 18;
	BottomLeft.y = pRect.bottom - 3;
	BottomRight.x = pRect.right - 3;
	BottomRight.y = pRect.bottom - 3;

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
	// Do not call CComboBox::OnPaint() for painting messages
}
