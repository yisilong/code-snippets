// SwingSliderCtrl.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "SwingSliderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CSwingSliderCtrl

CSwingSliderCtrl::CSwingSliderCtrl()
{
	bFocus = FALSE;
	pBackBrush.CreateSolidBrush(RGB(192, 192, 192));
	pSelBackBrush.CreateSolidBrush(RGB(160, 160, 192));

	nSelThumbPen.CreatePen(PS_SOLID, 1, RGB(166, 202, 240));
	nUnselThumbPen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	nDarkPen.CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
	nGreyBrush.CreateSolidBrush(RGB(204, 204, 204));
	nLastRect.top = nLastRect.left = nLastRect.right = 
		nLastRect.bottom = 0;
}

CSwingSliderCtrl::~CSwingSliderCtrl()
{
}


BEGIN_MESSAGE_MAP(CSwingSliderCtrl, CSliderCtrl)
	//{{AFX_MSG_MAP(CSwingSliderCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSwingSliderCtrl message handlers
void CSwingSliderCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW lpcd = (LPNMCUSTOMDRAW)pNMHDR;

	if (lpcd->dwDrawStage == CDDS_PREPAINT)
	{
		// Request prepaint notifications for each item.
		*pResult = CDRF_NOTIFYITEMDRAW;         
		return;
	}

	if (lpcd->dwDrawStage == CDDS_ITEMPREPAINT)
	{	
		CDC *pDC = CDC::FromHandle(lpcd->hdc);
		CRect rect(lpcd->rc);
		int nSavedDC = pDC->SaveDC();

		if (lpcd->dwItemSpec == TBCD_TICS)
		{
			*pResult = CDRF_DODEFAULT;
			return;
		}	// if drawing tics

		else if (lpcd->dwItemSpec ==  TBCD_THUMB)
		{
			*pResult = CDRF_SKIPDEFAULT;

			if (rect.Height() > rect.Width())
			{
				DrawVerticalThumb(pDC, rect, bFocus);
			}
	
			else
			{
				DrawHorizontalThumb(pDC, rect, bFocus);
			}

			return;
		}
		else if (lpcd->dwItemSpec == TBCD_CHANNEL)
		{
			*pResult = CDRF_SKIPDEFAULT;
			DrawChannel(pDC, rect);
			return;
		}
	}
}

void CSwingSliderCtrl::DrawVerticalThumb(CDC *pDC, CRect pRect, BOOL hasFocus)
{
	InvalidateRect(nLastRect);
	nLastRect = pRect;
	nLastRect.right = nLastRect.left + 20;

	points[0].x = pRect.left + 0;		points[0].y = pRect.top + 0;
	points[1].x = pRect.left + 0;		points[1].y = pRect.top + 7;
	points[2].x = pRect.left + 7;		points[2].y = pRect.top + 14;
	points[3].x = pRect.left + 14;		points[3].y = pRect.top + 7;
	points[4].x = pRect.left + 14;		points[4].y = pRect.top + 0;
	int x[] = {2, 6, 10, 4, 8, 2, 6, 10};
	int y[] = {2, 2, 2, 4, 4, 6, 6, 6};

	pDC->SelectObject(hasFocus? &pSelBackBrush : &pBackBrush);
	pDC->SelectStockObject(BLACK_PEN);
	pDC->Polygon(points, 5);

	pDC->SelectObject(hasFocus ? nSelThumbPen : nUnselThumbPen);
	pDC->MoveTo(pRect.left + 1, pRect.top + 1);
	pDC->LineTo(pRect.left + 1, pRect.top + 7);
	pDC->MoveTo(pRect.left + 1, pRect.top + 1);
	pDC->LineTo(pRect.left + 13,pRect.top + 1);


	for (int i = 0; i < 8; ++i)
	{
		pDC->SetPixelV(pRect.left + x[i], pRect.top + y[i], hasFocus ? RGB(166, 202, 240) : RGB(255, 255, 255));
		pDC->SetPixelV(pRect.left + x[i] + 1,pRect.top +  y[i] + 1, hasFocus ? RGB(60, 60, 80) : RGB(0, 0, 0));

	}

}

void CSwingSliderCtrl::DrawHorizontalThumb(CDC *pDC, CRect pRect, BOOL hasFocus)
{
	InvalidateRect(nLastRect);
	nLastRect = pRect;
	nLastRect.bottom = nLastRect.top + 20;

	points[0].x = pRect.left + 0;		points[0].y = pRect.top + 0;
	points[1].x = pRect.left + 0;		points[1].y = pRect.top + 14;
	points[2].x = pRect.left + 7;		points[2].y = pRect.top + 14;
	points[3].x = pRect.left + 14;		points[3].y = pRect.top + 7;
	points[4].x = pRect.left + 7;		points[4].y = pRect.top + 0;

	pDC->SelectObject(hasFocus? &pSelBackBrush : &pBackBrush);
	pDC->SelectStockObject(BLACK_PEN);
	pDC->Polygon(points, 5);

	pDC->SelectObject(hasFocus ? nSelThumbPen : nUnselThumbPen);
	pDC->MoveTo(pRect.left + 1, pRect.top + 1);
	pDC->LineTo(pRect.left + 1, pRect.top + 13);
	pDC->MoveTo(pRect.left + 1, pRect.top + 1);
	pDC->LineTo(pRect.left + 7,pRect.top +  1);

	int x[] = {2, 2, 2, 4, 4, 6, 6, 6};
	int y[] = {2, 6, 10, 4, 8, 2, 6, 10};

	for (int i = 0; i < 8; ++i)
	{
		pDC->SetPixelV(pRect.left + x[i], pRect.top + y[i], hasFocus ? RGB(166, 202, 240) : RGB(255, 255, 255));
		pDC->SetPixelV(pRect.left + x[i] + 1, pRect.top + y[i] + 1, hasFocus ? RGB(60, 60, 80) : RGB(0, 0, 0));

	}
}

void CSwingSliderCtrl::DrawActiveDC()
{

}

void CSwingSliderCtrl::DrawInactiveDC()
{
}

void CSwingSliderCtrl::DrawChannel(CDC *pDC, CRect pRect)
{
	pDC->SelectStockObject(NULL_PEN);
	pDC->SelectObject(&nGreyBrush);
	pDC->Rectangle(pRect);

	pDC->SelectObject(&nDarkPen);
	pDC->MoveTo(pRect.left, pRect.top);
	pDC->LineTo(pRect.right, pRect.top);
	pDC->MoveTo(pRect.left, pRect.top);
	pDC->LineTo(pRect.left, pRect.bottom);
	pDC->MoveTo(pRect.left, pRect.bottom - 1);
	pDC->LineTo(pRect.right, pRect.bottom - 1);
	pDC->MoveTo(pRect.right - 1, pRect.bottom - 1);
	pDC->LineTo(pRect.right - 1, pRect.top);

	pDC->SelectStockObject(WHITE_PEN);
	pDC->MoveTo(pRect.left, pRect.bottom);
	pDC->LineTo(pRect.right, pRect.bottom);
	pDC->MoveTo(pRect.right, pRect.bottom);
	pDC->LineTo(pRect.right, pRect.top);
}

void CSwingSliderCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CSliderCtrl::OnSetFocus(pOldWnd);
	bFocus = TRUE;
	Invalidate();
	// TODO: Add your message handler code here
	
}

void CSwingSliderCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	CSliderCtrl::OnKillFocus(pNewWnd);
	bFocus = FALSE;
	Invalidate();
	
	// TODO: Add your message handler code here
	
}
