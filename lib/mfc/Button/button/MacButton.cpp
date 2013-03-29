// MacButton.cpp : implementation file
//
//	CMacButton class, version 1.0
//
//	Copyright (c) 1999 Paul Meidinger (pmmeidinger@yahoo.com)
//
//
// Feel free to modifiy and/or distribute this file, but
// do not remove this header.
//
// I would appreciate a notification of any bugs discovered or 
// improvements that could be made.
//
// This file is provided "as is" with no expressed or implied warranty.
//

#include "stdafx.h"
#include "MacButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMacButton

//-------------------------------------------------------------------
//
CMacButton::CMacButton()
//
// Return Value:	None.
//
// Parameters	:	None.
//
// Remarks		:	Standard constructor.
//
{
	m_bBold = FALSE;

	GetColors();
}	// CMacButton

//-------------------------------------------------------------------
//
CMacButton::~CMacButton()
//
// Return Value:	None.
//
// Parameters	:	None.
//
// Remarks		:	Destructor.
//
{
}	// ~CMacButton


BEGIN_MESSAGE_MAP(CMacButton, CButton)
	//{{AFX_MSG_MAP(CMacButton)
	ON_WM_SYSCOLORCHANGE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMacButton message handlers

//-------------------------------------------------------------------
//
void CMacButton::PreSubclassWindow() 
//
// Return Value:	None.
//
// Parameters	:	None.
//
// Remarks		:	This member function is called by the framework to 
//						allow other necessary subclassing to occur before the 
//						window is subclassed. Adds the BS_OWNERDRAW style.
//
{
	CButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
}	// PreSubclassWindow

//-------------------------------------------------------------------
//
void CMacButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
//
// Return Value:	None.
//
// Parameters	:	lpDIS - A long pointer to a DRAWITEMSTRUCT structure.
//
// Remarks		:	Called by the framework when a visual aspect of an 
//						owner-drawn button has changed.
//
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CRect rectItem(lpDIS->rcItem);

	UINT nState = lpDIS->itemState;
	UINT nStyle = GetStyle();

	int nSaveDC = pDC->SaveDC();

	DrawButton(pDC, rectItem, nStyle, nState);

	pDC->SetBkMode(TRANSPARENT);

	CFont *pOldFont = NULL, *pFont = GetFont();
	CFont fontNew;
	if (pFont && m_bBold)
	{
		LOGFONT lf;
		pFont->GetLogFont(&lf);
		lf.lfWeight = FW_BOLD;
		fontNew.CreateFontIndirect(&lf);
		pOldFont = (CFont *)pDC->SelectObject(&fontNew);
	}

	CString sText;
	GetWindowText(sText);

	CSize sizeText = pDC->GetTextExtent(sText);
	rectItem.left = (rectItem.Width() - sizeText.cx) >> 1;
	rectItem.top = (rectItem.Height() - sizeText.cy) >> 1;
	rectItem.right = rectItem.left + sizeText.cx;
	rectItem.bottom = rectItem.top + sizeText.cy;

	if (nState & ODS_DISABLED)
		pDC->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
	if (nState & ODS_SELECTED)
		pDC->SetTextColor(m_crHilight);

	pDC->DrawText(sText, rectItem, DT_CENTER);

	if (pFont && m_bBold)
	{
		pDC->SelectObject(pOldFont);
		fontNew.DeleteObject();
	}

	rectItem = lpDIS->rcItem;
	rectItem.DeflateRect(4, 4, 4, 4);

	if (nState & ODS_FOCUS)
		pDC->DrawFocusRect(rectItem);

	pDC->RestoreDC(nSaveDC);
}	// DrawItem

