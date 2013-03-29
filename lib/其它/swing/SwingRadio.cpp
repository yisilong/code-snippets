// SwingRadio.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "SwingRadio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSwingRadio
//	SwingControl Classes class, version 1.0
//
//	Copyright (c) 2000 Abhijit N Bhole (abhijit_bhole@hotmail.com.com)
//
// Feel free to modifiy and/or distribute this file, but
// do not remove this header.
//
// I would appreciate a notification of any bugs discovered or 
// improvements that could be made.
//
// This file is provided "as is" with no expressed or implied warranty.
//
//	History:
//		PMM	5/14/2000		Initial implementation.		

CSwingRadio::CSwingRadio()
{
	m_Checked = FALSE;

	nActiveBrush.CreateSolidBrush(RGB(110, 110, 110));
	nInactiveBrush.CreateSolidBrush(RGB(204, 204, 204));
	nDarkBorder.CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
	nWhiteBorder.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	nSelectedBorder.CreatePen(PS_SOLID, 1, RGB(153, 153, 204));
	nCheck.CreateSolidBrush(RGB(153, 153, 204));
}

CSwingRadio::~CSwingRadio()
{
}


BEGIN_MESSAGE_MAP(CSwingRadio, CButton)
	//{{AFX_MSG_MAP(CSwingRadio)
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSwingRadio message handlers

void CSwingRadio::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CRect rectItem(lpDIS->rcItem);

	CRect rectCheck(rectItem);
	rectCheck.right = rectCheck.left + 12;
	rectCheck.bottom = rectCheck.top + 12;

	// TODO: Add your code to draw the specified item

	pDC->SelectObject(&nInactiveBrush);
	pDC->SelectStockObject(NULL_PEN);
	pDC->Rectangle(rectItem);

	pDC->SelectStockObject(NULL_BRUSH);
	pDC->SelectObject(&nWhiteBorder);
	pDC->Ellipse(rectCheck.left + 1, rectCheck.top + 1, rectCheck.right, rectCheck.bottom);

	pDC->SelectObject(&nDarkBorder);
	pDC->Ellipse(rectCheck.left, rectCheck.top, rectCheck.right - 1, rectCheck.bottom - 1);

	pDC->SelectStockObject(DEFAULT_GUI_FONT);
	pDC->TextOut(16, 0, m_strCaption);

	if (m_Checked)
	{
		pDC->SelectStockObject(NULL_PEN);
		pDC->SelectObject(&nCheck);
		pDC->Ellipse(3, 3, 9, 9);
	}

	if (lpDIS->itemState & ODS_FOCUS)
	{
		pDC->SelectObject(&nSelectedBorder);
		pDC->SelectStockObject(NULL_BRUSH);
		int xlen = pDC->GetTextExtent(m_strCaption).cx;

		pDC->Rectangle(15, 0, 13 + xlen + 5, 12);
	}
	// TODO: Add your code to draw the specified item
	
}

void CSwingRadio::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
}

void CSwingRadio::SetRadioText(CString pStr)
{
	m_strCaption = pStr;
}

CString CSwingRadio::GetRadioText()
{
	return m_strCaption;
}

void CSwingRadio::OnClicked() 
{
	SetCheck();
	// TODO: Add your control notification handler code here
	
}

void CSwingRadio::UncheckOtherRadios()
{
	// Go backwards (GetNextDlgItem will wrap around to the end of the group).
	CWnd *pParent = GetParent();
	ASSERT(pParent);

	CWnd *pWnd = pParent->GetNextDlgGroupItem((CWnd *)this);
	while (pWnd && (pWnd != this))
	{
		if ((((CButton*)pWnd)->GetButtonStyle() & BS_AUTORADIOBUTTON))
		{
			((CSwingRadio *)pWnd)->m_Checked = FALSE;
			pWnd->Invalidate();
		}
		pWnd = pParent->GetNextDlgGroupItem(pWnd);
	}
}	// UncheckOtherRadios

BOOL CSwingRadio::GetCheck()
{
	return m_Checked;

}

void CSwingRadio::SetCheck()
{
	m_Checked = TRUE;
	UncheckOtherRadios();
	Invalidate();
}
