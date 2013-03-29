//使用说明
//1:设置状态有三种:
//		A:正常即缺省状态不用对按钮做任何设置
//		B:设置为Disable和Notify为静态的文本
//		C:设置为Flat为隐藏形的
//2:设置对齐方式(SetXAlign)
//		A:文字在右 0 
//		B:文字在下 1
//		C:文字在上 2
//		D:文字在左 3
//3:设置图标(SetXico)
//------------------------------------------------------设计人:牛文平
#if !defined(AFX_MyButton_H__5254170E_59CF_11D1_ABBA_00A0243D1382__INCLUDED_)
#define AFX_MyButton_H__5254170E_59CF_11D1_ABBA_00A0243D1382__INCLUDED_
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
class CMyButton : public CButton
{
// Construction
public:
	CMyButton();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyButton();
	//形状的设置数组
	CRgn   m_rgn;
	//效果色1
	COLORREF m_clrLeft;
	//效果色2
	COLORREF m_clrRight;
	//文本色
	COLORREF m_clrText;
	//背景色
	COLORREF m_clrBack;
	//设置为Flat时的背景色
	COLORREF m_clrFlatBack;
	//显示方式
	//文本 0 图形 1 ;
	int intStyle;
	//对齐方式
	//文字在右 0  文字在下1 文字在上2 文字在左 3
	int	intAlgin;
	//鼠标是否是按下状态
	BOOL m_blnMouseOn;
	//设置按钮样式
	//0-方形 1-方圆 2-椭圆
	int intButtonFace;
	//图标
	HICON Xicon;
	//图标的高度
	DWORD dwIconHeight;
	//图标的宽度
	DWORD dwIconWidth;
	//设置背景色
	void SetBackColor(COLORREF color);
	//设置文本色
	void SetTextColor(COLORREF color);
	//设置效果色
	void SetMaskColor(COLORREF colLeft,COLORREF colRight);
	//设置设置为Flat时的背景色
	void SetFlatBack(COLORREF colFlat);
	//鼠标离开按钮时触发
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	LRESULT OnSetStyle(WPARAM wParam, LPARAM lParam);

	//设置图标
	void SetXIcon(UINT nIDResource);
	//显示方式
	//文字在右 0  文字在下1 文字在上2 文字在左 3
	void SetXAlign(int Align);
	//图像列表
	CImageList m_imgList;
	//图像大小
	CSize m_imgSize;
	//文本 0 图形 1;
	void SetXStyle(int Style);
	//设置背景
	void SetBackImage(UINT nIDResource);
	//设置按钮样式
	void SetButtonFace(int intStyle);
	//设置图像按钮
	void SetForeImage(UINT nIDResource,CSize sz);
	// Generated message map functions
protected:
	//{{AFX_MSG(CMyButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnSysColorChange();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	//画图标
	void DrawIconX(CDC *pDC,CRect rcItem);
	//销毁图标，释放内存
	void DestroyXIcon();
	//得到图标的信息
	void GetXiconInf();
	void DrawImgX(CDC *pDC,CRect rcItem);
	//背景画刷
	CBrush m_BackBrush;
	BOOL m_bIsDefault;
	UINT m_nTypeStyle;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MyButton_H__5254170E_59CF_11D1_ABBA_00A0243D1382__INCLUDED_)
