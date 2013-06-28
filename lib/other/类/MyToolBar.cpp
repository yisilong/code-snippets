#include "stdafx.h"
#include "MyToolBar.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//download by www.codefans.net
struct CToolBarData
{
	WORD wVersion;
	WORD wWidth;
	WORD wHeight;
	WORD wItemCount;
	//WORD aItems[wItemCount]
	
	WORD* items()
	{ return (WORD*)(this+1); }
};

void TransparentBltEx( HDC hdcDest,    
					  int nXOriginDest,  
					  int nYOriginDest,  
					  int nWidthDest,    
					  int nHeightDest,   
					  HDC hdcSrc,        
					  int nXOriginSrc,   
					  int nYOriginSrc,   
					  int nWidthSrc,     
					  int nHeightSrc,    
					  UINT crTransparent 
					  )
{
	HBITMAP hOldImageBMP, hImageBMP = CreateCompatibleBitmap(hdcDest, nWidthDest, nHeightDest);	// 创建兼容位图
	HBITMAP hOldMaskBMP, hMaskBMP = CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);			// 创建单色掩码位图
	HDC		hImageDC = CreateCompatibleDC(hdcDest);
	HDC		hMaskDC = CreateCompatibleDC(hdcDest);
	hOldImageBMP = (HBITMAP)SelectObject(hImageDC, hImageBMP);
	hOldMaskBMP = (HBITMAP)SelectObject(hMaskDC, hMaskBMP);
	
	if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc)
		BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY);
	else
		StretchBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, 
		hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, SRCCOPY);
	
	SetBkColor(hImageDC, crTransparent);
	
	BitBlt(hMaskDC, 0, 0, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCCOPY);
	
	SetBkColor(hImageDC, RGB(0,0,0));
	SetTextColor(hImageDC, RGB(255,255,255));
	BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);
	
	SetBkColor(hdcDest,RGB(255,255,255));
	SetTextColor(hdcDest,RGB(0,0,0));
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);
	
	// or
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCPAINT);
	
	
	SelectObject(hImageDC, hOldImageBMP);
	DeleteDC(hImageDC);
	SelectObject(hMaskDC, hOldMaskBMP);
	DeleteDC(hMaskDC);
	DeleteObject(hImageBMP);
	DeleteObject(hMaskBMP);
}
/////////////////////////////////////////////////////////////////////////////
// CMyToolBar
IMPLEMENT_DYNAMIC(CMyToolBar,CToolBar)

BEGIN_MESSAGE_MAP(CMyToolBar, CToolBar)
//{{AFX_MSG_MAP(CMyToolBar)
ON_WM_ERASEBKGND()
ON_WM_WINDOWPOSCHANGING()
ON_WM_PAINT()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
CMyToolBar::CMyToolBar()
{	
	m_bBKStyle=bs_RGB;
	m_clrBack=RGB(161,192,245);
	m_bkbrush.CreateSolidBrush(RGB(161,192,245));
}
CMyToolBar::~CMyToolBar()
{
	try{
		delete[] m_pbtButtonStyle;
	}
	catch(...)
	{}
}
/////////////////////////////////////////////////////////////////////////////
// CMyToolBar message handlers

void CMyToolBar::EraseNonClient()
{
	// get window DC that is clipped to the non-client area
	CWindowDC dc(this);
	CRect rectClient;
	GetClientRect(rectClient);
	CRect rectWindow;
	GetWindowRect(rectWindow);
	ScreenToClient(rectWindow);
	rectClient.OffsetRect(-rectWindow.left, -rectWindow.top);
	dc.ExcludeClipRect(rectClient);
	
	// draw borders in non-client area
	rectWindow.OffsetRect(-rectWindow.left, -rectWindow.top);
	DrawBorders(&dc, rectWindow);
	
	// erase parts not drawn
	dc.IntersectClipRect(rectWindow);
	SendMessage(WM_ERASEBKGND, (WPARAM)dc.m_hDC);
	
	DrawGripper(&dc, rectWindow); //
}

