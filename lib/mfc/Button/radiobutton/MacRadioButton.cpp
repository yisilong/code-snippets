// MacRadioButton.cpp : implementation file
//
// CMacRadioButton class, version 1.0
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
#include "MacRadioButton.h"
#include "MemDC.h"
#include "test.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Funtion prototypes.
void LightenColorref(COLORREF &crColor, BYTE byIncreaseVal);
void DarkenColorref(COLORREF &crColor, BYTE byReduceVal);

//-------------------------------------------------------------------
//
void LightenColorref(COLORREF &crColor, BYTE byIncreaseVal)
//
// Return Value:	None.
//
// Parameters	:	crColor - References a COLORREF structure.
//						byReduceVal - The amount to reduce the RGB values by.
//
// Remarks		:	Lightens a color by increasing the RGB values by the given number.
//
{
	BYTE byRed = GetRValue(crColor);
	BYTE byGreen = GetGValue(crColor);
	BYTE byBlue = GetBValue(crColor);

	if ((byRed + byIncreaseVal) <= 255)
		byRed = BYTE(byRed + byIncreaseVal);
	if ((byGreen + byIncreaseVal)	<= 255)
		byGreen = BYTE(byGreen + byIncreaseVal);
	if ((byBlue + byIncreaseVal) <= 255)
		byBlue = BYTE(byBlue + byIncreaseVal);

	crColor = RGB(byRed, byGreen, byBlue);
}	// LightenColorref

//-------------------------------------------------------------------
//
void DarkenColorref(COLORREF &crColor, BYTE byReduceVal)
//
// Return Value:	None.
//
// Parameters	:	crColor - References a COLORREF structure.
//						byReduceVal - The amount to reduce the RGB values by.
//
// Remarks		:	Darkens a color by reducing the RGB values by the given number.
//
{
	BYTE byRed = GetRValue(crColor);
	BYTE byGreen = GetGValue(crColor);
	BYTE byBlue = GetBValue(crColor);

	if (byRed >= byReduceVal)
		byRed = BYTE(byRed - byReduceVal);
	if (byGreen >= byReduceVal)
		byGreen = BYTE(byGreen - byReduceVal);
	if (byBlue >= byReduceVal)
		byBlue = BYTE(byBlue - byReduceVal);

	crColor = RGB(byRed, byGreen, byBlue);
}	// DarkenColorref


/////////////////////////////////////////////////////////////////////////////
// CMacRadioButton

IMPLEMENT_DYNAMIC(CMacRadioButton, CButton)

//-------------------------------------------------------------------
//
CMacRadioButton::CMacRadioButton()
//
// Return Value:	None.
//
// Parameters	:	None.
//
// Remarks		:	Standard constructor.
//
{
	m_strChecked="";
	m_strUnchecked="";
	m_bChecked = FALSE;
	m_bMouseDown = FALSE;
	m_bOver=FALSE;
	m_bSetBackGround=FALSE;
	InitColors();
}	// CMacRadioButton

//-------------------------------------------------------------------
//
CMacRadioButton::~CMacRadioButton()
//
// Return Value:	None.
//
// Parameters	:	None.
//
// Remarks		:	Destructor. Does nothing.
//
{
}	// ~CMacRadioButton


BEGIN_MESSAGE_MAP(CMacRadioButton, CButton)
	//{{AFX_MSG_MAP(CMacRadioButton)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMacRadioButton message handlers

