/////////////////////////////////////////////////////////////////////////////
//CPluckButton:推拉窗口按钮
//
//在Dreamweaver、Flash等软件中有一种推拉窗口的按钮，点击后会使窗口尺寸发生
//变化，本按钮就是仿造它的形状做出来的。
//
//用法：
//①在对话框中放置一个按钮控件，清除其文本，把形状调整为细条状；
//②为按钮定义变量，类型改为CPluckButton;
//③在对话框的OnInitDialog()函数中用SetPluckButStyle()函数设置按钮初始风格。
//
//作者：风林
//                   2004-12-18
////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLUCKBUTTON_H__397B334A_9686_4EF6_9070_3F6591BCB24F__INCLUDED_)
#define AFX_PLUCKBUTTON_H__397B334A_9686_4EF6_9070_3F6591BCB24F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PluckButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPluckButton window

//按钮风格（按箭头方向划分）
#define  KEY_STYLE_DOWN  1
#define  KEY_STYLE_UP    2
#define  KEY_STYLE_LEFT  3
#define  KEY_STYLE_RIGHT 4

class CPluckButton : public CButton
{
// Construction
public:
	CPluckButton();

private:
	int    m_style;			//按钮风格
	BOOL   b_InRect;		//鼠标进入标志
	CRect  m_ButRect;		//按钮尺寸
	POINT  m_pt[3];			//箭头坐标

	void NormalButton(CDC *pDC);	//画正常按钮
	void PassButton(CDC *pDC);		//画鼠标经过时的按钮

// Attributes
public:
	void SetPluckButStyle(int Style);	//设置按钮风格
	int  GetPluckButStyle();			//获取按钮风格

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPluckButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPluckButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPluckButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLUCKBUTTON_H__397B334A_9686_4EF6_9070_3F6591BCB24F__INCLUDED_)