BOOL CMyToolBar::OnEraseBkgnd(CDC *pDC)
{
    CRect rect;
	GetWindowRect(&rect);
	ScreenToClient(&rect);
	int r1,g1,b1,i;
	switch(m_bBKStyle)
	{
	case bs_RGB:
		r1=GetRValue(m_clrBack);
		g1=GetGValue(m_clrBack);
		b1=GetBValue(m_clrBack);
		for(i=rect.Height()/2;i>=0;i--)
		{
			r1=(r1+3)>255?255:(r1+3);
			g1=(g1+3)>255?255:(g1+3);
			b1=(b1+3)>255?255:(b1+3);
			CPen pen(PS_SOLID, 1, RGB(r1, g1, b1));
			CPen *old = pDC->SelectObject(&pen);
			pDC->MoveTo(rect.left,rect.top+i);
			pDC->LineTo(rect.right,rect.top+i);
			pDC->MoveTo(rect.left,rect.bottom-i);
			pDC->LineTo(rect.right,rect.bottom-i);
			pDC->SelectObject(old);
		}	
		break;
	case bs_BITMAP:
	case bs_ORG:
		pDC->FillRect(&rect,&m_bkbrush);
		break;
	default:
		CToolBar::OnEraseBkgnd(pDC);
		break;
	}
	CPen spen(PS_SOLID,1,RGB(255,255,255));
	CPen *mpen=pDC->SelectObject(&spen);
	pDC->MoveTo(rect.left,rect.bottom-3);
	pDC->LineTo(rect.right,rect.bottom-3);
	pDC->MoveTo(rect.left,rect.bottom-1);
	pDC->LineTo(rect.right,rect.bottom-1);
	CPen npen(PS_SOLID,1,RGB(178,178,178));
	pDC->SelectObject(&npen);
	pDC->MoveTo(rect.left,rect.bottom-2);
	pDC->LineTo(rect.right,rect.bottom-2);
	pDC->SelectObject(mpen);
	return TRUE;
}

void CMyToolBar::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CToolBar::OnWindowPosChanging(lpwndpos);
	
	// TODO: Add your message handler code here
	RepaintBackground();
}
void CMyToolBar::RepaintBackground()
{
	// get parent window (there should be one)
	CWnd* pParent = GetParent();
	if (pParent) {
		// get rect for this toolbar
		CRect rw; GetWindowRect(&rw);
		
		// convert rect to parent coords
		CRect rc = rw; pParent->ScreenToClient(&rc);
		// invalidate this part of parent
		pParent->InvalidateRect(&rc);
		// now do all the other toolbars (etc) that belong to the parent
		for (
			CWnd* pSibling = pParent->GetWindow(GW_CHILD);
		pSibling;
		pSibling = pSibling->GetNextWindow(GW_HWNDNEXT)
			) {
			// but do not draw ourselves
			if (pSibling == this) continue;
			// convert rect to siblings coords
			CRect rc = rw; pSibling->ScreenToClient(&rc);
			// invalidate this part of sibling
			pSibling->InvalidateRect(&rc);
		}
	}
}


