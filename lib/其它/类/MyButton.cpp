#include "stdafx.h"
#include "math.h"
#include "MyButton.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//download by www.codefans.net
/////////////////////////////////////////////////////////////////////////////
// CMyButton

CMyButton::CMyButton()
{
	m_clrText=RGB(0,0,0);
	m_clrBack=RGB(161,192,245);
	m_clrLeft=RGB(97,244,78);
	m_clrRight=RGB(52,148,39);
	m_clrFlatBack=GetSysColor(COLOR_3DFACE);
	m_blnMouseOn=FALSE;
	intStyle=0;
	intAlgin=0;
	intButtonFace=0;
	m_BackBrush.CreateSolidBrush(m_clrFlatBack);
	m_bIsDefault = FALSE;
	m_nTypeStyle=SS_TYPEMASK;
}
CMyButton::~CMyButton()
{
	m_rgn.DeleteObject();
	DestroyXIcon();
	m_BackBrush.DeleteObject();
}

BEGIN_MESSAGE_MAP(CMyButton, CButton)
	//{{AFX_MSG_MAP(CMyButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_GETDLGCODE()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_CTLCOLOR_REFLECT()
	ON_MESSAGE(BM_SETSTYLE, OnSetStyle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyButton message handlers

void CMyButton::PreSubclassWindow() 
{
	CButton::PreSubclassWindow();
	UINT nBS;
	nBS = GetButtonStyle();
	m_nTypeStyle = nBS & SS_TYPEMASK;
	if (m_nTypeStyle == BS_DEFPUSHBUTTON)
	{
		m_bIsDefault = TRUE;
		m_nTypeStyle = BS_PUSHBUTTON;
	}
	ModifyStyle(SS_TYPEMASK, BS_OWNERDRAW, SWP_FRAMECHANGED);
	CRect rect;
	GetClientRect(rect);
	m_rgn.DeleteObject(); 
	SetWindowRgn(NULL, FALSE);
	if(intButtonFace==1)
	m_rgn.CreateRoundRectRgn(rect.left,rect.top,rect.right,rect.bottom,10,10);
	if(intButtonFace==0)
	m_rgn.CreateRectRgn(rect.left,rect.top,rect.right,rect.bottom);
	if(intButtonFace==2)
	m_rgn.CreateEllipticRgn(rect.left,rect.top,rect.right,rect.bottom);
	SetWindowRgn(m_rgn, TRUE);
	ClientToScreen(rect);
	CWnd* pParent = GetParent();
	if (pParent) pParent->ScreenToClient(rect);
	MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
}

void CMyButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	ASSERT(lpDrawItemStruct != NULL);
	CDC* pDC   = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rect = lpDrawItemStruct->rcItem;
	UINT state = lpDrawItemStruct->itemState;
	UINT nStyle = GetStyle();
	CRgn rgn1,rgn2;
	int r1,g1,b1;
	if(intButtonFace==1)
	{
		rgn1.CreateRoundRectRgn(rect.left,rect.top,rect.right,rect.bottom,10,10);
		rgn2.CreateRoundRectRgn(rect.left+1,rect.top+1,rect.right-1,rect.bottom-1,10,10);
	}
	if(intButtonFace==0)
	{
		rgn1.CreateRectRgn(rect.left,rect.top,rect.right,rect.bottom);
		rgn2.CreateRectRgn(rect.left+1,rect.top+1,rect.right-1,rect.bottom-1);
	}
	if(intButtonFace==2)
	{
		rgn1.CreateEllipticRgn(rect.left,rect.top,rect.right,rect.bottom);
		rgn2.CreateEllipticRgn(rect.left+1,rect.top+1,rect.right-1,rect.bottom-1);
	}
	pDC->SelectClipRgn(&rgn1);
	int nSavedDC = pDC->SaveDC();
	CBrush brush1,brush2;
	if(!(nStyle&BS_NOTIFY))
	{
		if ((state & ODS_FOCUS)||(state & ODS_SELECTED))
		{	
			if(intStyle!=2)
			{
				r1=GetRValue(m_clrBack);
				g1=GetGValue(m_clrBack);
				b1=GetBValue(m_clrBack);
				for(int i=rect.Height()/2;i>0;i--)
				{
					r1=(r1+10)>255?255:(r1+10);
					g1=(g1+10)>255?255:(g1+10);
					b1=(b1+10)>255?255:(b1+10);
					CPen pen(PS_SOLID, 1, RGB(r1, g1, b1));
					CPen *old = pDC->SelectObject(&pen);
					pDC->MoveTo(rect.left,rect.top+i);
					pDC->LineTo(rect.right,rect.top+i);
					pDC->MoveTo(rect.left,rect.bottom-i);
					pDC->LineTo(rect.right,rect.bottom-i);
					pDC->SelectObject(old);
				}
			}
			else
			{
				pDC->FillSolidRect(rect,m_clrBack);
			}
			r1=GetRValue(m_clrRight);
			g1=GetGValue(m_clrRight);
			b1=GetBValue(m_clrRight);
			r1=(r1+30)>255?255:(r1+30);
			g1=(g1+30)>255?255:(g1+30);
			b1=(b1+30)>255?255:(b1+30);
			brush2.CreateSolidBrush(m_clrRight);
			brush1.CreateSolidBrush(RGB(r1,g1,b1));
			pDC->FrameRgn(&rgn1,&brush1,1,1);
			pDC->FrameRgn(&rgn2,&brush2,1,1);
		}
		else if(m_blnMouseOn)
		{
			if(intStyle!=2)
			{
				r1=GetRValue(m_clrBack);
				g1=GetGValue(m_clrBack);
				b1=GetBValue(m_clrBack);
				for(int i=rect.Height()/2;i>0;i--)
				{
					r1=(r1+10)>255?255:(r1+10);
					g1=(g1+10)>255?255:(g1+10);
					b1=(b1+10)>255?255:(b1+10);
					CPen pen(PS_SOLID, 1, RGB(r1, g1, b1));
					CPen *old = pDC->SelectObject(&pen);
					pDC->MoveTo(rect.left,rect.top+i);
					pDC->LineTo(rect.right,rect.top+i);
					pDC->MoveTo(rect.left,rect.bottom-i);
					pDC->LineTo(rect.right,rect.bottom-i);
					pDC->SelectObject(old);
				}
			}
			else
			{
				pDC->FillSolidRect(rect,m_clrBack);
			}
			r1=GetRValue(m_clrLeft);
			g1=GetGValue(m_clrLeft);
			b1=GetBValue(m_clrLeft);
			r1=(r1+30)>255?255:(r1+30);
			g1=(g1+30)>255?255:(g1+30);
			b1=(b1+30)>255?255:(b1+30);
			brush2.CreateSolidBrush(m_clrLeft);
			brush1.CreateSolidBrush(RGB(r1,g1,b1));
			pDC->FrameRgn(&rgn1,&brush1,1,1);
			pDC->FrameRgn(&rgn2,&brush2,1,1);
		}
		else 
		{
			if(nStyle&BS_FLAT)
			{
				CBrush bs;
				bs.CreateSolidBrush(m_clrFlatBack);
				CRect rcBack;
				GetClientRect(rcBack);
				pDC->FillRect(rcBack,&m_BackBrush);
			}
			else
			{
				if(intStyle!=2)
				{
					r1=GetRValue(m_clrBack);
					g1=GetGValue(m_clrBack);
					b1=GetBValue(m_clrBack);
					for(int i=rect.Height()/2;i>0;i--)
					{
						r1=(r1+13)>255?255:(r1+13);
						g1=(g1+13)>255?255:(g1+13);
						b1=(b1+13)>255?255:(b1+13);
						CPen pen(PS_SOLID, 1, RGB(r1, g1, b1));
						CPen *old = pDC->SelectObject(&pen);
						pDC->MoveTo(rect.left,rect.top+i);
						pDC->LineTo(rect.right,rect.top+i);
						pDC->MoveTo(rect.left,rect.bottom-i);
						pDC->LineTo(rect.right,rect.bottom-i);
						pDC->SelectObject(old);
					}
				}
				else
				{
					pDC->FillSolidRect(rect,m_clrBack);
				}
				r1=GetRValue(m_clrBack);
				g1=GetGValue(m_clrBack);
				b1=GetBValue(m_clrBack);
				r1=(r1+30)>255?255:(r1+30);
				g1=(g1+30)>255?255:(g1+30);
				b1=(b1+30)>255?255:(b1+30);
				brush2.CreateSolidBrush(m_clrBack);
				brush1.CreateSolidBrush(RGB(r1,g1,b1));
				pDC->FrameRgn(&rgn1,&brush1,1,1);
				pDC->FrameRgn(&rgn2,&brush2,1,1);
			}

		}
	}
	else
	{
		pDC->FillSolidRect(rect,m_clrBack);
	}
	//输出文字	
	CFont *ft=GetFont();
	LOGFONT logft;
	ft->GetLogFont(&logft);
	CRect rcIcon(rect);
	if(intStyle==1)
	{	
		switch(intAlgin)
		{
		case 1:	//文字在下端
			rcIcon.bottom=rect.bottom-abs(logft.lfHeight);
			if(rcIcon.bottom<0) rcIcon.bottom=0;
			DrawIconX(pDC,rcIcon);
			rect.top=rcIcon.bottom-(rcIcon.Height()-dwIconHeight)/2;
			break;
		case 2://文字在上
			rcIcon.top=abs(logft.lfHeight)+rect.top;
			DrawIconX(pDC,rcIcon);
			rect.bottom=rcIcon.top+(rcIcon.Height()-dwIconHeight)/2;
			break;
		case 3://文字在左
			GetXiconInf();
			rcIcon.left=(long)(rect.Width()*0.9+rect.left-dwIconWidth-5);
			rcIcon.right=(long)(rect.Width()*0.9+rect.left);
			DrawIconX(pDC,rcIcon);
			rect.right=rcIcon.left;
			break;
		case 0://文字在右
		default:
			DrawIconX(pDC,rcIcon);
			rect.left=(long)(dwIconWidth+rect.left+rect.Width()*0.1);
			break;
		}
	}
	if(intStyle==0)
	{
		switch(intAlgin)
		{
		case 1:	//文字在下端
			rect.top=rect.bottom-abs(logft.lfHeight)-10;
			break;
		case 2://文字在上
			rect.bottom=abs(logft.lfHeight)+10+rect.top;
			break;
		case 3://文字在左
		case 0://文字在右
		default:
			break;
		}
	}
	if(intStyle==2)
	{
		pDC->FrameRgn(&rgn1,&brush1,1,1);
		pDC->FrameRgn(&rgn2,&brush2,1,1);
		switch(intAlgin)
		{
		case 1:	//文字在下端
			rcIcon.bottom=rect.bottom-abs(logft.lfHeight);
			if(rcIcon.bottom<0) rcIcon.bottom=0;
			DrawImgX(pDC,rcIcon);
			rect.top=rcIcon.bottom-(rcIcon.Height()-m_imgSize.cy)/2;
			break;
		case 2://文字在上
			rcIcon.top=abs(logft.lfHeight)+rect.top;
			DrawImgX(pDC,rcIcon);
			rect.bottom=rcIcon.top+(rcIcon.Height()-m_imgSize.cy)/2;
			break;
		case 3://文字在左
			rcIcon.left=(long)(rect.Width()*0.9+rect.left-m_imgSize.cx-5);
			rcIcon.right=(long)(rect.Width()*0.9+rect.left);
			DrawImgX(pDC,rcIcon);
			rect.right=rcIcon.left;
			break;
		case 0://文字在右
		default:
			DrawImgX(pDC,rcIcon);
			rect.left=(long)(m_imgSize.cx+rect.left+rect.Width()*0.1);
			break;
		}
	}
	CPoint m_ptCentre=rect.CenterPoint();
	CString strText;
	GetWindowText(strText);
	if (!strText.IsEmpty())
	{
		pDC->SetTextColor(m_clrText);
		CSize Extent = pDC->GetTextExtent(strText);
		CPoint pt;
		if(intAlgin==1 || intAlgin==2)
		{
			pt.x = m_ptCentre.x - Extent.cx/2;
			pt.y=m_ptCentre.y - Extent.cy/2;
		}
		if(intAlgin==0)
		{
			pt.x=rect.left+15;
			pt.y=m_ptCentre.y - Extent.cy/2;
		}
		if(intAlgin==3)
		{
			pt.x=rect.right-Extent.cy-10;
			pt.y=m_ptCentre.y - Extent.cy/2;
		}
		if (state & ODS_SELECTED) pt.Offset(1,1);
		pDC->SetBkMode(TRANSPARENT);
		if ((state & ODS_DISABLED)&!(nStyle&BS_NOTIFY))	
			pDC->DrawState(pt, Extent, strText, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
		else if(nStyle&BS_NOTIFY)
		{	
			pDC->FillRect(rect,&m_BackBrush);
			if((state & ODS_FOCUS)||(state & ODS_SELECTED))
			{
				pDC->SetTextColor(m_clrRight);
				pDC->TextOut(pt.x+2, pt.y+2, strText);
			}
			else if(m_blnMouseOn)
			{

				pDC->SetTextColor(m_clrLeft);
				pDC->TextOut(pt.x+2, pt.y+2, strText);				
			}
			else
			{
				pDC->TextOut(pt.x, pt.y, strText);		
			}
		}
		else
		{
			pDC->TextOut(pt.x, pt.y, strText);
		}
	}
	pDC->SelectClipRgn(NULL);
	pDC->RestoreDC(nSavedDC);
	rgn1.DeleteObject();
	rgn2.DeleteObject();
}
void CMyButton::SetMaskColor(COLORREF colLeft,COLORREF colRight)
{
	m_clrRight=colRight;
	m_clrLeft=colLeft;
	this->Invalidate(TRUE);
}
void CMyButton::SetBackColor(COLORREF color)
{
	m_clrBack=color;
	this->Invalidate(TRUE);
}
void CMyButton::SetBackImage(UINT nIDResource)
{
	CBitmap bmp;
	intStyle=3;
	bmp.LoadBitmap(nIDResource);
	m_BackBrush.DeleteObject();
	m_BackBrush.CreatePatternBrush(&bmp);
}
void CMyButton::SetButtonFace(int intStyle)
{
	intButtonFace=intStyle;
	this->Invalidate();
}
void CMyButton::SetFlatBack(COLORREF colFlat)
{
	m_clrFlatBack=colFlat;
	m_BackBrush.DeleteObject();
	m_BackBrush.CreateSolidBrush(colFlat);
	this->Invalidate(TRUE);
}
void CMyButton::SetTextColor(COLORREF color)
{
	m_clrText=color;
	this->Invalidate(TRUE);
}
void CMyButton::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd*				wndUnderMouse = NULL;
	CWnd*				wndActive = this;
	TRACKMOUSEEVENT		csTME;
	CButton::OnMouseMove(nFlags, point);
	ClientToScreen(&point);
	wndUnderMouse = WindowFromPoint(point);
	// If the mouse enter the button with the left button pressed then do nothing
	if (nFlags & MK_LBUTTON) return;
	// If our button is not flat then do nothing
	wndActive = GetActiveWindow();
	if (wndUnderMouse && wndUnderMouse->m_hWnd == m_hWnd && wndActive)
	{
		if (!m_blnMouseOn)
		{
			m_blnMouseOn = TRUE;
			Invalidate();
			csTME.cbSize = sizeof(csTME);
			csTME.dwFlags = TME_LEAVE;
			csTME.hwndTrack = m_hWnd;
			::_TrackMouseEvent(&csTME);
		}
	} 
	else
	{
		m_blnMouseOn=FALSE;
		Invalidate();
	}
}
LRESULT CMyButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{	
	m_blnMouseOn=FALSE;
	Invalidate();
	return 0;	
}
void CMyButton::SetXIcon(UINT nIDResource)
{
	intStyle=1;
	Xicon=AfxGetApp()->LoadIcon(nIDResource);
	this->Invalidate();
}
void CMyButton::DrawIconX(CDC *pDC,CRect rcItem)
{
	GetXiconInf();
	int intTop=(rcItem.Height()-dwIconHeight)/2;
	if(intTop<0) intTop=0;
	intTop=rcItem.top+intTop;
	if(intAlgin==0)
	{
		int intLeft=rcItem.left+(int)(rcItem.Width()*0.1);
		pDC->DrawIcon(intLeft,intTop,Xicon);
	}
	else
	{
		int intLeft=(rcItem.Width()-dwIconWidth)/2;
		if(intLeft<0) intLeft=0;
		intLeft=intLeft+rcItem.left;
		pDC->DrawIcon(intLeft,intTop,Xicon);
	}
}
void CMyButton::DrawImgX(CDC *pDC,CRect rcItem)
{
	int cx,cy;
	if(intAlgin==0)
	{
		cx=rcItem.left+(int)(rcItem.Width()*0.1);
		cy=(rcItem.Height()-m_imgSize.cy)/2+rcItem.top;
	}
	else
	{
		cx=(rcItem.Width()-m_imgSize.cx)/2+rcItem.left;
		cy=(rcItem.Height()-m_imgSize.cy)/2+rcItem.top;
	}
	CPoint pt(cx,cy);
	m_imgList.Draw(pDC,0,pt,ILD_NORMAL);
}
void CMyButton::DestroyXIcon()
{
	if(Xicon) ::DestroyIcon(Xicon);
}
void CMyButton::GetXiconInf()
{
	BOOL		bRetValue;
	ICONINFO	ii;
	if (Xicon)
	{
		::ZeroMemory(&ii, sizeof(ICONINFO));
		bRetValue = ::GetIconInfo(Xicon, &ii);
		if (bRetValue == FALSE)
		{
			DestroyXIcon();
		} // if
		dwIconWidth	= (DWORD)(ii.xHotspot * 2);
		dwIconHeight= (DWORD)(ii.yHotspot * 2);
		::DeleteObject(ii.hbmMask);
		::DeleteObject(ii.hbmColor);
	}
}
void CMyButton::SetXAlign(int Align)
{
	intAlgin=Align;
	this->Invalidate();
}
void CMyButton::SetXStyle(int Style)
{
	intStyle=Style;
	this->Invalidate();
}
void CMyButton::SetForeImage(UINT nIDResource,CSize sz)
{
	intStyle=2;
	m_imgSize=sz;
	HBITMAP hbm=(HBITMAP)::LoadImage(AfxGetInstanceHandle(),
		                             MAKEINTRESOURCE(nIDResource),
									 IMAGE_BITMAP,
									 0,0,
									LR_CREATEDIBSECTION |LR_LOADMAP3DCOLORS);	
	CBitmap bm;
	bm.Attach(hbm);
	m_imgList.Create(sz.cx,sz.cy,ILC_COLOR16,0,0);
	m_imgList.Add(&bm,(CBitmap*)NULL);
}
UINT CMyButton::OnGetDlgCode() 
{
	UINT nCode = CButton::OnGetDlgCode();
	nCode |= (m_bIsDefault ? DLGC_DEFPUSHBUTTON : DLGC_UNDEFPUSHBUTTON);
	return nCode;
}
void CMyButton::OnSysColorChange() 
{
	CButton::OnSysColorChange();
}
HBRUSH CMyButton::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	return (HBRUSH)::GetStockObject(NULL_BRUSH); 
}
LRESULT CMyButton::OnSetStyle(WPARAM wParam, LPARAM lParam)
{
	UINT nNewType = (wParam & SS_TYPEMASK);
	if (nNewType == BS_DEFPUSHBUTTON)
	{
		m_bIsDefault = TRUE;
	}
	else if (nNewType == BS_PUSHBUTTON)
	{
		m_bIsDefault = FALSE;
	} 
	return DefWindowProc(BM_SETSTYLE,
		(wParam & ~SS_TYPEMASK) | BS_OWNERDRAW, lParam);
}