//-------------------------------------------------------------------
//
void CMacRadioButton::PreSubclassWindow() 
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
void CMacRadioButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
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
	BOOL bDisabled = BOOL(nState & ODS_DISABLED);
	BOOL bRadioBkImg=(m_strChecked!="" && m_strUnchecked!="")? TRUE : FALSE;
	
	int nSaveDC = pDC->SaveDC();
	CMemDC memdc(pDC,rectItem);	  
	memdc->SetBkMode(TRANSPARENT);
	//»­±³¾°
	if (m_bSetBackGround)
	{
		memdc->BitBlt(0, 0, rectItem.Width(), rectItem.Height(), &m_dcBk, 0, 0, SRCCOPY);
	}
	else
	{
		// 		CBrush *pBrh=(CBrush *)(memdc->SelectStockObject(NULL_BRUSH));
		// 		memdc->FillRect(rectItem,pBrh);
		memdc->FillSolidRect(rectItem,m_clrbackground);
	}
	// Determine the rect for the check mark.
	CRect rectCheck;

	int nHeight = 13;
	int nAdjust = (rectItem.Height() - nHeight) >> 1;
	if (nStyle & BS_LEFTTEXT)
		rectCheck.SetRect(rectItem.right - nHeight, rectItem.top + nAdjust, 
								rectItem.right, rectItem.top + nHeight + nAdjust);
	else
		rectCheck.SetRect(rectItem.left, rectItem.top + nAdjust, 
								rectItem.left + nHeight, rectItem.top + nHeight + nAdjust);

	if (bRadioBkImg)
	{		
		Graphics gs(memdc->m_hDC);
		if (m_bChecked)
		{
			Image img(m_strChecked.AllocSysString(),FALSE);
			gs.DrawImage(&img,0,0);
		}
		else
		{
			Image img(m_strUnchecked.AllocSysString(),FALSE);
			gs.DrawImage(&img,0,0);
		}
	}
	else	DrawRadio(memdc, rectCheck, nStyle, nState);

	// Determine the rect for the text.
	if (nStyle & BS_LEFTTEXT)
		rectItem.left = lpDIS->rcItem.left + 2;
	else
		rectItem.left += nHeight + 5;
	rectItem.top++;

	CString sText;
	GetWindowText(sText);

	CSize size = memdc->GetTextExtent(sText);
  	rectItem.right = rectItem.left + size.cx + (nStyle & BS_LEFTTEXT ? 2 : -5);
	rectItem.top = (rectItem.Height() - size.cy) >> 1;
	rectItem.bottom = rectItem.top + size.cy-1;

	CFont *pFont=GetFont();
	memdc->SelectObject(pFont);
	// Draw the text.
	if (bDisabled)
	{
		memdc->SetTextColor(GetSysColor(COLOR_3DFACE));
		memdc->DrawText(sText,rectItem,DT_LEFT | DT_SINGLELINE | DT_VCENTER );
//		memdc->DrawState(rectItem.TopLeft(), rectItem.Size(), sText, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
	}
	else
		memdc->DrawText(sText, rectItem, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	// Draw the focus rect.
// 	rectItem.InflateRect(1, 1, 0, 2);
// 
// 	if (nState & ODS_FOCUS)
// 		memdc->DrawFocusRect(rectItem); 

	memdc->RestoreDC(nSaveDC);
}	// DrawItem

