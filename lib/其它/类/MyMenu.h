//完成对菜单的重绘
//背景的掩码为RGB(192,192,192)
//------------------------------------------------设计人:牛文平
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_MYCOOLMENU_H__061B4615_4569_4278_9F86_6DFF6FCEA45F__INCLUDED_)
#define AFX_MYCOOLMENU_H__061B4615_4569_4278_9F86_6DFF6FCEA45F__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#ifndef Public_Area
#define Public_Area 20 //最左边灰色区域宽度
#endif
#define BACK_COLOR 1
#define BACK_PICTURE 2
class CMenuItemContext//保存菜单的所有有关信息
{
public:
	int     nMenuID; 
	//-2:顶层菜单条 //-1:弹出菜单
	//0:分隔条      //>0:一般的菜单
	UINT    U_icon;
	CString strText;//menu text 
	
};
class CMyMenu : public CMenu  
{
public:
	//存储ID数组
	CUIntArray m_menuIDs;
	//存储图标数组
	CImageList   m_ImageList;
	//顶端颜色
	COLORREF m_clrTop;
	//顶端图片
	BITMAP m_btmTop;
	//顶端的设置
	//BACK_COLOR 1
	//BACK_PICTURE 2
	int    m_topStyle;
	//顶端的大小
	CRect rcMain;
	//效果色1
	COLORREF m_clrSelectLeft;
	//效果色2
	COLORREF m_clrSelectRight;
	//文本色
	COLORREF m_clrText;
	//顶端背景画刷
	CBrush m_backbrush;
	CMyMenu();
	virtual ~CMyMenu();
	int			GetImageFromToolBar(UINT uToolBarID, CSize sz, CImageList *pImageList,  CUIntArray *uIDArray);
	int			GetIconIndexByID(UINT uID);
	BOOL		ChangeMenuStyle(HMENU hMenu, BOOL bTop);
	BOOL		AttachMenu(HMENU hMenu,UINT uToolBarID,CSize sz);
	UINT		AddToolBar(UINT uToolBarID, CSize sz);
	void		DrawBestRect(CDC *pDC, CRect rect, COLORREF cr1,COLORREF cr2, BOOL bHor);
	void		DrawTop(CDC *pDC,CRect rect,BOOL bSelected);
	void		DrawBGColor(CDC* pDC, CRect rect, BOOL bSelected);
	void		DrawIcon(CDC* pDC, CRect rect, UINT uIndex, BOOL IsEnabled);
	void		DrawText(CDC* pDC, CRect rect, CString sText);
	void		DrawGray(CDC* pDC, CRect rect, CString sText);
	void		DrawTopBack(CDC* pDC,CRect rcSourse);
	void		DrawTopBack(CDC* pDC);
	void        SetTopBack(COLORREF clr);
	virtual		void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual		void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);
	virtual     void LoadMenu(UINT nIDResource);
	void		SetTopBackImage(UINT nIDResource);
};
#endif // !defined(AFX_MYCOOLMENU_H__061B4615_4569_4278_9F86_6DFF6FCEA45F__INCLUDED_)
