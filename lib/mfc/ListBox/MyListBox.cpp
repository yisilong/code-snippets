/////////////////////////////////////////////////////////////////////////////
// CEditHelper.cpp : implementation file
//////////////////////////////////////////////////////////////////////////////////////////////
#include "..//stdafx.h"
#include "MyListBox.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "MemDC.h"
typedef unsigned long  DWORD_PTR, *PDWORD_PTR;
/////////////////////////////////////////////////////////////////////////////
// CMyListBox.cpp : implementation file
#define TID_SCROLLDOWN  100
#define TID_SCROLLUP    101
#define	WM_APP_LB_ITEM_EDITED ( WM_APP + 04100 )
IMPLEMENT_DYNAMIC(CMyListBox, CListBox)

CMyListBox::CMyListBox()
{
	m_clrDlgBorder=RGB(205,205,205);
	m_clrItemBorderNormal=RGB(173,210,238);//RGB(236,236,236);
	m_clrItemBorderHover=RGB(173,210,238);
	m_clrItemBorderSelected=RGB(173,210,238);
	m_clrItemTopStartNormal=Color(240,240,240);//Color(250,250,250);
	m_clrItemBottomEndNormal=Color(255,255,255);//Color(247,247,247);
	m_clrItemTopStartHover=Color(241,247,252);
	m_clrItemBottomEndHover=Color(230,240,251);
	m_clrItemTopStartSelected=Color(218,234,250);
	m_clrItemBottomEndSelected=Color(181,213,248);

	m_nPreHoverIndex=-1;

	m_clrText = RGB(0,0,255);
	m_clrSelText = RGB(0,0,255);


	m_clrDlgBackGround=RGB(254,250,247);
	m_dlgBrush.CreateSolidBrush(m_clrDlgBackGround);

	bScrollEnd=false;
	bWhellEnd=false;
}

CMyListBox::~CMyListBox()
{
	m_dlgBrush.DeleteObject();
}
BEGIN_MESSAGE_MAP(CMyListBox, CListBox)
    //{{AFX_MSG_MAP(CMyListBox)
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_ERASEBKGND()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
    ON_WM_MEASUREITEM_REFLECT()
	ON_CONTROL_REFLECT(LBN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyListBox message handlers
void CMyListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if ((int)lpDrawItemStruct->itemID < 0)
		return;
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rcItem=CRect(lpDrawItemStruct->rcItem);	
	ItemData *pItemData=(ItemData*)GetItemDataPtr(lpDrawItemStruct->itemID);
	UINT itemState=lpDrawItemStruct->itemState;
	int itemID=lpDrawItemStruct->itemID;
	BOOL bSelected=FALSE;

	BOOL bHover=(itemID==m_nPreHoverIndex)? TRUE:FALSE;
	CString sLabel;
	GetText(itemID, sLabel);
	Color clrStart,clrEnd;
	COLORREF clrBorder,clrText;
	//开始画
	if (itemState & ODS_SELECTED)
	{
		clrBorder=m_clrItemBorderSelected;
		clrStart=m_clrItemTopStartSelected;
		clrEnd=m_clrItemBottomEndSelected;
		clrText=m_clrSelText;
		bSelected=TRUE;
	}
	else
	{
		if (bHover)
		{
			clrBorder=m_clrItemBorderHover;
			clrStart=m_clrItemTopStartHover;
			clrEnd=m_clrItemBottomEndHover;
			clrText=m_clrText;
		}
		else
		{
			clrBorder=m_clrItemBorderNormal;
			clrStart=m_clrItemTopStartNormal;
			clrEnd=m_clrItemBottomEndNormal;
			clrText=m_clrText;
		}
	}

	CDC memDC;
	CBitmap memBmp;
	memDC.CreateCompatibleDC(pDC);
	CRect rcFull;
	GetClientRect(rcFull);
	memBmp.CreateCompatibleBitmap(pDC,rcFull.Width(),rcFull.Height());
	CBitmap *pOldBmp=memDC.SelectObject(&memBmp);

	//填充底色
	memDC.StretchBlt(rcItem.left,rcItem.top,rcItem.Width(),rcItem.Height(),&m_memDC,0,0,rcItem.Width(),rcItem.Height(),SRCCOPY);

	CRect rcTemp(&rcItem);	
	int  nInterval=5; //模拟各item的间隔
 	rcTemp.DeflateRect(0,nInterval,0,nInterval);
	
	Image *pImg;
	ImageFromIDResource(AfxGetInstanceHandle(),IDR_PNG_BG,"png",pImg);
	Graphics myGraphics(memDC.m_hDC);
	myGraphics.DrawImage(pImg,rcTemp.left,rcTemp.top,rcTemp.Width(),rcTemp.Height());
	memDC.SetBkMode(TRANSPARENT);
	COLORREF colorTextSave = memDC.SetTextColor(clrText);
	CFont font;
	font.CreateFont(
		20,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,				   // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("微软雅黑"));            
	CFont *pOldFont=memDC.SelectObject(&font);
	memDC.DrawText( sLabel, -1, &rcTemp, DT_WORDBREAK  | DT_SINGLELINE | DT_VCENTER );
	memDC.SelectObject(pOldFont);
	memDC.SetTextColor(colorTextSave);

	pDC->StretchBlt(rcItem.left,rcItem.top,rcItem.Width(),rcItem.Height(),
		&memDC,rcItem.left,rcItem.top,rcItem.Width(),rcItem.Height(),SRCCOPY);
	memDC.SelectObject(pOldBmp);	
	memBmp.DeleteObject();
	memDC.DeleteDC();
}

void CMyListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// all items are of fixed size
	// must use LBS_OWNERDRAWVARIABLE for this to work
    ASSERT(lpMeasureItemStruct->CtlType == ODT_LISTBOX);	
// 	int nItem = lpMeasureItemStruct->itemID;
// 	CPaintDC dc(this);
// 	CString sLabel;
// 	CListBox::GetText(nItem, sLabel);
// 	int itemHeight = dc.DrawText( sLabel, -1, csRect, DT_CALCRECT | DT_WORDBREAK);
    lpMeasureItemStruct->itemHeight =60; //itemHeight+6*GetSystemMetrics(SM_CXBORDER);
}

