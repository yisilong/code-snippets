#if !defined(AFX_MYPROGRESSCTRL_H__4C78DBBE_EFB6_11D1_AB14_203E25000000__INCLUDED_)
#define AFX_MYPROGRESSCTRL_H__4C78DBBE_EFB6_11D1_AB14_203E25000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// MyProgressCtrl.h : header file
/////////////////////////////////////////////////////////////////////////////
// CMyProgressCtrl window
#define MYBAR_STYLE_NORMAL 0
#define MYBAR_STYLE_XP 1
#define MYBAR_STYLE_WIN7 2
class CMyProgressCtrl : public CProgressCtrl
{
// Construction
public:
    CMyProgressCtrl();

// Attributes
public:

// Operations
public:
    int         SetPos(int nPos);
	int         GetPos();
    int         StepIt();
	int         SetStep(int nStep);
    void        SetRange(int nLower, int nUpper);
	void        GetRange(int& nLower, int& nUpper);
    int         OffsetPos(int nPos);                  //以nPos指定的增量来增加进度条控件的当前位置
    void        SetStartBarColor(COLORREF col);           //设置进度条起始颜色
	void        SetEndBarColor(COLORREF col);           //设置进度条终止颜色
    void        SetDlgBkColor(COLORREF col);            //设置进度条的背景色
    void        SetTextUnCoverColor(COLORREF col);    //当进度条没有覆盖文本时，文本的颜色
    void        SetTextCoveredColor(COLORREF col);      //当进度条覆盖文本时，文本的颜色
	COLORREF    GetStartBarColor();                       //得到进度条的起始颜色
	COLORREF    GetEndBarColor();                       //得到进度条的终止颜色
	COLORREF    GetBkColor();                        //得到进度条的背景色
	COLORREF    GetTextUnCoverColor();                //当进度条没有覆盖文本时，得到文本的颜色
	COLORREF    GetTextCoveredColor();                  //当进度条覆盖文本时，得到文本的颜色	
    void        EnableShowText(BOOL bShowText = TRUE,BOOL bShowPercent = TRUE );       //设置是否显示文字和百分比(百分比仅当m_bShowText为TRUE,m_strText为空时显示)
	int         SetBarStyle(int nbarStyle = MYBAR_STYLE_NORMAL);            //设置进度条的风格
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMyProgressCtrl)
protected:
	virtual void PreSubclassWindow();
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CMyProgressCtrl();

// Generated message map functions
protected:
    int         m_nCurrentPos,                     //进度条当前位置
                m_nStepSize,                //步进增量尺度
                m_nMax,                     //进度条控件范围的上限
                m_nMin;                     //进度条控件范围的下限
    CString     m_strText;                  //进度条显示的文字
    BOOL        m_bShowText;                //是否显示文字
	BOOL        m_bShowPercent;             //是否显示百分比,m_bShowText为TRUE,m_strText为空时显示

    COLORREF    m_clrStartBar,               //进度条起始位置颜色
		        m_clrEndBar,                 //进度条终止位置颜色
                m_clrDlgBk,                    //进度条背景色
                m_clrTextUnCover,              //进度条没有覆盖文本时，文本的颜色
                m_clrTextCovered;              //进度条覆盖文本后，文本的颜色
	int         m_nBarStyle;                   //进度条的风格

    //{{AFX_MSG(CMyProgressCtrl)
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG
    afx_msg LRESULT OnSetText(UINT cchTextMax, LPCTSTR szText);
    afx_msg LRESULT OnGetText(UINT cchTextMax, LPTSTR szText);

    DECLARE_MESSAGE_MAP()
protected:
	void DrawNormalStyleBar(CDC* pDC, const RECT& rectClient, const int& nMaxWidth);  //绘制一般进度条
	void DrawXPStyleBar(CDC* pDC, const RECT& rectClient, const int& nMaxWidth);      //绘制xp风格进度条
	void DrawWin7StyleBar(CDC* pDC, const RECT& rectClient, const int& nMaxWidth);      //绘制win7风格进度条
	void DrawText(CDC* pDC, const RECT& rectClient, const int& nMaxWidth);                                                                       //绘制文字
	void DrawXPStep(CDC* pDC, const RECT& rectClient, const int& nMaxWidth);
	void DrawXPBackgroup(CDC* pDC, const RECT& rectClient);
	void FillXPStepRect(CDC* pDC, const RECT &rc,COLORREF clrStepStart,float rStep,float gStep,float bStep);
private:
	//////////////////////////////////////////////////
	// CMemDC - memory DC
	//
	// Author: Keith Rule
	// Email:  keithr@europa.com
	// Copyright 1996-1997, Keith Rule
	//
	// You may freely use or modify this code provided this
	// Copyright is included in all derived versions.
	//
	// History - 10/3/97 Fixed scrolling bug.
	//                   Added print support.
	//
	// This class implements a memory Device Context
#ifndef _MEMDC_H_
#define _MEMDC_H_	
	class CMemDC : public CDC
	{
	public:
		
		// constructor sets up the memory DC
		CMemDC(CDC* pDC) : CDC()
		{
			ASSERT(pDC != NULL);
			
			m_pDC = pDC;
			m_pOldBitmap = NULL;
			m_bMemDC = !pDC->IsPrinting();  //判断m_MemDC是否为兼容内存DC     
			if (m_bMemDC)    // Create a Memory DC
			{
				pDC->GetClipBox(&m_rect);
				CreateCompatibleDC(pDC);
				m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
				m_pOldBitmap = SelectObject(&m_bitmap);
				SetWindowOrg(m_rect.left, m_rect.top);
			}
			else        // Make a copy of the relevent parts of the current DC for printing
			{
				m_bPrinting = pDC->m_bPrinting;
				m_hDC       = pDC->m_hDC;
				m_hAttribDC = pDC->m_hAttribDC;
			}
		}
		// Destructor copies the contents of the mem DC to the original DC
		~CMemDC()
		{
			if (m_bMemDC) 
			{    
				// Copy the offscreen bitmap onto the screen.
				m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
					this, m_rect.left, m_rect.top, SRCCOPY);
				
				//Swap back the original bitmap.
				SelectObject(m_pOldBitmap);
			} 
			else 
			{
				// All we need to do is replace the DC with an illegal value,
				// this keeps us from accidently deleting the handles associated with
				// the CDC that was passed to the constructor.
				m_hDC = m_hAttribDC = NULL;
			}
		}
		
		// Allow usage as a pointer
		CMemDC* operator->() {return this;}
		// Allow usage as a pointer
		operator CMemDC*() {return this;}
		
	private:
		CBitmap  m_bitmap;      // Offscreen bitmap
		CBitmap* m_pOldBitmap;  // bitmap originally found in CMemDC
		CDC*     m_pDC;         // Saves CDC passed in constructor
		CRect    m_rect;        // Rectangle of drawing area.
		BOOL     m_bMemDC;      // TRUE if CDC really is a Memory DC.
	};	
#endif

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYPROGRESSCTRL_H__4C78DBBE_EFB6_11D1_AB14_203E25000000__INCLUDED_)

