// SwingButton.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "SwingButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSwingButton

CSwingButton::CSwingButton()
{
	nActiveBrush.CreateSolidBrush(RGB(110, 110, 110));
	nInactiveBrush.CreateSolidBrush(RGB(204, 204, 204));
	nDarkBorder.CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
	nWhiteBorder.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	nSelectedBorder.CreatePen(PS_SOLID, 1, RGB(153, 153, 204));
}

CSwingButton::~CSwingButton()
{
}


BEGIN_MESSAGE_MAP(CSwingButton, CButton)
	//{{AFX_MSG_MAP(CSwingButton)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSwingButton message handlers

void CSwingButton::SetButtonText(CString pString)
{
	m_strCaption = pString;

}

void CSwingButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	DrawButton(lpDIS);
	// TODO: Add your code to draw the specified item
}

void CSwingButton::DrawButton(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CRect rectItem(lpDIS->rcItem);

	CPoint TopLeft(rectItem.left, rectItem.top);
	CPoint BottomRight(rectItem.right - 1, rectItem.bottom - 1);
	CPoint TopRight(rectItem.right - 1, rectItem.top);
	CPoint BottomLeft(rectItem.left, rectItem.bottom - 1);

	if (lpDIS->itemState & ODS_SELECTED)
	{
		pDC->SelectObject(&nActiveBrush);
		pDC->SelectStockObject(NULL_PEN);
		pDC->Rectangle(rectItem);
		goto WRITE_TEXT;
	}

	pDC->SelectObject(&nInactiveBrush);
	pDC->SelectStockObject(NULL_PEN);
	pDC->Rectangle(rectItem);

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

WRITE_TEXT:
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectStockObject(DEFAULT_GUI_FONT);
	CSize pExtent = pDC->GetTextExtent(m_strCaption);

	CPoint centerPoint = rectItem.CenterPoint();
	CPoint drawText;
	drawText.x = centerPoint.x - (pExtent.cx / 2);
	drawText.y = centerPoint.y - (pExtent.cy / 2);

	pDC->TextOut(drawText.x, drawText.y, m_strCaption);

	if (lpDIS->itemState & ODS_FOCUS)
	{
		pDC->SelectObject(&nSelectedBorder);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(drawText.x - 1, drawText.y - 1, 
			centerPoint.x + (pExtent.cx / 2) + 3,
			centerPoint.y + (pExtent.cy / 2) + 3); 
	}
}

void CSwingButton::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class	
	CButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
}

CString CSwingButton::GetButtonText()
{
	return m_strCaption;
}