int CMyListBox::AddString(LPCTSTR lpszItem, CString strPathIcon)
{
    int nItem=((CListBox*)this)->AddString(lpszItem);
	if (nItem >= 0)
	{
		ItemData *pItemData=new ItemData;
		strcpy(pItemData->chImgPath,strPathIcon.GetBuffer(0));
		SetItemDataPtr(nItem,(void*)pItemData);
	}
	return nItem;
}

void CMyListBox::OnMouseMove(UINT nFlags, CPoint point)
{
	if (GetCount()<=0)
	{
		return;
	}
	TRACKMOUSEEVENT   tme;
	tme.cbSize=sizeof(TRACKMOUSEEVENT);
	tme.dwFlags=TME_LEAVE;         
	tme.dwHoverTime=HOVER_DEFAULT;       
	tme.hwndTrack=m_hWnd;     
     _TrackMouseEvent(&tme);  
	BOOL bOutside;
	int nIdexNow=ItemFromPoint(point,bOutside);
	if (nIdexNow!=LB_ERR)
	{
		if (m_nPreHoverIndex!=nIdexNow)
		{
			SetRedraw(FALSE);
			CRect rect;
			if (m_nPreHoverIndex!=-1)
			{
				GetItemRect(m_nPreHoverIndex,rect);
				InvalidateRect(rect);
			}
			m_nPreHoverIndex=nIdexNow;
		    GetItemRect(nIdexNow,rect);			
			InvalidateRect(rect);
			SetRedraw(TRUE);
		}
	}
	CListBox::OnMouseMove(nFlags, point);
}

LRESULT CMyListBox::OnMouseLeave(WPARAM wParam  ,LPARAM  lParam )
{
	if (m_nPreHoverIndex!=-1)
	{
		CRect rc;
		GetItemRect(m_nPreHoverIndex,rc);
		InvalidateRect(rc);	
		m_nPreHoverIndex=-1;
	}
	return 0;
}

void CMyListBox::OnDestroy() 
{
	CListBox::OnDestroy();
	
	// TODO: Add your message handler code here
	int ItemCount;
	ItemCount = GetCount();
	ItemData *pItemData=NULL;
	for (int i = 0; i < ItemCount; i++)
	{
		pItemData = (ItemData*) GetItemDataPtr(i);
		delete pItemData;
	}
}// CMyListBox
///////////////////////////////////////////////////////////////////////////////////////

