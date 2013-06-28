//---------------------------------------------------------设计人:牛文平
#ifndef MYSORTHEADERCTRL_H
#define MYSORTHEADERCTRL_H

#if _MSC_VER > 1000
#pragma once
#endif
class CMyHeaderCtrl : public CHeaderCtrl
{
public:
	CMyHeaderCtrl();
	//{{AFX_VIRTUAL(CMyHeaderCtrl)
	public:
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL
public:
	virtual ~CMyHeaderCtrl();
	//排列设置
	void SetSortArrow( const int iColumn, const BOOL bAscending );
	//初始化时所有列头重绘
	void SetDefault();
	//背景色
	COLORREF m_clrBack;
	//效果色1
	COLORREF m_clrLeft;
	//效果色2
	COLORREF m_clrRight;
protected:
	void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
	//列多少
	int m_iSortColumn;
	//升序与降序
	BOOL m_bSortAscending;
	//是否初始化重绘
	BOOL m_blnDefault;
	//{{AFX_MSG(CMyHeaderCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
#endif // SORTHEADERCTRL_H
