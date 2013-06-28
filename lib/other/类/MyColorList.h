//-----------------------------------------------------设计人：牛文平
#ifndef AFX_MYCOLORLISTCTRL_H__9FC7D36B_FC88_42FD_8DAA_2F70016038BA__INCLUDED_
#define AFX_MYCOLORLISTCTRL_H__9FC7D36B_FC88_42FD_8DAA_2F70016038BA__INCLUDED_
#include "MyHeaderCtrl.h"
#include <comdef.h>
//Item的颜色设置
struct CMyColor
{
	COLORREF colText;
	COLORREF colBack;
	COLORREF colSelect;
};
class CMyColorList : public CListCtrl
{
	//存储ITEM的颜色数组
	CObArray m_ArrayCol;
	//存储列的对齐方式
	CStringArray m_ArrayHeaderAlign;
public:
	CMyColorList();
	//列头的重绘
	CMyHeaderCtrl m_ctlHeader;
	//设置是否可选
	BOOL m_blnSelect;
	//设置列头
	void SetHeader();
	//添加一项
    int AddItem(int image,COLORREF colText);
	//添加一项
    int AddItem(int image,COLORREF colText,COLORREF colBack,COLORREF colSelect);
	//设置ITEM的颜色
	void SetItemColor(int nitem,COLORREF colText,COLORREF colBack,COLORREF colSelect);
	void SetItemColor(int nitem,COLORREF colText);
	//返回ITEM的颜色
	CMyColor GetItemColor(int nitem);
public:
	virtual ~CMyColorList();
	virtual void PreSubclassWindow();
	virtual	void DeleteAllItems();
	virtual void DeleteItem(int nItem);
	virtual void InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat /* = LVCFMT_LEFT */, int nWidth /* = -1 */, int nSubItem /* = -1 */);
	virtual void InsertItem(int nItem, LPCTSTR lpszItem, int nImage);
protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//{{AFX_MSG(CMyColorList)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif // AFX_COLORLISTCTRL_H__9FC7D36B_FC88_42FD_8DAA_2F70016038BA__INCLUDED_