HBRUSH CMyListBox::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
// 	CRect rc;
// 	GetClientRect(rc);
// //	rc.InflateRect(1,1,1,1);
// 	CPen pen;
// 	pen.CreatePen(PS_SOLID,1,m_clrDlgBorder);
// 	
// 	CBrush *pBrh=(CBrush*)pDC->SelectStockObject(NULL_BRUSH);
// 	CPen *pOldPen=pDC->SelectObject(&pen);
// 	pDC->Rectangle(rc);
// 	pDC->SelectObject(pBrh);
// 	pDC->SelectObject(pOldPen);
	// TODO: Return a non-NULL brush if the parent's handler should not be called

	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)GetStockObject(HOLLOW_BRUSH);//m_dlgBrush;
}


BOOL CMyListBox::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
// 	if (bScrollEnd || bWhellEnd)
// 	{
// 		bScrollEnd=false;
// 		CRect rc;
// 		GetClientRect(rc);
// 		pDC->FillSolidRect(0,rc.Height()-50,rc.Width(),50,RGB(255,255,255));		
// 	}
	return TRUE;
}

void CMyListBox::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default

// 	if (nSBCode==SB_ENDSCROLL)
// 	{
// 		if (!bScrollEnd)
// 		{
// 			CRect rc;
// 			GetClientRect(rc);
// 			bScrollEnd=true;
// 			InvalidateRect(CRect(0,rc.Height()-50,rc.Width(),50));
// 		}
// 		return;
// 	}
// 	else if (SB_LINEUP || SB_PAGEUP || SB_THUMBPOSITION  || SB_THUMBTRACK || SB_TOP )
// 	{
// 		bScrollEnd=false;
// 	}	
	CListBox::OnVScroll(nSBCode, nPos, pScrollBar);	
}

BOOL CMyListBox::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
// 	if (zDelta<0)
// 	{
// 		if (!bWhellEnd)
// 		{
// 			CRect rc;
// 			GetClientRect(rc);
// 			bWhellEnd=true;
// 			InvalidateRect(CRect(0,rc.Height()-50,rc.Width(),50));
// 		}		
// 	}
// 	else
// 	{
// 		bWhellEnd=false;
// 	}	

	return CListBox::OnMouseWheel(nFlags, zDelta, pt);
}

void CMyListBox::OnSelchange() 
{
	// TODO: Add your control notification handler code here
// 	static bool bFirst=true;
// 	if (GetCurSel()==GetCount()-1)
// 	{
// 		if (bFirst)
// 		{
// 			Invalidate();
// 			CRect rc;
// 			GetClientRect(rc);
// 			bFirst=false;
// 			bScrollEnd=true;
// 			InvalidateRect(CRect(0,rc.Height()-50,rc.Width(),50));
// 		}		
// 	}
// 	else
// 	{
// 		bFirst=true;
// 	}
}

void CMyListBox::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CListBox::PreSubclassWindow();

	static BOOL bInit=FALSE;
	if (!bInit)
	{
		CWnd *pParent=GetParent();
		if (pParent)  //保存父窗口dc
		{
			bInit=TRUE;
			CDC *pParentDC=pParent->GetDC();
			CRect rcMe,rcParent; //rcMe在出现滚动条的时候以后会改变
			GetWindowRect(rcMe);
			pParent->GetClientRect(rcParent);		
			
			CDC memDC;
			memDC.CreateCompatibleDC(pParentDC);
			CBitmap bmp;
			bmp.CreateCompatibleBitmap(pParentDC,rcParent.Width(),rcParent.Height());
			memDC.SelectObject(&bmp);
			::SendMessage(pParent->GetSafeHwnd(),WM_ERASEBKGND,(WPARAM)memDC.m_hDC,0);

			bmp.DeleteObject();
			CDC *pDC=GetDC();
			bmp.CreateCompatibleBitmap(pDC,rcMe.Width(),rcMe.Height());
			m_memDC.CreateCompatibleDC(pDC);
			m_memDC.SelectObject(&bmp);
			m_memDC.StretchBlt(0,0,rcMe.Width(),rcMe.Height(),&memDC,rcMe.left,rcMe.top+31,rcMe.Width(),rcMe.Height(),SRCCOPY);
			bmp.DeleteObject();
			ReleaseDC(pDC);
			ReleaseDC(pParentDC);
		}
	}

}
