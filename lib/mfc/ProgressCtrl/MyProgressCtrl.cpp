// MyProgressCtrl.cpp : implementation file

#include "stdafx.h"
#include "MyProgressCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMyProgressCtrl

CMyProgressCtrl::CMyProgressCtrl()
{
    m_nCurrentPos  = 0;
    m_nStepSize  = 1;
    m_nMax  = 100;
    m_nMin  = 0;
    m_bShowText  = TRUE;
    m_strText.Empty();
	m_nBarStyle = MYBAR_STYLE_XP;//MYBAR_STYLE_NORMAL;
    m_clrDlgBk  = RGB(255,255,255);
    m_clrTextUnCover = ::GetSysColor(COLOR_HIGHLIGHT);
    m_clrTextCovered = ::GetSysColor(COLOR_HIGHLIGHT);
	m_clrStartBar=RGB(42,214,42);
    m_clrEndBar=RGB(42,214,42);	
}

CMyProgressCtrl::~CMyProgressCtrl()
{
}

BEGIN_MESSAGE_MAP(CMyProgressCtrl, CProgressCtrl)
    //{{AFX_MSG_MAP(CTextProgressCtrl)
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
    ON_MESSAGE(WM_SETTEXT, OnSetText)
    ON_MESSAGE(WM_GETTEXT, OnGetText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextProgressCtrl message handlers

LRESULT CMyProgressCtrl::OnSetText(UINT, LPCTSTR szText)
{
    LRESULT result = Default();

    if ( (!szText && m_strText.GetLength()) ||
         (szText && (m_strText != szText))   )
    {
        m_strText = szText;
        Invalidate();
    }
    return result;
}

LRESULT CMyProgressCtrl::OnGetText(UINT cchTextMax, LPTSTR szText)
{
    if (!_tcsncpy(szText, m_strText, cchTextMax))
        return 0;
    else 
        return min(cchTextMax, (UINT) m_strText.GetLength());
}

BOOL CMyProgressCtrl::OnEraseBkgnd(CDC* /*pDC*/) 
{    
     return FALSE;
}

void CMyProgressCtrl::OnSize(UINT nType, int cx, int cy) 
{
    CProgressCtrl::OnSize(nType, cx, cy);    
}

void CMyProgressCtrl::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
//	ModifyStyle(0, PBS_SMOOTH);
	ModifyStyleEx(WS_EX_STATICEDGE,0);
	
	//设置按钮的有效区域,消除残角
	CRgn rgn;
	RECT rect;
	GetClientRect(&rect);
	
	//有效区域是一个角半径为5的圆角矩形
	rgn.CreateRoundRectRgn(rect.left,rect.top,rect.right+3,rect.bottom+3,5,5);
	SetWindowRgn(rgn,TRUE);
	rgn.DeleteObject();
	CProgressCtrl::PreSubclassWindow();
}
void CMyProgressCtrl::DrawText(CDC* pDC, const RECT& rectClient, const int& nMaxWidth)
{
	//显示文字
	if (m_bShowText)
	{
		CRect ClientRect=rectClient;
		RECT LeftRect,RightRect;
		LeftRect=RightRect=rectClient;
		LeftRect.right = nMaxWidth;
		RightRect.left = nMaxWidth;
		CString str;
		if (m_strText.GetLength())
			str = m_strText;
		else if (m_bShowPercent)
		{
			str.Format("%.2f%%", (float)nMaxWidth/rectClient.right*100.0);
		}
		else return;
		
		int model=pDC->SetBkMode(TRANSPARENT);
		
		HGDIOBJ hOldFont = ::SelectObject(pDC->m_hDC, ::GetStockObject(DEFAULT_GUI_FONT));//使用系统默认字体
		
		CRgn rgn;
		rgn.CreateRectRgn(LeftRect.left, LeftRect.top, LeftRect.right, LeftRect.bottom);
		pDC->SelectClipRgn(&rgn);
		pDC->SetTextColor(m_clrTextCovered);
		
		pDC->DrawText(str, &ClientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		
		rgn.DeleteObject();
		rgn.CreateRectRgn(RightRect.left, RightRect.top, RightRect.right, RightRect.bottom);
		pDC->SelectClipRgn(&rgn);
		pDC->SetTextColor(m_clrTextUnCover);
		pDC->DrawText(str, &ClientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		pDC->SetBkMode(model);
		::SelectObject(pDC->m_hDC, hOldFont);
    }
}

void CMyProgressCtrl::DrawNormalStyleBar(CDC* pDC, const RECT& rectClient, const int& nMaxWidth)  //绘制进度条
{
	RECT ClientRect;
    ClientRect=rectClient;
		     
	RECT rectFill;            //显示区域
    int r, g, b;
    //得到不同颜色差值
    r = (GetRValue(m_clrEndBar) - GetRValue(m_clrStartBar));
    g = (GetGValue(m_clrEndBar) - GetGValue(m_clrStartBar));
    b = (GetBValue(m_clrEndBar) - GetBValue(m_clrStartBar));
	
	float rStep, gStep, bStep;	
	rStep = (float) r/(float) rectClient.right;
	gStep = (float) g/(float) rectClient.right;
	bStep = (float) b/(float) rectClient.right;
	
	r = GetRValue(m_clrStartBar);
	g = GetGValue(m_clrStartBar);
	b = GetBValue(m_clrStartBar);
	

	int nBarStyle=GetStyle() & PBS_SMOOTH; //是否平滑进度条
	if (nBarStyle)  //绘制平滑进度条
	{
		for (int step=1;step<=nMaxWidth;step=step++)
		{
			::SetRect(&rectFill,step-1,0,step,rectClient.bottom);
			pDC->FillSolidRect(&rectFill, RGB(r+step*rStep,g+step*gStep,b+step*bStep));
		}
		::SetRect(&rectFill, nMaxWidth, 0, rectClient.right,rectClient.bottom);
	    pDC->FillSolidRect(&rectFill, m_clrDlgBk);
	}
	else  
	{
		int i=1;
		for (int step=1;step<=nMaxWidth;step=step++)
		{
			if (i%7)
			{
				::SetRect(&rectFill,step-1,0,step,rectClient.bottom);
				pDC->FillSolidRect(&rectFill, RGB(r+step*rStep,g+step*gStep,b+step*bStep));
			}
			else
			{
				::SetRect(&rectFill,step-1,0,step,rectClient.bottom);
			    pDC->FillSolidRect(&rectFill, m_clrDlgBk);
			}
			i++;
		}
		::SetRect(&rectFill, nMaxWidth, 0, rectClient.right,rectClient.bottom);
	    pDC->FillSolidRect(&rectFill, m_clrDlgBk);
	}
}

void CMyProgressCtrl::DrawXPStyleBar(CDC* pDC, const RECT& rectClient, const int& nMaxWidth)      //绘制xp风格进度条
{
	DrawXPBackgroup(pDC,rectClient);
	DrawXPStep(pDC,rectClient,(int)nMaxWidth);
}
void CMyProgressCtrl::DrawWin7StyleBar(CDC* pDC, const RECT& rectClient, const int& nMaxWidth)      //绘制win7风格进度条
{

}

void CMyProgressCtrl::OnPaint() 
{
    if (m_nMin >= m_nMax) 
        return;

    RECT ClientRect;
    GetClientRect(&ClientRect);

    CPaintDC PaintDC(this); // device context for painting
    CMemDC memDC(&PaintDC);
    float maxWidth=(float) (m_nCurrentPos -m_nMin) /(float) (m_nMax-m_nMin)*(float) ClientRect.right; //进度条显示位置
	if (MYBAR_STYLE_NORMAL == m_nBarStyle)
	{  
		DrawNormalStyleBar(&memDC,ClientRect,(int)maxWidth);  
	}
	else if (MYBAR_STYLE_XP == m_nBarStyle)
	{
        DrawXPStyleBar(&memDC,ClientRect,(int)maxWidth);
	}
	else /*if (MYBAR_STYLE_WIN7 == m_nBarStyle)*/
	{

	}
	DrawText(&memDC,ClientRect,(int)maxWidth);  //输出文字
}

void CMyProgressCtrl::SetStartBarColor(COLORREF col)
{
    m_clrStartBar = col;
}
void CMyProgressCtrl::SetEndBarColor(COLORREF col)  
{
    m_clrEndBar = col;
}

void CMyProgressCtrl::SetDlgBkColor(COLORREF col)
{
    m_clrDlgBk = col;
}

void CMyProgressCtrl::SetTextUnCoverColor(COLORREF col)
{
    m_clrTextUnCover = col;
}

void CMyProgressCtrl::SetTextCoveredColor(COLORREF col)
{
    m_clrTextCovered = col;
}

COLORREF CMyProgressCtrl::GetStartBarColor()
{
    return m_clrStartBar;
}
COLORREF CMyProgressCtrl::GetEndBarColor()
{
    return m_clrEndBar;
}

COLORREF CMyProgressCtrl::GetBkColor()
{
    return m_clrDlgBk;
}

COLORREF CMyProgressCtrl::GetTextUnCoverColor()
{
    return m_clrTextUnCover;
}

COLORREF CMyProgressCtrl::GetTextCoveredColor()
{
    return m_clrTextCovered;
}
/////////////////////////////////////////////////////////////////////////////
// CTextProgressCtrl message handlers

void CMyProgressCtrl::EnableShowText(BOOL bShowText /* = TRUE */,BOOL bShowPercent /* = TRUE */ )
{ 
    if (::IsWindow(m_hWnd) && m_bShowText != bShowText)
        Invalidate();

    m_bShowText = bShowText;
	m_bShowPercent = bShowPercent;
}


void CMyProgressCtrl::SetRange(int nLower, int nUpper)
{
	if (nLower>=0 && nLower<nUpper)
	{
		m_nMax = nUpper; 
		m_nMin = nLower;
		CMyProgressCtrl::SetPos(nLower);
	}
}

int CMyProgressCtrl::SetPos(int nPos) 
{    
/*  if (!::IsWindow(m_hWnd))
        return -1;

    int nOldPos = m_nCurrentPos;
    m_nCurrentPos = nPos;

    CRect rect;
    GetClientRect(rect);

    double Fraction = (double)(m_nCurrentPos - m_nMin) / ((double)(m_nMax - m_nMin));
    int nBarWidth = (int) (Fraction * rect.Width());

    if (nBarWidth != m_nBarWidth)
    {
        m_nBarWidth = nBarWidth;
        RedrawWindow();
    }
    return nOldPos;*/
	if (!::IsWindow(m_hWnd))
	        return -1;
	int nTempPos = m_nCurrentPos;
	if (nPos>=m_nMin && nPos<=m_nMax && nPos!=m_nCurrentPos)
	{
		m_nCurrentPos=nPos;
		Invalidate(TRUE);
	}    
    return	nTempPos;
}
int  CMyProgressCtrl::SetBarStyle(int nbarStyle /*= MYBAR_STYLE_NORMAL*/)
{

	int nTempStyle = m_nBarStyle;
	if (m_nBarStyle != nbarStyle)
	{
		m_nBarStyle=nbarStyle;
		Invalidate(TRUE);
	}
	return nTempStyle;
}
int  CMyProgressCtrl::GetPos()
{
	return m_nCurrentPos;
}

void  CMyProgressCtrl::GetRange(int& nLower, int& nUpper)
{
	nLower=m_nMin;
	nUpper=m_nMax;
}

int CMyProgressCtrl::StepIt() 
{    
    return SetPos(m_nCurrentPos + m_nStepSize);
}

int CMyProgressCtrl::OffsetPos(int nPos)
{
    return SetPos(m_nCurrentPos + nPos);
}

int CMyProgressCtrl::SetStep(int nStep)
{
    int nOldStep = nStep;
	if (nStep>=0)
	{ 
		m_nStepSize = nStep;
	}
    return nOldStep;
} 

void CMyProgressCtrl::DrawXPBackgroup(CDC* pDC, const RECT& rectClient)
{	
	//最外层边框
	CPen hPen;
	hPen.CreatePen(PS_SOLID, 1, RGB(104, 104, 104));
	CPen* pOldPen = (CPen*)pDC->SelectObject(&hPen);
	pDC->RoundRect(&rectClient,CPoint(5,5));
	VERIFY(hPen.DeleteObject());

	//画第二层
	hPen.CreatePen(PS_SOLID, 1, RGB(190, 190, 190));
    pDC->SelectObject(hPen);
	pDC->MoveTo(rectClient.left, rectClient.top+1);
	pDC->LineTo(rectClient.right, rectClient.top+1);
	pDC->MoveTo(rectClient.left+1, rectClient.top);
	pDC->LineTo(rectClient.left+1, rectClient.bottom);

//  	pDC->MoveTo(rectClient.left, rectClient.bottom-1);
//  	pDC->LineTo(rectClient.right, rectClient.bottom-1);
//  	pDC->MoveTo(rectClient.right-1, rectClient.top);
//  	pDC->LineTo(rectClient.right-1, rectClient.bottom);
	VERIFY(hPen.DeleteObject());

	//画第三层
	hPen.CreatePen(PS_SOLID, 1, RGB(239, 239, 239));
	pDC->SelectObject(&hPen);
	pDC->Rectangle(rectClient.left+2, rectClient.top+2, rectClient.right-2, rectClient.bottom-2);	
	VERIFY(hPen.DeleteObject());

	//填充关键点
	pDC->SetPixel(CPoint(rectClient.left, rectClient.top), RGB(255, 255, 255));
	pDC->SetPixel(CPoint(rectClient.right-1, rectClient.top), RGB(255, 255, 255));
	pDC->SetPixel(CPoint(rectClient.left, rectClient.bottom-1), RGB(255, 255, 255));
	pDC->SetPixel(CPoint(rectClient.right-1, rectClient.bottom-1), RGB(255, 255, 255));

	pDC->SetPixel(CPoint(rectClient.left+1, rectClient.top+1), RGB(104, 104, 104));
	pDC->SetPixel(CPoint(rectClient.left, rectClient.bottom-2), RGB(190, 190, 190));
	pDC->SetPixel(CPoint(rectClient.left+1, rectClient.bottom-2), RGB(104, 104, 104));
	pDC->SetPixel(CPoint(rectClient.right-2, rectClient.top+1), RGB(104, 104, 104));
	pDC->SetPixel(CPoint(rectClient.right-2, rectClient.top+2), RGB(190, 190, 190));

	pDC->SetPixel(CPoint(rectClient.left+2, rectClient.top+2), RGB(190, 190, 190));
	pDC->SetPixel(CPoint(rectClient.right-2, rectClient.bottom-2), RGB(104, 104, 104));

	//恢复
	pDC->SelectObject(pOldPen);//Restore original brush
}

void CMyProgressCtrl::DrawXPStep(CDC* pDC, const RECT& rectClient, const int& nMaxWidth)
{
	if(nMaxWidth == 0)
		return;
	CRect rc(rectClient.left+3, rectClient.top+2, rectClient.right-2, rectClient.bottom-2);//整个色块的区域
	int nStep=8;
	if (GetStyle() & PBS_SMOOTH)
	{
		nStep=6;
	}
	CRect StepRect(rc.left, rc.top, rc.left+6, rc.bottom);//第一色块区域
	int count = nMaxWidth/nStep+1;                       //需要绘制的竖条数目
	if(nMaxWidth>0 && count==0)                          //1%的时候出现一条色块
		count = 1;

	int r, g, b;
    //得到不同颜色差值
    r = (GetRValue(m_clrEndBar) - GetRValue(m_clrStartBar));
    g = (GetGValue(m_clrEndBar) - GetGValue(m_clrStartBar));
    b = (GetBValue(m_clrEndBar) - GetBValue(m_clrStartBar));
	
	float rStep, gStep, bStep;	
	rStep = (float) r/(float) rectClient.right;
	gStep = (float) g/(float) rectClient.right;
	bStep = (float) b/(float) rectClient.right;
	
	r = GetRValue(m_clrStartBar);
	g = GetGValue(m_clrStartBar);
	b = GetBValue(m_clrStartBar);

	for(int i=0; i<count; i++)
	{
		FillXPStepRect(pDC, StepRect,RGB(r+i*nStep*rStep,g+i*nStep*gStep,b+i*nStep*bStep),rStep, gStep, bStep);                   //绘制色块

		StepRect.SetRect(StepRect.left+nStep, StepRect.top, StepRect.right+nStep, StepRect.bottom);//下一个色块区域
		if(StepRect.left > rc.right)
			StepRect.left = rc.right;
		if(StepRect.right > rc.right)
			StepRect.right = rc.right;
	}	
}

void CMyProgressCtrl::FillXPStepRect( CDC* pDC, const RECT &rc,COLORREF clrStepStart,float rStep,float gStep,float bStep )
{
// 	pDC->FillSolidRect(CRect(rc.left, rc.top, rc.right, rc.top+1), RGB(171, 237, 172));
// 	pDC->FillSolidRect(CRect(rc.left, rc.top+1, rc.right, rc.top+2), RGB(149, 233, 150));
// 	pDC->FillSolidRect(CRect(rc.left, rc.top+2, rc.right, rc.top+3), RGB(102, 223, 104));
// 	pDC->FillSolidRect(CRect(rc.left, rc.top+3, rc.right, rc.top+4), RGB(78, 218, 80));
// 	pDC->FillSolidRect(CRect(rc.left, rc.top+4, rc.right, rc.top+5), RGB(53, 213, 56));
// 	pDC->FillSolidRect(CRect(rc.left, rc.top+5, rc.right, rc.bottom-5), RGB(40, 210, 43));//中间部分
// 	pDC->FillSolidRect(CRect(rc.left, rc.bottom-1, rc.right, rc.bottom), RGB(171, 237, 172));
// 	pDC->FillSolidRect(CRect(rc.left, rc.bottom-2, rc.right, rc.bottom-1), RGB(149, 233, 150));
// 	pDC->FillSolidRect(CRect(rc.left, rc.bottom-3, rc.right, rc.bottom-2), RGB(102, 223, 104));
// 	pDC->FillSolidRect(CRect(rc.left, rc.bottom-4, rc.right, rc.bottom-3), RGB(78, 218, 80));
// 	pDC->FillSolidRect(CRect(rc.left, rc.bottom-5, rc.right, rc.bottom-4), RGB(53, 213, 56));

// 	int nHeight=(rc.bottom-rc.top)/4;
// 	int rValue=GetRValue(m_clrStartBar);
// 	int gValue=GetGValue(m_clrStartBar);
// 	int bValue=GetBValue(m_clrStartBar);
// 
// 	int rStep=(255-rValue)/nHeight;
// 	int gStep=(255-gValue)/nHeight;
//     int bStep=(255-bValue)/nHeight;
// 
// 	for (int i=1;i<=nHeight;i++)
// 	{
// 		pDC->FillSolidRect(CRect(rc.left, rc.top+nHeight-i, rc.right, rc.top+nHeight-i+1), 
// 			RGB(rValue+rStep*i,gValue+gStep*i,bValue+bStep*i));
// 		pDC->FillSolidRect(CRect(rc.left, rc.bottom-(nHeight-i), rc.right, rc.bottom-(nHeight-i+1)), 
// 			RGB(rValue+rStep*i,gValue+gStep*i,bValue+bStep*i));
// 	}
// 	
// 	pDC->FillSolidRect(CRect(rc.left, rc.top+nHeight, rc.right, rc.bottom-nHeight), 
// 			m_clrStartBar);

	int nHeight=(rc.bottom-rc.top)/3;
	for (int j=0;j<rc.right-rc.left;j++)
	{
		int rValue=GetRValue(clrStepStart)+j*rStep;
		int gValue=GetGValue(clrStepStart)+j*gStep;
		int bValue=GetBValue(clrStepStart)+j*bStep;
		
		int rStep=(255-rValue)/nHeight;
		int gStep=(255-gValue)/nHeight;
        int bStep=(255-bValue)/nHeight;
		for (int i=1;i<=nHeight;i++)
		{
			pDC->FillSolidRect(CRect(rc.left, rc.top+nHeight-i, rc.left+j+1, rc.top+nHeight-i+1), 
				RGB(rValue+rStep*i,gValue+gStep*i,bValue+bStep*i));
			pDC->FillSolidRect(CRect(rc.left, rc.bottom-(nHeight-i), rc.left+j+1, rc.bottom-(nHeight-i+1)), 
				RGB(rValue+rStep*i,gValue+gStep*i,bValue+bStep*i));	
			pDC->FillSolidRect(CRect(rc.left, rc.top+nHeight, rc.left+j+1, rc.bottom-nHeight), 
				RGB(rValue,gValue,bValue));
		}
	}





    
}