//-------------------------------------------------------------------
//
void CMacButton::DrawButton(CDC *pDC, CRect rect, UINT nStyle, UINT nState)
//
// Return Value:	None.
//
// Parameters	:	pDC - A pointer to the DC to draw on.
//						rect - The button rectangle.
//						nStyle - The button style.
//						nState - The button state.
//
// Remarks		:	Draws the button.
//
{
	if (nStyle & BS_FLAT)
	{
		CBrush br(nState & ODS_SELECTED ? ::GetSysColor(COLOR_WINDOWFRAME): ::GetSysColor(COLOR_WINDOW));
		CBrush *pOldBrush = (CBrush *)pDC->SelectObject(&br);
		CPen pen(PS_SOLID, 1, nState & ODS_SELECTED ? ::GetSysColor(COLOR_WINDOW) : ::GetSysColor(COLOR_WINDOWFRAME));
		CPen *pOldPen = (CPen *)pDC->SelectObject(&pen);
		pDC->RoundRect(rect, CPoint(10, 10));
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
		return;
	}	// if flat
		
	if (nState & ODS_SELECTED)
	{
		CBrush brLiteShadow(m_crLiteShadow);
		CBrush *pOldBrush	= (CBrush *)pDC->SelectObject(&brLiteShadow);
		CPen penDkShadow(PS_SOLID, 1, m_crDkShadow);
		CPen *pOldPen = (CPen *)pDC->SelectObject(&penDkShadow);

		pDC->RoundRect(rect, CPoint(6, 6));

		CPen penDarkDkShadow(PS_SOLID, 1, m_crDarkDkShadow);
		pDC->SelectObject(&penDarkDkShadow);
		pDC->MoveTo(rect.left + 1, rect.bottom - 4);
		pDC->LineTo(rect.left + 1, rect.top + 2);
		pDC->LineTo(rect.left + 2, rect.top + 2);
		pDC->LineTo(rect.left + 2, rect.top + 1);
		pDC->LineTo(rect.right - 3, rect.top + 1);

		CPen penShadow(PS_SOLID, 1, m_crShadow);
		pDC->SelectObject(&penShadow);
		pDC->MoveTo(rect.left + 2, rect.bottom - 4);
		pDC->LineTo(rect.left + 2, rect.top + 3);
		pDC->LineTo(rect.left + 3, rect.top + 3);
		pDC->LineTo(rect.left + 3, rect.top + 2);
		pDC->LineTo(rect.right - 3, rect.top + 2);

		CPen penHilight(PS_SOLID, 1, m_crLiteFace);
		pDC->SelectObject(&penHilight);
		pDC->MoveTo(rect.left + 3, rect.bottom - 2);
		pDC->LineTo(rect.right - 4, rect.bottom - 2);
		pDC->LineTo(rect.right - 2, rect.bottom - 4);
		pDC->LineTo(rect.right - 2, rect.top + 2);

		CPen penFace(PS_SOLID, 1, m_crFace);
		pDC->SelectObject(&penFace);
		pDC->MoveTo(rect.left + 3, rect.bottom - 3);
		pDC->LineTo(rect.right - 4, rect.bottom - 3);
		pDC->LineTo(rect.right - 4, rect.bottom - 4);
		pDC->LineTo(rect.right - 3, rect.bottom - 4);
		pDC->LineTo(rect.right - 3, rect.top + 2);

		pDC->SetPixel(rect.right - 2, rect.top + 2, m_crFace);
		pDC->SetPixel(rect.left + 2, rect.bottom - 2, m_crFace);
		pDC->SetPixel(rect.right - 3, rect.top + 1, m_crShadow);
		pDC->SetPixel(rect.left + 1, rect.bottom - 3, m_crShadow);

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}	// if selected
	else
	{
		CBrush brFace(m_crFace);
		CBrush *pOldBrush	= (CBrush *)pDC->SelectObject(&brFace);
		CPen penDkShadow(PS_SOLID, 1, m_crDkShadow);
		CPen *pOldPen = (CPen *)pDC->SelectObject(&penDkShadow);

		pDC->RoundRect(rect, CPoint(6, 6));

		CPen penHilight(PS_SOLID, 1, m_crHilight);
		pDC->SelectObject(&penHilight);
		pDC->MoveTo(rect.left + 2, rect.bottom - 4);
		pDC->LineTo(rect.left + 2, rect.top + 2);
		pDC->LineTo(rect.right - 3, rect.top + 2);
		pDC->SetPixel(rect.left + 3, rect.top + 3, m_crHilight);

		CPen penLiteShadow(PS_SOLID, 1, m_crLiteShadow);
		pDC->SelectObject(&penLiteShadow);
		pDC->MoveTo(rect.left + 3, rect.bottom - 3);
		pDC->LineTo(rect.right - 4, rect.bottom - 3);
		pDC->LineTo(rect.right - 4, rect.bottom - 4);
		pDC->LineTo(rect.right - 3, rect.bottom - 4);
		pDC->LineTo(rect.right - 3, rect.top + 2);

		CPen penShadow(PS_SOLID, 1, m_crShadow);
		pDC->SelectObject(&penShadow);
		pDC->MoveTo(rect.left + 3, rect.bottom - 2);
		pDC->LineTo(rect.right - 3, rect.bottom - 2);
		pDC->LineTo(rect.right - 3, rect.bottom - 3);
		pDC->LineTo(rect.right - 2, rect.bottom - 3);
		pDC->LineTo(rect.right - 2, rect.top + 2);

		pDC->SetPixel(rect.left, rect.top + 2, m_crDarkDkShadow);
		pDC->SetPixel(rect.left + 2, rect.top, m_crDarkDkShadow);
		pDC->SetPixel(rect.right - 3, rect.top, m_crDarkDkShadow);
		pDC->SetPixel(rect.right - 1, rect.top + 2, m_crDarkDkShadow);
		pDC->SetPixel(rect.right - 1, rect.bottom - 3, m_crDarkDkShadow);
		pDC->SetPixel(rect.right - 3, rect.bottom - 1, m_crDarkDkShadow);
		pDC->SetPixel(rect.left + 2, rect.bottom - 1, m_crDarkDkShadow);
		pDC->SetPixel(rect.left, rect.bottom - 3, m_crDarkDkShadow);

		pDC->SetPixel(rect.left + 1, rect.top + 2, m_crLiteShadow);
		pDC->SetPixel(rect.left + 2, rect.top + 1, m_crLiteShadow);
		pDC->SetPixel(rect.right - 3, rect.top + 1, m_crLiteShadow);
		pDC->SetPixel(rect.right - 2, rect.top + 2, m_crLiteShadow);
		pDC->SetPixel(rect.left + 2, rect.bottom - 2, m_crLiteShadow);
		pDC->SetPixel(rect.left + 1, rect.bottom - 3, m_crLiteShadow);

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}	// else if not selected

	pDC->SetPixel(rect.left, rect.top + 2, m_crDarkDkShadow);
	pDC->SetPixel(rect.left + 2, rect.top, m_crDarkDkShadow);
	pDC->SetPixel(rect.right - 3, rect.top, m_crDarkDkShadow);
	pDC->SetPixel(rect.right - 1, rect.top + 2, m_crDarkDkShadow);
	pDC->SetPixel(rect.right - 1, rect.bottom - 3, m_crDarkDkShadow);
	pDC->SetPixel(rect.right - 3, rect.bottom - 1, m_crDarkDkShadow);
	pDC->SetPixel(rect.left + 2, rect.bottom - 1, m_crDarkDkShadow);
	pDC->SetPixel(rect.left, rect.bottom - 3, m_crDarkDkShadow);
}	// DrawButton