void CMyToolBar::OnPaint() 
{
	//CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	// standard tolbar
    CToolBar::OnPaint();
	
    // erase the background
    EraseNonClient();
	
    // plus separators
    DrawSeparators();
	// Do not call CToolBar::OnPaint() for painting messages
}
// Draw the separators in the client area
void CMyToolBar::DrawSeparators()
{
	// get a dc for the client area
	CClientDC dc(this);
	// draw the separators on it
	DrawSeparators(&dc);
}
// Draw the separators
void CMyToolBar::DrawSeparators(CClientDC* pDC)
{
	// horizontal vs vertical
	bool ishorz = (m_dwStyle & CBRS_ORIENT_HORZ) != 0;
	// get number of buttons
	int nIndexMax = (int)DefWindowProc(TB_BUTTONCOUNT, 0, 0);
	int nIndex;
	
	// try each button
	for (nIndex = 0; nIndex < nIndexMax; nIndex++)
	{
		UINT dwStyle=GetButtonStyle(nIndex);
		UINT wStyle=LOWORD(dwStyle);
		
		// if it is a separator
		if (wStyle == TBBS_SEPARATOR)
		{
			// get it's rectangle and width
			CRect rect;
			GetItemRect(nIndex,rect);
			
			// if small enough to be a true separator
			int w=rect.Width();
			if (w <= 8)
			{
				if (ishorz)
				{
					// draw the separator bar in the middle
					CRect rectbar=rect;
					int x=(rectbar.left+rectbar.right)/2;
					rectbar.left=x-1;
					rectbar.right=x+1;
					pDC->Draw3dRect(rectbar,::GetSysColor(COLOR_3DSHADOW),::GetSysColor(COLOR_3DHILIGHT));
				}
				else
				{
					// draw the separator bar in the middle
					CRect rectbar = rect;
					rectbar.left = rectbar.left - m_sizeButton.cx;
					rectbar.right = rectbar.left + m_sizeButton.cx;
					rectbar.top = rectbar.bottom+1;
					rectbar.bottom = rectbar.top+3;
					int y = (rectbar.top+rectbar.bottom)/2;
					rectbar.top = y-1;
					rectbar.bottom = y+1;
					pDC->Draw3dRect(rectbar,::GetSysColor(COLOR_3DSHADOW),::GetSysColor(COLOR_3DHILIGHT));
					
				}
			}
		}
	}
}

// Draw the gripper at left or top
void CMyToolBar::DrawGripper(CWindowDC *pDC, CRect& rectWindow)
{
	// get the gripper rect (1 pixel smaller than toolbar)
	CRect gripper = rectWindow;
	CRect rc;
	CBrush brush1,brush2;
	brush1.CreateSolidBrush(RGB(192,192,192));
	brush2.CreateSolidBrush(RGB(255,255,255));
	gripper.DeflateRect(1,1);
	if (m_dwStyle & CBRS_FLOATING) {
		// no grippers
	} else if (m_dwStyle & CBRS_ORIENT_HORZ) {
		// gripper at left
		rc.top=gripper.top+3;
		rc.left=gripper.left+1;
		rc.right=rc.left+2;
		rc.bottom=rc.top+2;
		for(int i=0;i<(gripper.Height()-4)/5;i++)
		{
			//pDC->Draw3dRect(rc,RGB(128,128,128),RGB(255,255,255));//::GetSysColor(COLOR_3DHIGHLIGHT),::GetSysColor(COLOR_3DSHADOW));
			rc.OffsetRect(+1,+1);
			pDC->FillRect(&rc,&brush2) ;
			rc.OffsetRect(-1,-1);
			pDC->FillRect(&rc,&brush1) ;
			rc.OffsetRect(0,+5); 
		}
	} else {
		// gripper at top
		rc.top=gripper.top+1;
		rc.left=gripper.left+3;
		rc.right=rc.left+2;
		rc.bottom=rc.top+2;
		for(int i=0;i<(gripper.Width()-4)/5;i++)
		{
			//pDC->Draw3dRect(rc,RGB(128,128,128),RGB(255,255,255));//::GetSysColor(COLOR_3DHIGHLIGHT),::GetSysColor(COLOR_3DSHADOW));
			rc.OffsetRect(+1,+1);
			pDC->FillRect(&rc,&brush2) ;
			rc.OffsetRect(-1,-1);
			pDC->FillRect(&rc,&brush1) ;
			rc.OffsetRect(+5,0); 
		} 
	}
}