//-------------------------------------------------------------------
//
void CMacRadioButton::DrawRadio(CDC *pDC, CRect rect, UINT nStyle, UINT nState)
//
// Return Value:	None.
//
// Parameters	:	pDC - A pointer to the DC to draw on.
//						rect - The radio's rectangle.
//						nStyle - The button's style.
//						nState - The button's state.
//
// Remarks		:	Draws the radio button.
//
{
	// Give the user a visual indication that the button is pressed by darkening some colors.
	BOOL bDarkened = FALSE;
	if (m_bMouseDown && !(nStyle & BS_FLAT))
	{
		DarkenColorref(m_clrRadioFace, 40);
		DarkenColorref(m_clrShadow, 40);
		DarkenColorref(m_clrHilight, 40);
		DarkenColorref(m_clrLightShadow, 40);
		DarkenColorref(m_clrLightFace, 40);
		bDarkened = TRUE;
	}

	// Draw the outer round rect.
	CPen penDkShadow(PS_SOLID, 1, m_clrDkShadow);
	CPen *pOldPen = (CPen *)pDC->SelectObject(&penDkShadow);
	if (nStyle & BS_FLAT)
	{
		CBrush br(m_bMouseDown ? m_clrRadioFace : m_clrHilight);
		CBrush *pOldBrush = (CBrush *)pDC->SelectObject(&br);
		pDC->RoundRect(rect, CPoint(10, 10));
		pDC->SelectObject(pOldBrush);
	}
	else
		pDC->RoundRect(rect, CPoint(10, 10));

	// Draw some shadowing
	if (!(nState & ODS_DISABLED) && !(nStyle & BS_FLAT))
	{
		int nAdjust = (m_bChecked || m_bMouseDown ? -11 : 0);
		int nSign = (m_bChecked || m_bMouseDown ? -1 : 1);

		CPen penDarkDkShadow(PS_SOLID, 1, m_clrDarkDkShadow);
		pDC->SelectObject(&penDarkDkShadow);

		pDC->MoveTo(rect.left + 0, rect.top + 4);
		pDC->LineTo(rect.left + 5, rect.top - 1);
		pDC->MoveTo(rect.left + 7, rect.top);
		pDC->LineTo(rect.left + 12, rect.top + 5);
		pDC->MoveTo(rect.left + 11, rect.top + 7);
		pDC->LineTo(rect.left + 6, rect.top + 12);
		pDC->MoveTo(rect.left + 4, rect.top + 11);
		pDC->LineTo(rect.left - 1, rect.top + 6);

		pDC->SetPixel(rect.left + (nSign * (nAdjust + 2)), rect.top + (nSign * (nAdjust + 2)), m_clrRadioFace);

		CPen penLiteFace(PS_SOLID, 1, m_clrLightFace);
		pDC->SelectObject(&penLiteFace);

		pDC->MoveTo(rect.left + (nSign * (nAdjust + 1)), rect.top + (nSign * (nAdjust + 6)));
		pDC->LineTo(rect.left + (nSign * (nAdjust + 1)), rect.top + (nSign * (nAdjust + 4)));
		pDC->LineTo(rect.left + (nSign * (nAdjust + 4)), rect.top + (nSign * (nAdjust + 1)));
		pDC->LineTo(rect.left + (nSign * (nAdjust + 7)), rect.top + (nSign * (nAdjust + 1)));
		pDC->SetPixel(rect.left + (nSign * (nAdjust + 3)), rect.top + (nSign * (nAdjust + 3)), m_clrLightFace);

		CPen penLite(PS_SOLID, 1, m_clrHilight);
		pDC->SelectObject(&penLite);
 
		pDC->MoveTo(rect.left + (nSign * (nAdjust + 2)), rect.top + (nSign * (nAdjust + 7)));
		pDC->LineTo(rect.left + (nSign * (nAdjust + 2)), rect.top + (nSign * (nAdjust + 4)));
		pDC->LineTo(rect.left + (nSign * (nAdjust + 6)), rect.top + (nSign * (nAdjust + 4)));
		pDC->MoveTo(rect.left + (nSign * (nAdjust + 4)), rect.top + (nSign * (nAdjust + 5)));
		pDC->LineTo(rect.left + (nSign * (nAdjust + 4)), rect.top + (nSign * (nAdjust + 2)));
 		pDC->LineTo(rect.left + (nSign * (nAdjust + 8)), rect.top + (nSign * (nAdjust + 2)));
		pDC->SetPixel(rect.left + (nSign * (nAdjust + 3)), rect.top + (nSign * (nAdjust + 5)), m_clrHilight);
		pDC->SetPixel(rect.left + (nSign * (nAdjust + 5)), rect.top + (nSign * (nAdjust + 3)), m_clrHilight);
		pDC->SetPixel(rect.left + (nSign * (nAdjust + 3)), rect.top + (nSign * (nAdjust + 6)), m_clrLightFace);
		pDC->SetPixel(rect.left + (nSign * (nAdjust + 6)), rect.top + (nSign * (nAdjust + 3)), m_clrLightFace);

		pDC->SelectObject(&penLiteFace);

		pDC->MoveTo(rect.left + (nSign * (nAdjust + 2)), rect.top + (nSign * (nAdjust + 8)));
  		pDC->LineTo(rect.left + (nSign * (nAdjust + 9)), rect.top + (nSign * (nAdjust + 1)));
		pDC->MoveTo(rect.left + (nSign * (nAdjust + 3)), rect.top + (nSign * (nAdjust + 8)));
  		pDC->LineTo(rect.left + (nSign * (nAdjust + 9)), rect.top + (nSign * (nAdjust + 2)));
		pDC->MoveTo(rect.left + (nSign * (nAdjust + 4)), rect.top + (nSign * (nAdjust + 8)));
    	pDC->LineTo(rect.left + (nSign * (nAdjust + 9)), rect.top + (nSign * (nAdjust + 3)));

		CPen penLiteShadow(PS_SOLID, 1, m_clrLightShadow);
		pDC->SelectObject(&penLiteShadow);

		pDC->MoveTo(rect.left + (nSign * (nAdjust + 6)), rect.top + (nSign * (nAdjust + 9))); 
		pDC->LineTo(rect.left + (nSign * (nAdjust + 10)), rect.top + (nSign * (nAdjust + 5))); 
		pDC->MoveTo(rect.left + (nSign * (nAdjust + 7)), rect.top + (nSign * (nAdjust + 9))); 
		pDC->LineTo(rect.left + (nSign * (nAdjust + 10)), rect.top + (nSign * (nAdjust + 6))); 

		CPen penShadow(PS_SOLID, 1, m_clrShadow);
		pDC->SelectObject(&penShadow);

		pDC->MoveTo(rect.left + (nSign * (nAdjust + 4)), rect.top + (nSign * (nAdjust + 10)));
		pDC->LineTo(rect.left + (nSign * (nAdjust + 7)), rect.top + (nSign * (nAdjust + 10))); 
		pDC->LineTo(rect.left + (nSign * (nAdjust + 10)), rect.top + (nSign * (nAdjust + 7))); 
		pDC->LineTo(rect.left + (nSign * (nAdjust + 10)), rect.top + (nSign * (nAdjust + 3))); 
	}	// if not disabled and not flat

	// Draw the check if necessary.
	if (m_bChecked)
	{
		CBrush br(nState & ODS_DISABLED ? m_clrShadow : m_clrDkShadow);
		CBrush *pOldBrush = pDC->SelectObject(&br);
		CPen pen(PS_SOLID, 1, (nState & ODS_DISABLED ? m_clrShadow : m_clrDkShadow));
		pDC->SelectObject(&pen);
		rect.DeflateRect(3, 3, 3, 3);
		pDC->RoundRect(rect, CPoint(2, 2));
		pDC->SelectObject(pOldBrush);
	}

	// Lighten the colors if they were darkened.
	if (bDarkened)
	{
		LightenColorref(m_clrRadioFace, 40);
		LightenColorref(m_clrShadow, 40);
		LightenColorref(m_clrHilight, 40);
		LightenColorref(m_clrLightShadow, 40);
		LightenColorref(m_clrLightFace, 40);
	}

	pDC->SelectObject(pOldPen);
}	// DrawRadio

