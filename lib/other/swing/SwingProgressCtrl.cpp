// SwingProgressCtrl.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "SwingProgressCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSwingProgressCtrl

CSwingProgressCtrl::CSwingProgressCtrl()
{
	nBkBrush.CreateSolidBrush(RGB(204, 204, 204));
	nProgressBrush.CreateSolidBrush(RGB(153, 153, 204));

	nDarkBorder.CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
	nWhiteBorder.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	nSelectedBorder.CreatePen(PS_SOLID, 1, RGB(153, 153, 204));
}

CSwingProgressCtrl::~CSwingProgressCtrl()
{
}


BEGIN_MESSAGE_MAP(CSwingProgressCtrl, CProgressCtrl)
	//{{AFX_MSG_MAP(CSwingProgressCtrl)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSwingProgressCtrl message handlers

void CSwingProgressCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	BOOL bVertical = GetStyle() & PBS_VERTICAL;

	CRect pRect;
	GetClientRect(pRect);

	pRect.DeflateRect(2, 2, 2, 2);
	CRect mProgressRect(pRect);
	int nStart, nEnd;
	GetRange(nStart, nEnd);
	int nRange = nEnd - nStart;
	int pos = GetPos();

	CPoint drawText = pRect.CenterPoint();
	CSize len = dc.GetTextExtent(m_strCaption);
	drawText.x -= (len.cx / 2);
	drawText.y -= (len.cy / 2);

	int nPrg;	
	if (!bVertical)
	{
		nPrg = (pRect.Width() * pos) / nRange;
		mProgressRect.right = mProgressRect.left + nPrg;
		
	}

	if (bVertical)
	{
		nPrg = (pRect.Height() * pos) / nRange;
		mProgressRect.top = mProgressRect.bottom - nPrg;
	}

	CRect drawRect(drawText.x, drawText.y, mProgressRect.right,
		mProgressRect.bottom);

	CRect rectGrey(pRect);
	if (bVertical)
	{
		rectGrey.bottom = mProgressRect.top;
	}
	else
	{
		rectGrey.left = mProgressRect.right;
	}

	dc.SelectObject(&nBkBrush);
	dc.SelectStockObject(NULL_PEN);
	dc.Rectangle(rectGrey);

	if (!bVertical)
	{
		dc.SelectStockObject(DEFAULT_GUI_FONT);
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(RGB(128, 128, 128));
		dc.TextOut(drawText.x, drawText.y , m_strCaption);
		dc.SetTextColor(RGB(255, 255, 255));
		dc.TextOut(drawText.x + 1, drawText.y + 1 , m_strCaption);

		DrawHorzBar(&dc, mProgressRect);

		dc.SetTextColor(RGB(60, 60, 80));
		dc.DrawText(m_strCaption, drawRect, DT_LEFT );

		dc.SetTextColor(RGB(255, 255, 255));
		drawRect.left += 1;
		drawRect.top += 1;
		dc.DrawText(m_strCaption, drawRect, DT_LEFT );
	}
	else 
	{
		DrawHorzBar(&dc, mProgressRect);
	}
	// Do not call CProgressCtrl::OnPaint() for painting messages
}

void CSwingProgressCtrl::DrawHorzBar(CDC *pDC, CRect pRect)
{
//	pRect.DeflateRect(0, 0, 1, 1);
	pDC->SelectStockObject(NULL_PEN);
	pDC->SelectObject(&nProgressBrush);
	pDC->Rectangle(pRect);

}

void CSwingProgressCtrl::SetCaption(CString pstr)
{
	m_strCaption = pstr;
}

CString CSwingProgressCtrl::GetCaption()
{
	return m_strCaption;
}

void CSwingProgressCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CProgressCtrl::OnSetFocus(pOldWnd);
	// TODO: Add your message handler code here
	
}

BOOL CSwingProgressCtrl::OnEraseBkgnd(CDC* pDC) 
{
	CDC *gDC = pDC;

	pDC = GetWindowDC();
	CRect rectItem, mRect;
	GetWindowRect(mRect);
	GetParent()->ClientToScreen(rectItem);
	GetClientRect(rectItem);

	rectItem.right = rectItem.left + mRect.Width();
	rectItem.bottom = rectItem.top + mRect.Height();

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
	pDC = gDC;
	return TRUE;
	// TODO: Add your message handler code here
	// TODO: Add your message handler code here and/or call default
	
//	return CProgressCtrl::OnEraseBkgnd(pDC);
}