//-------------------------------------------------------------------
//
void CMacButton::OnLButtonDblClk(UINT /*nFlags*/, CPoint point) 
//
// Return Value:	None.
//
// Parameters	:	nFlags - Indicates whether various virtual keys are down.
//						point - Specifies the x- and y-coordinate of the cursor. 
//							These coordinates are always relative to the upper-left
//							 corner of the window.
//
// Remarks		:	The framework calls this member function when the user 
//						double-clicks the left mouse button. The WM_LBUTTONDOWN
//						message is sent to simulate a single click.
//
{
	SendMessage(WM_LBUTTONDOWN, 0, MAKELPARAM(point.x, point.y));	
}	// OnLButtonDblClk

//-------------------------------------------------------------------
//
void CMacButton::OnSysColorChange() 
//
// Return Value:	None.
//
// Parameters	:	None.
//
// Remarks		:	The framework calls this member function for all 
//						top-level windows when a change is made in the 
//						system color setting
//
{
	CButton::OnSysColorChange();
	
	GetColors();	
}	// OnSysColorChange

//-------------------------------------------------------------------
//
void CMacButton::GetColors()
//
// Return Value:	None.
//
// Parameters	:	None.
//
// Remarks		:	Sets the COLORREF member variables.
//
{
	m_crHilight = ::GetSysColor(COLOR_3DHILIGHT);
	m_crFace = ::GetSysColor(COLOR_3DFACE);
	m_crLiteFace = ::GetSysColor(COLOR_3DLIGHT);
	m_crShadow = ::GetSysColor(COLOR_3DSHADOW);
	m_crDkShadow = ::GetSysColor(COLOR_3DDKSHADOW);

	// Create some intermediate colors. This is done by averaging two colors.
	BYTE byRedFace = GetRValue(m_crFace);
	BYTE byGreenFace = GetGValue(m_crFace);
	BYTE byBlueFace = GetBValue(m_crFace);
	BYTE byRedShadow = GetRValue(m_crShadow);
	BYTE byGreenShadow = GetGValue(m_crShadow);
	BYTE byBlueShadow = GetBValue(m_crShadow);
	BYTE byRedDkShadow = GetRValue(m_crDkShadow);
	BYTE byGreenDkShadow = GetGValue(m_crDkShadow);
	BYTE byBlueDkShadow = GetBValue(m_crDkShadow);

	m_crLiteShadow = RGB(byRedFace + ((byRedShadow - byRedFace) >> 1),
										 byGreenFace + ((byGreenShadow - byGreenFace) >> 1),
										 byBlueFace + ((byBlueShadow - byBlueFace) >> 1));

	m_crDarkDkShadow = RGB(byRedShadow + ((byRedDkShadow - byRedShadow) >> 1),
											byGreenShadow + ((byGreenDkShadow - byGreenShadow) >> 1),
											byBlueShadow + ((byBlueDkShadow - byBlueShadow) >> 1));
}	// GetColors

//-------------------------------------------------------------------
//
void CMacButton::SetBold(BOOL bBold)
//
// Return Value:	None.
//
// Parameters	:	bBold - Flag used to set m_bBold.
//
// Remarks		:	Sets the m_bBold member variable.
//
{
	m_bBold = bBold;
	RedrawWindow();
}	// SetBold

//-------------------------------------------------------------------
//
BOOL CMacButton::GetBold()
//
// Return Value:	m_bBold.
//
// Parameters	:	None.
//
// Remarks		:	Returns m_bBold.
//
{
	return m_bBold;
}	// GetBold