//-------------------------------------------------------------------
//
BOOL CMacRadioButton::PreTranslateMessage(MSG* pMsg) 
//
// Return Value:	Nonzero if the message was translated and should not 
//						be dispatched; 0 if the message was not translated 
//						and should be dispatched.
//
// Parameters	:	pMsg - Points to a MSG structure that contains the 
//							message to process.
//
// Remarks		:	Used by class CWinApp to translate window messages 
//						before they are dispatched to theTranslateMessage 
//						andDispatchMessage Windows functions. Catches the 
//						WM_KEYDOWN message and handles the up, down, left
//						and right arrows.
//
{
//	if (GetCapture() == this)
//		return CButton::PreTranslateMessage(pMsg);

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_DOWN || pMsg->wParam == VK_UP ||
			 pMsg->wParam == VK_LEFT || pMsg->wParam == VK_RIGHT)
		{
			UncheckOtherRadios();

			// Get the previous/next button and check it.
			CWnd *pParent = GetParent();
			ASSERT(pParent);

			CWnd *pWnd = pParent->GetNextDlgGroupItem((CWnd *)this, (pMsg->wParam == VK_LEFT || pMsg->wParam == VK_UP ? TRUE : FALSE));
			if (pWnd && (pWnd->GetStyle() & BS_AUTORADIOBUTTON))
			{
				if (pWnd->IsKindOf(RUNTIME_CLASS(CMacRadioButton)))
					((CMacRadioButton*)pWnd)->SetCheck(1);
				else if (pWnd->IsKindOf(RUNTIME_CLASS(CButton)))
					((CButton*)pWnd)->SetCheck(1);
			}
			SetCheck(0);
			return FALSE;
		}
	}
	return CButton::PreTranslateMessage(pMsg);
}	// PreTranslateMessage