void CMyToolBar::SetBKColor(COLORREF color)
{
	m_bkbrush.DeleteObject(); 
	m_bkbrush.CreateSolidBrush(color);
	m_bBKStyle=bs_RGB;
	m_clrBack=color;
	SetButtonEx(m_lpszResourceName);
	Invalidate();
}
void CMyToolBar::SetDefaultStyle()
{
	SetBKColor(RGB(161,192,245));
}
void CMyToolBar::SetBKImage(UINT nIDResource)
{
	CBitmap tmp;
	tmp.LoadBitmap(nIDResource);
	m_bkbrush.DeleteObject();
	m_bkbrush.CreatePatternBrush(&tmp); 
	m_bBKStyle=bs_BITMAP;
	SetButtonEx(m_lpszResourceName);
	Invalidate();
}
void CMyToolBar::SetBKImage(LPCTSTR lpszResourceName)
{
	CBitmap tmp;
	tmp.LoadBitmap(lpszResourceName);
	m_bkbrush.DeleteObject();
	m_bkbrush.CreatePatternBrush(&tmp); 
	m_bBKStyle=bs_BITMAP;
	SetButtonEx(m_lpszResourceName);
	Invalidate();
}
BOOL CMyToolBar::LoadToolBar(UINT nIDResource)
{
	
	LPCTSTR lpszResourceName=MAKEINTRESOURCE(nIDResource);
	ASSERT_VALID(this);
	ASSERT(lpszResourceName != NULL);
	
	// determine location of the bitmap in resource fork
	HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_TOOLBAR);
	HRSRC hRsrc = ::FindResource(hInst, lpszResourceName, RT_TOOLBAR);
	if (hRsrc == NULL)
		return FALSE;
	
	CBitmap tmp;
	BITMAP BitMap; 
	tmp.LoadBitmap(lpszResourceName);
	if(tmp.GetBitmap(&BitMap)==0)
		return FALSE;
	
	HGLOBAL hGlobal = LoadResource(hInst, hRsrc);
	if (hGlobal == NULL)
		return FALSE;
	
	CToolBarData* pData = (CToolBarData*)LockResource(hGlobal);
	if (pData == NULL)
		return FALSE;
	ASSERT(pData->wVersion == 1);
    m_pbtButtonStyle=new BYTE[pData->wItemCount];
	UINT* pItems = new UINT[pData->wItemCount];
	for (int i = 0; i < pData->wItemCount; i++)
		pItems[i] = pData->items()[i];
	BOOL bResult = SetButtons(pItems, pData->wItemCount);
	delete[] pItems;
	
    m_nButtonCount=pData->wItemCount;
	for(int j = 0; j <pData->wItemCount; j++)
	{
		if(TBBS_SEPARATOR==GetButtonStyle(j))
		{
			m_nButtonCount--;
			m_pbtButtonStyle[j]=1;
		}
		else
			m_pbtButtonStyle[j]=0;
	}
	m_nSeparatorCount=pData->wItemCount-m_nButtonCount;
	pData->wWidth=BitMap.bmWidth/m_nButtonCount;
	pData->wHeight=(UINT)BitMap.bmHeight;
		
	m_nButtonWidth=BitMap.bmWidth/m_nButtonCount;
	m_nButtonHeight=pData->wHeight;
		
	if (bResult)
	{
		// set new sizes of the buttons
		CSize sizeImage(pData->wWidth, pData->wHeight);
		CSize sizeButton(pData->wWidth + 7, pData->wHeight + 7);
		m_ButtonSize=sizeButton;
		SetSizes(sizeButton, sizeImage);
		
		// load bitmap now that sizes are known by the toolbar control
		bResult = LoadBitmap(lpszResourceName);
	}
	m_ButtonImageSize.cx=pData->wWidth;
	m_ButtonImageSize.cy=pData->wHeight;
	UnlockResource(hGlobal);
	FreeResource(hGlobal);
	return bResult;
}

