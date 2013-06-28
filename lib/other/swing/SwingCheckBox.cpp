// SwingCheckBox.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "SwingCheckBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSwingCheckBox

CSwingCheckBox::CSwingCheckBox()
{
	m_Checked = FALSE;

	nActiveBrush.CreateSolidBrush(RGB(110, 110, 110));
	nInactiveBrush.CreateSolidBrush(RGB(204, 204, 204));
	nDarkBorder.CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
	nWhiteBorder.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	nSelectedBorder.CreatePen(PS_SOLID, 1, RGB(153, 153, 204));
	nCheck.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

}

CSwingCheckBox::~CSwingCheckBox()
{
}


BEGIN_MESSAGE_MAP(CSwingCheckBox, CButton)
	//{{AFX_MSG_MAP(CSwingCheckBox)
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSwingCheckBox message handlers

void CSwingCheckBox::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
}

void CSwingCheckBox::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CRect rectItem(lpDIS->rcItem);

	CRect rectCheck(rectItem);
	rectCheck.right = rectCheck.left + 12;
	rectCheck.bottom = rectCheck.top + 12;

	CPoint TopLeft(rectCheck.left, rectCheck.top);
	CPoint BottomRight(rectCheck.right, rectCheck.bottom);
	CPoint TopRight(rectCheck.right, rectCheck.top);
	CPoint BottomLeft(rectCheck.left, rectCheck.bottom);
	// TODO: Add your code to draw the specified item

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

	pDC->SelectStockObject(DEFAULT_GUI_FONT);
	pDC->TextOut(16, 0, m_strCaption);

	if (m_Checked)
	{
		pDC->SelectObject(&nCheck);
		pDC->MoveTo(2, 3);
		pDC->LineTo(3, 8);
		pDC->MoveTo(3, 9);
		pDC->LineTo(9, 3);
	}

	if (lpDIS->itemState & ODS_FOCUS)
	{
		pDC->SelectObject(&nSelectedBorder);
		pDC->SelectStockObject(NULL_BRUSH);
		int xlen = pDC->GetTextExtent(m_strCaption).cx;

		pDC->Rectangle(15, 0, 13 + xlen + 3, 12);
	}

}

void CSwingCheckBox::SetCheckboxText(CString pStr)
{
	m_strCaption = pStr;

}

void CSwingCheckBox::OnClicked() 
{
	if (!m_Checked)
		m_Checked = TRUE;
	else
		m_Checked = FALSE;

	RedrawWindow();
	// TODO: Add your control notification handler code here
	
}

CString CSwingCheckBox::GetCheckboxText()
{
	return m_strCaption;
}