//-------------------------------------------------------------------
//
void CMacRadioButton::OnLButtonUp(UINT /*nFlags*/, CPoint point) 
//
// Return Value:	None.
//
// Parameters	:	nFlags - Indicates whether various virtual keys are down. 
//						point - Specifies the x- and y-coordinate of the cursor. 
//							These coordinates are always relative to the upper-left 
//							corner of the window.
//
// Remarks		:	The framework calls this member function when the user 
//						releases the left mouse button. Checks to see if the mouse
//						was released over the button. If so, check it and uncheck others.
//
{
	ReleaseCapture();

	CRect rect;
	GetClientRect(rect);
	if (rect.PtInRect(point))
	{
		if (!m_bChecked)
			m_bChecked = TRUE;

		// Uncheck all other radios in the group
		if ((GetStyle() & BS_AUTORADIOBUTTON) && m_bChecked)
		{
			UncheckOtherRadios();
		}
	}

	m_bMouseDown = FALSE;

	RedrawCheck();

//	CButton::OnLButtonUp(nFlags, point);
}	// OnLButtonUp

//-------------------------------------------------------------------
//
void CMacRadioButton::OnLButtonDown(UINT /*nFlags*/, CPoint /*point*/) 
//
// Return Value:	None.
//
// Parameters	:	nFlags - Indicates whether various virtual keys are down. 
//						point - Specifies the x- and y-coordinate of the cursor. 
//							These coordinates are always relative to the upper-left 
//							corner of the window.
//
// Remarks		:	The framework calls this member function when the user 
//						presses the left mouse button. Sets focus to this button,
//						captures the mouse, and redraws the check.
//
{
	m_bMouseDown = TRUE;
	SetFocus();
	SetCapture();
	RedrawCheck();
//	CButton::OnLButtonDown(nFlags, point);
}	// OnLButtonDown

//-------------------------------------------------------------------
//
void CMacRadioButton::OnMouseMove(UINT nFlags, CPoint point) 
//
// Return Value:	None.
//
// Parameters	:	nFlags - Indicates whether various virtual keys are down. 
//						point - Specifies the x- and y-coordinate of the cursor. 
//							These coordinates are always relative to the upper-left 
//							corner of the window.
//
// Remarks		:	The framework calls this member function when the 
//						mouse cursor moves. Checks to see if the mouse is over
//						the button, and redraws the check if it is, but wasn't previously.
//
{
	if (!m_bOver)
	{
		m_bOver=TRUE;		
		Invalidate(TRUE);
		TRACKMOUSEEVENT	csTME;	
		csTME.cbSize=sizeof(TRACKMOUSEEVENT);
		csTME.dwFlags=TME_LEAVE;
		csTME.hwndTrack=m_hWnd;
		csTME.dwHoverTime=1;
		_TrackMouseEvent(&csTME);
	}
	if (GetCapture() != this)
		return;

	BOOL bWasMouseDown = m_bMouseDown;

	CRect rect;
	GetClientRect(rect);
	m_bMouseDown = rect.PtInRect(point);

	if (bWasMouseDown != m_bMouseDown)
		RedrawCheck();

	CButton::OnMouseMove(nFlags, point);
}	// OnMouseMove