BOOL CMyToolBar::LoadBitmap(LPCTSTR lpszResourceName)
{
	ASSERT_VALID(this);
	ASSERT(lpszResourceName != NULL);
	
	// determine location of the bitmap in resource fork
	HINSTANCE hInstImageWell = AfxFindResourceHandle(lpszResourceName, RT_BITMAP);
	HRSRC hRsrcImageWell = ::FindResource(hInstImageWell, lpszResourceName, RT_BITMAP);
	if (hRsrcImageWell == NULL)
		return FALSE;
	
	
	HGLOBAL hglb;
	if ((hglb = LoadResource(hInstImageWell, hRsrcImageWell)) == NULL)
		return FALSE;
	
	LPBITMAPINFOHEADER lpBitmap = (LPBITMAPINFOHEADER)LockResource(hglb);
	if (lpBitmap == NULL)
		return NULL;
	
	m_wToolBarBitCount=lpBitmap->biBitCount;
	
	::FreeResource(hglb);
    //if(m_wToolBarBitCount<8)//如果工具栏颜色小于等于256色则调用原函数
	//{
	//	return CToolBar::LoadBitmap(lpszResourceName);
	//}
	m_lpszResourceName=lpszResourceName;
    SetButtonEx(lpszResourceName);
	
	return TRUE;
}
BOOL CMyToolBar::SetButtonEx(UINT nIDResource)
{
	LPCTSTR lpszResourceName=MAKEINTRESOURCE(nIDResource);
	return SetButtonEx(lpszResourceName);
}
BOOL CMyToolBar::SetButtonEx(LPCTSTR lpszResourceName)
{
    CClientDC rdc(this);
	CDC dc1,dc2,dc3,dc4;
	
	CBitmap bmp1,bmp2,bmp3,bmp4,bmpd;
	COLORREF pixelcolor,maskcolor;
	BITMAP bitmap;
	BYTE gray=0;
	CRect rc4;

	rc4.top=rc4.left=0;
	if (m_dwStyle & CBRS_ORIENT_HORZ)
	{
		rc4.right=m_ButtonSize.cx*m_nButtonCount+6*m_nSeparatorCount;
		rc4.bottom=m_ButtonSize.cy;
	}
	else
	{
		rc4.right=m_ButtonSize.cx;
		rc4.bottom=m_ButtonSize.cy*m_nButtonCount+6*m_nSeparatorCount;
	}
	
	bmp1.LoadBitmap(lpszResourceName);
	bmp1.GetBitmap(&bitmap); 
	bmp2.CreateBitmap(bitmap.bmWidth,bitmap.bmHeight,1,bitmap.bmBitsPixel,NULL); 
	bmp3.CreateBitmap(bitmap.bmWidth,bitmap.bmHeight,1,bitmap.bmBitsPixel,NULL);  
	bmp4.CreateBitmap(rc4.right,rc4.bottom,1,bitmap.bmBitsPixel,NULL);  
	bmpd.CreateBitmap(bitmap.bmWidth,bitmap.bmHeight,1,bitmap.bmBitsPixel,NULL);  
	
	dc1.CreateCompatibleDC(&rdc);
	dc2.CreateCompatibleDC(&rdc);
	dc3.CreateCompatibleDC(&rdc);
	dc4.CreateCompatibleDC(&rdc);
	
	dc1.SelectObject(&bmp1);
	dc2.SelectObject(&bmp2); 
	dc3.SelectObject(&bmp3); 
	dc4.SelectObject(&bmp4); 
    CRect rect;
	GetWindowRect(&rect);
	ScreenToClient(&rect);
	if(m_bBKStyle==bs_RGB)
	{
		int r1,g1,b1;
		for(int i=0;i<=rc4.Height();i++)
		{	
			r1=GetRValue(m_clrBack);
			g1=GetGValue(m_clrBack);
			b1=GetBValue(m_clrBack);
			r1=(r1+3*abs(rc4.top+i-rect.Height()/2-rect.top))>255?255:(r1+3*abs(rc4.top+i-rect.Height()/2-rect.top));
			g1=(g1+3*abs(rc4.top+i-rect.Height()/2-rect.top))>255?255:(g1+3*abs(rc4.top+i-rect.Height()/2-rect.top));
			b1=(b1+3*abs(rc4.top+i-rect.Height()/2-rect.top))>255?255:(b1+3*abs(rc4.top+i-rect.Height()/2-rect.top));
			CPen pen(PS_SOLID, 1, RGB(r1, g1, b1));
			CPen *old = dc4.SelectObject(&pen);
			dc4.MoveTo(rc4.left,rc4.top+i);
			dc4.LineTo(rc4.right,rc4.top+i);
			dc4.SelectObject(old);
		}
	}
	else
		dc4.FillRect(&rc4,&m_bkbrush); 
	maskcolor=dc1.GetPixel(0,0);
	
    CRect rc2;
	rc2.left=rc2.top=0;
	rc2.right=bitmap.bmWidth;
	rc2.bottom=bitmap.bmHeight;
	
	
///////////////////////////////////////////////////////////////////
	//dc2.FillRect(&rc2,&m_bkbrush);
    int totalbuttons=m_nButtonCount+m_nSeparatorCount;
	int buttonindex=0;
	int btnindex=0;
	
	for(buttonindex=0,btnindex=0;buttonindex<totalbuttons;buttonindex++)
	{
	    if(0==m_pbtButtonStyle[buttonindex])
		{
			if (m_dwStyle & CBRS_ORIENT_HORZ)
				dc2.BitBlt(btnindex*m_nButtonWidth,0,m_nButtonWidth,m_nButtonHeight,&dc4,btnindex*m_ButtonSize.cx+6*(buttonindex-btnindex)+3,3,SRCCOPY);
			else if(m_dwStyle & CBRS_ORIENT_VERT)
				dc2.BitBlt(btnindex*m_nButtonWidth,0,m_nButtonWidth,m_nButtonHeight,&dc4,3,btnindex*m_ButtonSize.cy+6*(buttonindex-btnindex)+3,SRCCOPY);
				//dc2.BitBlt(0,btnindex*m_nButtonHeight,m_nButtonWidth,m_nButtonHeight,&dc4,3,3,SRCCOPY);

			btnindex++;
		}
	}
	
	TransparentBltEx(dc2.m_hDC,0,0,bitmap.bmWidth,bitmap.bmHeight,
		dc1.m_hDC,0,0,bitmap.bmWidth,bitmap.bmHeight,maskcolor); 	
	LPBYTE lpbts;
	WORD ncount=0;
	ncount=(WORD)(bitmap.bmHeight*bitmap.bmWidthBytes);  
	lpbts=new BYTE[ncount];
	bmp2.GetBitmapBits(ncount,lpbts); 
	bmpd.SetBitmapBits(ncount,lpbts);
	
	m_imglstNormal.DeleteImageList(); 
	m_imglstNormal.Create(m_nButtonWidth, m_nButtonHeight, bitmap.bmBitsPixel|ILC_MASK, 1, 1);
	m_imglstNormal.Add(&bmpd, maskcolor);
	GetToolBarCtrl().SetImageList(&m_imglstNormal);
	/////////////////////////////////////////////////////////////////////
	//dc3.FillRect(&rc2,&m_bkbrush);
	dc3.BitBlt(0,0,bitmap.bmWidth,bitmap.bmHeight,&dc1,0,0,SRCCOPY); 
	
	for(int l1=0;l1<bitmap.bmWidth;l1++)
	{
		for(int c1=0;c1<bitmap.bmHeight;c1++) 
		{
			pixelcolor=dc3.GetPixel(l1,c1);
			//gray=(BYTE)(GetRValue(pixelcolor)*0.2+GetGValue(pixelcolor)*0.4+GetBValue(pixelcolor)*0.2);
			if(pixelcolor!=maskcolor)
				dc3.SetPixel(l1,c1,RGB(250,250,250)); 
		}
	}	
	for(buttonindex=0,btnindex=0;buttonindex<totalbuttons;buttonindex++)
	{
	    if(0==m_pbtButtonStyle[buttonindex])
		{
			if (m_dwStyle & CBRS_ORIENT_HORZ)
				dc2.BitBlt(btnindex*m_nButtonWidth,0,m_nButtonWidth,m_nButtonHeight,&dc4,btnindex*m_ButtonSize.cx+6*(buttonindex-btnindex)+3,3,SRCCOPY);
			else if(m_dwStyle & CBRS_ORIENT_VERT)
				dc2.BitBlt(btnindex*m_nButtonWidth,0,m_nButtonWidth,m_nButtonHeight,&dc4,3,btnindex*m_ButtonSize.cy+6*(buttonindex-btnindex)+3,SRCCOPY);
				//dc2.BitBlt(0,btnindex*m_nButtonHeight,m_nButtonWidth,m_nButtonHeight,&dc4,3,3,SRCCOPY);

			btnindex++;
		}
	}
	TransparentBltEx(dc2.m_hDC,1,1,bitmap.bmWidth,bitmap.bmHeight,
		dc3.m_hDC,0,0,bitmap.bmWidth,bitmap.bmHeight,maskcolor); 
	
	dc3.BitBlt(0,0,bitmap.bmWidth,bitmap.bmHeight,&dc1,0,0,SRCCOPY); 
	
	for(int l=0;l<bitmap.bmWidth;l++)
	{
		for(int c=0;c<bitmap.bmHeight;c++) 
		{
			pixelcolor=dc3.GetPixel(l,c);
			gray=(BYTE)(GetRValue(pixelcolor)*0.2+GetGValue(pixelcolor)*0.4+GetBValue(pixelcolor)*0.2);
			if(pixelcolor!=maskcolor)
				dc3.SetPixel(l,c,RGB(gray,gray,gray)); 
		}
	}	
	TransparentBltEx(dc2.m_hDC,0,0,bitmap.bmWidth,bitmap.bmHeight,
		dc3.m_hDC,0,0,bitmap.bmWidth,bitmap.bmHeight,maskcolor); 
	
	bmp2.GetBitmapBits(ncount,lpbts); 
	bmpd.SetBitmapBits(ncount,lpbts);
	
	m_imglstDisabled.DeleteImageList(); 
	m_imglstDisabled.Create(m_nButtonWidth, m_nButtonHeight, bitmap.bmBitsPixel|ILC_MASK, 1, 1);
	m_imglstDisabled.Add(&bmpd, maskcolor);
	GetToolBarCtrl().SetDisabledImageList(&m_imglstDisabled);
	
	
	/////////////////////////////////////////////////////////////////////
	dc3.BitBlt(0,0,bitmap.bmWidth,bitmap.bmHeight,&dc1,0,0,SRCCOPY); 
	for(int l3=0;l3<bitmap.bmWidth;l3++)
	{
		for(int c3=0;c3<bitmap.bmHeight;c3++) 
		{
			pixelcolor=dc3.GetPixel(l3,c3);
			if(pixelcolor!=maskcolor)
				dc3.SetPixel(l3,c3,\
				RGB(\
				GetRValue(pixelcolor)>240?255:GetRValue(pixelcolor)+15,\
				GetGValue(pixelcolor)>200?255:GetGValue(pixelcolor)+55,\
				GetBValue(pixelcolor)>240?255:GetBValue(pixelcolor)+15
				)); 
		}	
	}	
				
	for(buttonindex=0,btnindex=0;buttonindex<totalbuttons;buttonindex++)
	{
	    if(0==m_pbtButtonStyle[buttonindex])
		{
			if (m_dwStyle & CBRS_ORIENT_HORZ)
				dc2.BitBlt(btnindex*m_nButtonWidth,0,m_nButtonWidth,m_nButtonHeight,&dc4,btnindex*m_ButtonSize.cx+6*(buttonindex-btnindex)+3,3,SRCCOPY);
			else if(m_dwStyle & CBRS_ORIENT_VERT)
				dc2.BitBlt(btnindex*m_nButtonWidth,0,m_nButtonWidth,m_nButtonHeight,&dc4,3,btnindex*m_ButtonSize.cy+6*(buttonindex-btnindex)+3,SRCCOPY);
				//dc2.BitBlt(0,btnindex*m_nButtonHeight,m_nButtonWidth,m_nButtonHeight,&dc4,3,3,SRCCOPY);

			btnindex++;
		}
	}
	TransparentBltEx(dc2.m_hDC,0,0,bitmap.bmWidth,bitmap.bmHeight,\
	dc3.m_hDC,0,0,bitmap.bmWidth,bitmap.bmHeight,maskcolor); 
				
				
	bmp2.GetBitmapBits(ncount,lpbts); 
	bmpd.SetBitmapBits(ncount,lpbts);
				
	m_imglstHot.DeleteImageList(); 
	m_imglstHot.Create(m_nButtonWidth, m_nButtonHeight, bitmap.bmBitsPixel|ILC_MASK, 1, 1);
	m_imglstHot.Add(&bmpd, maskcolor);
	GetToolBarCtrl().SetHotImageList(&m_imglstHot);
	CRect rt;
	GetToolBarCtrl().GetClientRect(&rt); 
	TBBUTTONINFO btinfo;
	int wx=0;
	for(UINT k=0;k<m_nButtonCount;k++)
	{
		GetToolBarCtrl().GetButtonInfo(k,&btinfo);
		wx+=btinfo.cx;
	}
	delete[] lpbts;
	return TRUE;
}

void CMyToolBar::MouseHover(POINT pt)
{
	if (m_dwStyle & CBRS_FLOATING) {
		// no grippers
	} else if (m_dwStyle & CBRS_ORIENT_HORZ) {
		// gripper at left
		CRect rc;
		GetWindowRect(&rc);
		rc.right=rc.left+8;
		if(rc.PtInRect(pt))
			::SetCursor(::LoadCursor(NULL,IDC_SIZEALL)); 
	} else {
		// gripper at top
		CRect rc;
		GetWindowRect(&rc);
		rc.bottom=rc.top+8;
		if(rc.PtInRect(pt))
			::SetCursor(::LoadCursor(NULL,IDC_SIZEALL)); 
	}

}
BOOL CMyToolBar::PreTranslateMessage(MSG* pMsg)
{
	switch(pMsg->message)
	{
	case WM_MOUSEMOVE:
		MouseHover(pMsg->pt);	
        break;
	}
	return CToolBar::PreTranslateMessage(pMsg); 
}

void CMyToolBar::OnBarStyleChange(DWORD dwOldStyle, DWORD dwNewStyle)
{
	CToolBar::OnBarStyleChange(dwOldStyle,dwNewStyle);
	if(( (dwOldStyle & CBRS_ORIENT_HORZ)&& (dwOldStyle & CBRS_ORIENT_VERT))||\
		( (dwOldStyle & CBRS_ORIENT_VERT)&& (dwOldStyle & CBRS_ORIENT_HORZ)))
	{
		LoadBitmap(m_lpszResourceName);
		Invalidate();
	}
}
void CMyToolBar::SetShowText(BOOL blnShow)
{		
	CSize szButton;
	int i;
	if(blnShow)
	{
		for(i = 0; i <GetCount(); i++)		//为每个按钮增加文本
		{	
			CString strButton;
			UINT id =GetItemID(i);
			if(!strButton.LoadString(id)) continue;
			int j = strButton.Find(_T('\n'));
			if(j < 0) continue;
			strButton = strButton.Right(strButton.GetLength() - j - 1);
			SetButtonText(i,strButton); 
		}
		szButton.cx=m_ButtonSize.cx+15;
		szButton.cy=m_ButtonSize.cy+15;
	}
	else
	{
		szButton.cx=m_ButtonSize.cx;
		szButton.cy=m_ButtonSize.cy;
	}
	SetSizes(szButton,m_ButtonImageSize);
}
void CMyToolBar::SetSizes(SIZE sizeButton)
{
	SetSizes(sizeButton,m_ButtonImageSize);
}
void CMyToolBar::SetSizes(SIZE sizeButton, SIZE sizeImage )
{
	CToolBar::SetSizes(sizeButton,sizeImage);	
}