//-------------------------------------------------------------------
//
void CMacRadioButton::OnLButtonDblClk(UINT /*nFlags*/, CPoint point) 
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
void CMacRadioButton::OnKeyUp(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/) 
//
// Return Value:	None.
//
// Parameters	:	nChar - Specifies the virtual-key code of the given key.
//						nRepCnt - Repeat count (the number of times the keystroke 
//							is repeated as a result of the user holding down the key).
//						nFlags - Specifies the scan code, key-transition code, 
//							previous key state, and context code
//
// Remarks		:	The framework calls this member function when a 
//						nonsystem key is released. 
//
{
	if (nChar == VK_SPACE)
	{
		m_bMouseDown = FALSE;
		RedrawCheck();
	}
}	// OnKeyUp

//-------------------------------------------------------------------
//
void CMacRadioButton::OnKeyDown(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/) 
//
// Return Value:	None.
//
// Parameters	:	nChar - Specifies the virtual-key code of the given key.
//						nRepCnt - Repeat count (the number of times the keystroke 
//							is repeated as a result of the user holding down the key).
//						nFlags - Specifies the scan code, key-transition code, 
//							previous key state, and context code
//
// Remarks		:	The framework calls this member function when a 
//						nonsystem key is pressed.
//
{
	if (nChar == VK_SPACE)
	{
		if (!m_bMouseDown)
		{
			m_bMouseDown = TRUE;
			RedrawCheck();
		}
	}
}	// OnKeyDown

//-------------------------------------------------------------------
//
void CMacRadioButton::OnSysColorChange() 
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
	
	InitColors();
}	// OnSysColorChange

//-------------------------------------------------------------------
//
void CMacRadioButton::SetCheck(int nCheck)
//
// Return Value:	None.
//
// Parameters	:	nCheck - Specifies the check state (checked or unchecked,
//							there is no indeterminate state). 
//
// Remarks		:	Sets or resets the check state of a radio button or check box.
//
{
	BOOL bWasChecked = m_bChecked;

	m_bChecked = nCheck;

	if (m_bChecked != bWasChecked)
		/*RedrawCheck();*/
		Invalidate();
}	// SetCheck

//-------------------------------------------------------------------
//
int CMacRadioButton::GetCheck() const
//
// Return Value:	1 if checked, 0 otherwise.
//
// Parameters	:	None.
//
// Remarks		:	Retrieves the check state of the radio button.
//
{
	return m_bChecked;
}	// GetCheck

//-------------------------------------------------------------------
//
void CMacRadioButton::RedrawCheck()
//
// Return Value:	None.
//
// Parameters	:	None.
//
// Remarks		:	Redraws the check part of the button.
//
{
	CRect rect;
	GetClientRect(rect);
	int nHeight = 12;
	int nAdjust = (rect.Height() - nHeight) >> 1;
	CRect rectCheck;
	if (GetStyle() & BS_LEFTTEXT)
		rectCheck.SetRect(rect.right - nHeight, rect.top + nAdjust, 
								rect.right/* + 2*/, rect.top + nHeight + nAdjust);
	else
		rectCheck.SetRect(rect.left, rect.top + nAdjust, 
								rect.left + nHeight/* + 2*/, rect.top + nHeight + nAdjust);

	InvalidateRect(rectCheck);
}	// RedrawCheck

//-------------------------------------------------------------------
//
void CMacRadioButton::UncheckOtherRadios()
//
// Return Value:	None.
//
// Parameters	:	None.
//
// Remarks		:	Attempts to uncheck all other radio buttons in the group.
//
{
	// Go backwards (GetNextDlgItem will wrap around to the end of the group).
	CWnd *pParent = GetParent();
	ASSERT(pParent);

	CWnd *pWnd = pParent->GetNextDlgGroupItem((CWnd *)this, TRUE);
	while (pWnd && pWnd != (CWnd *)this)
	{
		if (pWnd->GetStyle() & BS_AUTORADIOBUTTON)
		{
			if (pWnd->IsKindOf(RUNTIME_CLASS(CMacRadioButton)))
				((CMacRadioButton*)pWnd)->SetCheck(0);
			else if (pWnd->IsKindOf(RUNTIME_CLASS(CButton)))
				((CButton*)pWnd)->SetCheck(0);
		}
		pWnd = pParent->GetNextDlgGroupItem(pWnd, TRUE);
	}
}	// UncheckOtherRadios

//-------------------------------------------------------------------
//
void CMacRadioButton::InitColors()
//
// Return Value:	None.
//
// Parameters	:	None.
//
// Remarks		:	Sets the COLORREF member variables.
//
{
	m_clrHilight = ::GetSysColor(COLOR_3DHILIGHT);
	m_clrRadioFace = RGB(221,221,221);//::GetSysColor(COLOR_3DFACE);
	m_clrShadow = ::GetSysColor(COLOR_3DSHADOW);
	m_clrLightFace = ::GetSysColor(COLOR_3DLIGHT);
	m_clrDkShadow = RGB(0,128,0);//::GetSysColor(COLOR_3DDKSHADOW);
	m_clrbackground = ::GetSysColor(COLOR_3DFACE);
	m_clrFouse = RGB(0,128,0);


	// Create some intermediate colors. This is done by averaging two colors.
	BYTE byRedFace = GetRValue(m_clrRadioFace);
	BYTE byGreenFace = GetGValue(m_clrRadioFace);
	BYTE byBlueFace = GetBValue(m_clrRadioFace);
	BYTE byRedShadow = GetRValue(m_clrShadow);
	BYTE byGreenShadow = GetGValue(m_clrShadow);
	BYTE byBlueShadow = GetBValue(m_clrShadow);
	BYTE byRedDkShadow = GetRValue(m_clrDkShadow);
	BYTE byGreenDkShadow = GetGValue(m_clrDkShadow);
	BYTE byBlueDkShadow = GetBValue(m_clrDkShadow);

	m_clrLightShadow = RGB(byRedFace + ((byRedShadow - byRedFace) >> 1),
										 byGreenFace + ((byGreenShadow - byGreenFace) >> 1),
										 byBlueFace + ((byBlueShadow - byBlueFace) >> 1));

	m_clrDarkDkShadow = RGB(byRedShadow + ((byRedDkShadow - byRedShadow) >> 1),
											byGreenShadow + ((byGreenDkShadow - byGreenShadow) >> 1),
											byBlueShadow + ((byBlueDkShadow - byBlueShadow) >> 1));
}	// InitColors

void CMacRadioButton::SetBackGround(CDC* pDC)
{
	CRect rect, rectS;
	CBitmap bmp, *pOldBitmap;
	
	GetClientRect(rect);
	GetWindowRect(rectS);	
	GetParent()->ScreenToClient(rectS);
	BITMAP bitmap;	
	pDC->GetCurrentBitmap()->GetBitmap(&bitmap);	
	if (rectS.left>bitmap.bmWidth || rectS.top>bitmap.bmHeight)
	{
		m_bSetBackGround=FALSE;
	}
	else
	{
		m_bSetBackGround=TRUE;
		m_dcBk.DeleteDC();
		m_dcBk.CreateCompatibleDC(pDC);
		bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
		pOldBitmap = m_dcBk.SelectObject(&bmp);
		m_dcBk.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, rectS.left, rectS.top, SRCCOPY);
		bmp.DeleteObject();
	}
}

LRESULT CMacRadioButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bOver=FALSE;
	Invalidate();
	return 0;
}

void CMacRadioButton::SetBackGroundColor(COLORREF clr)
{
	m_clrbackground=clr;
}
void CMacRadioButton::SetRadioBtnImgPath(CString strChecked/*=""*/,CString strUnchecked/*=""*/)
{
	m_strChecked=strChecked;
	m_strUnchecked=strUnchecked;
}