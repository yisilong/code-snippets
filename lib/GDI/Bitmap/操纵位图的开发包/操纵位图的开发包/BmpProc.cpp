///////////////////////////////////////////////////////////////////////
/*
  目的：BMP位图处理类。（源代码文件）

  创建：张伟(SXZ)
  日期：2000.01
  版本：2.4

  SXZ software workgroup. No.200001
*/
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
/*
  注释：

  # 类中的位图都是兼容于当前显示器颜色格式的DDB位图
  # 如果读入的位图格式与当前显示器的颜色格式不同，则读入函数会先将其转换
    为兼容于当前显示器颜色格式的位图之后才放入类中
  # 对于所有BOOL型返回的函数，TRUE表示成功，FALSE表示失败


*/
///////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <math.h>
#include <io.h>
#include <errno.h>
#include <direct.h>

#include "BmpProc.h"


////////////////////////////// defines ///////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// 用于调试目的计数变量，它表示程序中构造的CBmpProc类实例个数
// 可以用类中static DWORD GetObjectCount()成员函数访问该变量
// 注意：该变量和访问它的函数都只是在调试模式下才会有效，如果
//		 转换为发布模式，它们都会失效。所以在使用GetObjectCount()
//		 这个成员函数时，应加入#ifdef _DEBUG .... #endif语句块
#ifdef _DEBUG
	DWORD CBmpProc::m_ObjectCount = 0;
#endif




/*************************************************************************
 * 构造函数。初始化类为空。
*************************************************************************/


CBmpProc::CBmpProc()
{
	// 初始化核心数据为空
	m_mark = FALSE;			// 有效标志。（TRUE-有有效位图，否则为FALSE）
	m_type = IT_NULL;		// 图像类型。
	m_addInfo = 0;			// 附加信息。
	m_cFileName.Empty();	// 文件路径。
	m_pInfo = NULL;			// DIB信息结构
	m_hObject = NULL;		// 清除核心DDB位图句柄。（基类成员）

#ifdef _DEBUG
	CBmpProc::m_ObjectCount++;
#endif
}



/*************************************************************************
 * 调试用函数，获取进程中CBmpProc类目标的个数。
*************************************************************************
#ifdef _DEBUG
DWORD CBmpProc::GetObjectCount()
{
	return m_ObjectCount;
}
#endif
*************************************************************************/


/*************************************************************************
 * 析构函数。如果类不为空则清空它。
*************************************************************************/


CBmpProc::~CBmpProc()
{
	Clear();

#ifdef _DEBUG
	CBmpProc::m_ObjectCount--;
#endif
}




/*************************************************************************
 *
 * operator=()
 *
 * 参数说明：
 * 
 * const CBmpProc& ds		- 源位图类
 * 
 * 返回值：
 * 
 * CBmpProc&				- 类自身引用
 * 
 * 描述：
 *
 * 复制取指定的位图类
 * 
 * 如果函数成功，则类中原来的位图（如果有的话）将被删除，如果不成功，它保留
 * 原位图。执行本函数之后，可调用该类的IsValid()函数判断是否成功复制（这种
 * 情况只使用于没有图像的类，如果复制前类中就存在图像，可用==操作符判断该类
 * 与源位图类是否相同，如果相同则表示复制成功，不同就说明失败）
 * 
 ************************************************************************/


CBmpProc& CBmpProc::operator=(const CBmpProc& ds)
{
	// 如果类中没有图像，直接返回
	if (!ds.IsValid())
		return *this;

	ASSERT(ds.m_pInfo);
	ASSERT(ds.m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	// 复制源图信息块
	LPBITMAPINFO pInfo = (LPBITMAPINFO)CreateMemoryBlockFromAddress(
		(LPVOID)ds.m_pInfo);
	if (!pInfo)
		return *this;

	// 复制源图
	CBitmap		tempBmp;
	HBITMAP		hBmp;

	HWND hWnd = ::GetDesktopWindow();
	ASSERT(hWnd);
	HDC hDC = ::GetDC(hWnd);
	CDC dc;
	dc.Attach(hDC);

	if (tempBmp.CreateCompatibleBitmap(&dc, ds.Width(), ds.Height()))
	{
		CDC			compDC, compDC2;
		CBitmap		*pOldBmp, *pOldBmp2;

		// 创建与当前显示设备兼容的内存设备描述表
		compDC.CreateCompatibleDC(NULL);
		compDC2.CreateCompatibleDC(NULL);

		pOldBmp	= compDC.SelectObject((CBitmap*)&tempBmp);
		pOldBmp2= compDC2.SelectObject((CBitmap*)&ds);

		// 复制指定尺寸的源位图到目标位图
		compDC.BitBlt(0, 0, ds.Width(), ds.Height(), 
					&compDC2, 0, 0, SRCCOPY);

		compDC2.SelectObject(pOldBmp2);
		compDC.SelectObject(pOldBmp);

		hBmp = (HBITMAP)tempBmp.Detach();
		// 如果分离操作失败，返回FALSE
		ASSERT(hBmp);
		if (!hBmp)
		{
			::ReleaseDC(hWnd, dc.Detach());
			::free((void*)pInfo);
			return *this;;
		}
		::ReleaseDC(hWnd, dc.Detach());

		// 删除原来的图像，并且设置新的位图
		if (!ClearAndSetData(IT_MEMORY,0,(LPCTSTR)"",pInfo,hBmp))
		{
			::free((void*)pInfo);
			::DeleteObject(hBmp);
			return *this;
		}
		return *this;
	}
	else
	{
		::ReleaseDC(hWnd, dc.Detach());
		::free((void*)pInfo);

		return *this;;
	}
}



/*************************************************************************
 *
 * operator=()
 *
 * 参数说明：
 * 
 * const HBITMAP sou		- 源位图句柄
 * 
 * 返回值：
 * 
 * CBmpProc&				- 类自身引用
 * 
 * 描述：
 *
 * 复制取指定的位图
 * 
 * 如果函数成功，则类中原来的位图（如果有的话）将被删除，如果不成功，它保留
 * 原位图。执行本函数之后，可调用该类的IsValid()函数判断是否成功复制（这种
 * 情况只使用于没有图像的类，如果复制前类中就存在图像，可用==操作符判断该类
 * 与源位图类是否相同，如果相同则表示复制成功，不同就说明失败）
 * 如果入口参数指定的位图句柄无效，则函数失败。
 * 
 ************************************************************************/


CBmpProc& CBmpProc::operator=(const HBITMAP sou)
{
	// 入口检测
	if (!sou)
		return *this;

	CBmpProc tmp;

	if (!tmp.Attach(sou))
		return *this;

	ASSERT(sou == (HBITMAP)tmp);

	// 调用类赋值操作符函数
	*this = tmp;
	// 分离源位图，防止析构函数将其删除
	tmp.Detach();

	return *this;
}



/*************************************************************************
 *
 * operator==()
 *
 * 参数说明：
 * 
 * const CBmpProc& ds		- 源位图类
 * 
 * 返回值：
 * 
 * BOOL						- 如果相同返回TRUE，不同返回FALSE
 * 
 * 描述：
 *
 * 判断源位图类与本身类是否相同
 * 
 * 该函数通过比较两个类的位图来判定它们是否具有相同的图像。
 * 
 * 注：# 如果两个类都是空的（即都没有图像），则该函数将视它们是不相同的
 *
 ************************************************************************/


BOOL CBmpProc::operator==(const CBmpProc& ds)
{
	// 如果两个类中有一个或都是空的（即都没有图像），则该函数将视它们
	// 是不相同的
	if ((ds.IsValid()==FALSE)||(IsValid()==FALSE))
		return FALSE;

	// 如果是与自身比较，则直接返回TRUE
	if (this == &ds)
		return TRUE;

	ASSERT(m_pInfo);
	ASSERT(ds.m_pInfo);
	ASSERT(m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));
	ASSERT(ds.m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	// 信息块长度比较
	if (::_msize(m_pInfo) != ::_msize(ds.m_pInfo))
		return FALSE;

	// 信息块内容比较
	if (::memcmp((const void*)m_pInfo, \
		(const void*)ds.m_pInfo,::_msize(m_pInfo)))
		return FALSE;

	// 创建存放DIB位数据的缓冲区
	LPSTR pBits1 = (LPSTR)CreateMemoryBlockFromAddress((LPVOID)NULL, 
		CalculateDIBitsSize((LPBITMAPINFOHEADER)m_pInfo));
	if (!pBits1)
		return FALSE;

	LPSTR pBits2 = (LPSTR)CreateMemoryBlockFromAddress((LPVOID)NULL, 
		CalculateDIBitsSize((LPBITMAPINFOHEADER)m_pInfo));
	if (!pBits2)
	{
		::free((void*)pBits1);
		return FALSE;
	}

	// 复制位图信息块，因为GetDIBits()函数有可能会改变颜色表数据
	LPBITMAPINFO pInfo = (LPBITMAPINFO)CreateMemoryBlockFromAddress(
		(LPVOID)m_pInfo);
	if (!pInfo)
	{
		::free((void*)pBits1);
		::free((void*)pBits2);
		return FALSE;
	}

	ASSERT(pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	HWND hWnd = ::GetDesktopWindow();
	ASSERT(hWnd);
	HDC hDC;

	// 获取位图的DIB位数据
	hDC = ::GetDC(hWnd);
	ASSERT(m_hObject);
	ASSERT(ds.m_hObject);
	if (!::GetDIBits(hDC,(HBITMAP)m_hObject,0,pInfo->bmiHeader.biHeight, \
			(LPVOID)pBits1,pInfo,DIB_RGB_COLORS))
	{
		::free((void*)pBits1);
		::free((void*)pBits2);
		::free((void*)pInfo);
		::ReleaseDC(hWnd, hDC);
		return FALSE;
	}
	if (!::GetDIBits(hDC,(HBITMAP)ds.m_hObject,0,pInfo->bmiHeader.biHeight, \
			(LPVOID)pBits2,pInfo,DIB_RGB_COLORS))
	{
		::free((void*)pBits1);
		::free((void*)pBits2);
		::free((void*)pInfo);
		::ReleaseDC(hWnd, hDC);
		return FALSE;
	}
	::ReleaseDC(hWnd, hDC);
	::free((void*)pInfo);

	// 位内容比较
	LONG leng = (LONG)CalculateDIBitsSize((LPBITMAPINFOHEADER)m_pInfo)/4;
	LPDWORD lp1 = (LPDWORD)pBits1;	// DIB位数据是DWORD对齐的
	LPDWORD lp2 = (LPDWORD)pBits2;

	// 因为不清楚memcmp()函数是否可以比较大于64K的数据，所以才采用这种
	// 老笨的方法。
	while(leng--)
	{
		if (*lp1 != *lp2) break;
		lp1++; lp2++;
	}

	// 比较完之后，位数据已无用，释放
	::free((void*)pBits1);
	::free((void*)pBits2);
	
	if (leng != -1L)
		return FALSE;	// 位数据不同返回FALSE

	return TRUE;
}



/*************************************************************************
 *
 * operator!=()
 *
 * 参数说明：
 * 
 * const CBmpProc& ds		- 源位图类
 * 
 * 返回值：
 * 
 * BOOL						- 如果不同返回TRUE，相同返回FALSE
 * 
 * 描述：
 *
 * 判断源位图类是否与本身类不同
 * 
 * 该函数通过比较两个类的位图来判定它们是否具有相同的图像。
 * 
 * 注：# 如果两个类都是空的（即都没有图像），则该函数将视它们是不相同的
 *
 ************************************************************************/


BOOL CBmpProc::operator!=(const CBmpProc& ds)
{
	return ((*this) == ds) ? FALSE:TRUE;
}



/*************************************************************************
 *
 * Draw()
 *
 * 参数说明：
 *
 * CDC& dc					- 显示位图的设备描述表
 * const CRect* rcDst		- 目标矩形
 * const CRect* rcSrc		- 源矩形
 * 
 * 返回值：
 *
 * BOOL						- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 将类中指定区域的图像绘入目标设备描述表的指定区域，如果源区域（即矩形）
 * 和目标区域不同，将产生拉伸或压缩动作
 * 
 * 如果源矩形和目标矩形是NULL,则函数将视这两个矩形都等于类中图像的尺寸
 * 
 * 如果类中没有图像，它什么也不作，直接返回
 * 如果待绘制的图像不在当前剪贴区域内，则直接返回
 * 
 ************************************************************************/


BOOL CBmpProc::Draw(CDC& dc, const CRect* rcDst, const CRect* rcSrc)
{
	// 如果类中没有图像，直接返回
	if (!IsValid())
		return TRUE;

	ASSERT(m_hObject);

	// 缺省矩形等于图像尺寸
	CRect DCRect(Rect());			// 目标位置数据
	CRect DibRect(Rect());			// 源位置数据

	if (rcDst)
		DCRect = *rcDst;
	if (rcSrc)
		DibRect = *rcSrc;

	// 如果待绘制的图像不在当前剪贴区域内，则直接返回
	if (!dc.RectVisible(&DCRect))
		return TRUE;

	CDC			compDC;
	CBitmap		*pOldBmp;

	compDC.CreateCompatibleDC(NULL);
	pOldBmp	= compDC.SelectObject((CBitmap*)this);

	/* 设置目标DC的拉伸模式为STRETCH_DELETESCANS，也就是不显示拉伸掉的图像 */
	int srlold = dc.SetStretchBltMode(STRETCH_DELETESCANS);

	// 显示位图
	dc.StretchBlt(DCRect.left, DCRect.top, DCRect.Width(), DCRect.Height(), 
					&compDC, DibRect.left, DibRect.top, 
					DibRect.Width(), DibRect.Height(), SRCCOPY);
	
	// 恢复设备描述表原来的设置
	dc.SetStretchBltMode(srlold);
	compDC.SelectObject(pOldBmp);

	return TRUE;
}



/*************************************************************************
 *
 * DrawTile()
 *
 * 参数说明：
 *
 * CDC& dc					- 显示位图的设备描述表
 * CRect* crArea			- 铺设范围（矩形坐标）
 * 
 * 返回值：
 *
 * BOOL						- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 将类中的图像以平铺方式排满指定区域
 * 
 * 如果入口参数crArea是NULL,则函数返回FALSE，如果矩形坐标不符合规范（比如
 * 左边的坐标大于右边的坐标）则函数将自动将其规范化。如果矩形坐标的宽度
 * 或高度是0，则函数将返回FALSE
 * 
 * 如果类中没有图像，它什么也不作，直接返回
 * 
 ************************************************************************/


BOOL CBmpProc::DrawTile(CDC& dc, CRect* crArea)
{
	if (!crArea)
		return FALSE;

	// 如果类中没有图像，直接返回
	if (!IsValid())
		return TRUE;

	CRect rect = *crArea;
	CRect tprc;

	// 矩形坐标规范化
	rect.NormalizeRect();

	if ((rect.Width()==0)||(rect.Height()==0))
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_hObject);

	// 因为下面的代码将改变设备描述表中剪贴区域的设置，所以先将其保存
	int savedc = dc.SaveDC();

	CRgn newrgn;
	newrgn.CreateRectRgnIndirect(&rect);
	// 只显示指定矩形中可见的部分（用AND操作）
	dc.SelectClipRgn(&newrgn, RGN_AND);

	for (int y=rect.top;y<rect.bottom;y+=Height())
	{
		for (int x=rect.left;x<rect.right;x+=Width())
		{
			// 以瓦片方式重复显示图像
			tprc.SetRect(x,y,x+Width(),y+Height());
			Draw(dc, &tprc);
		}
	}

	dc.RestoreDC(savedc);
	// 恢复设备描述表的内容之后，将区域的句柄显示的删除
	// （微软在文档中特意注明要用CGdiObject::DeleteObject()
	//   函数显示的删除区域句柄，不知是什么原因）
	newrgn.CGdiObject::DeleteObject();

	return TRUE;
}



/*************************************************************************
 *
 * DrawCenter()
 *
 * 参数说明：
 *
 * CDC& dc					- 显示位图的设备描述表
 * CRect* crArea			- 显示范围（矩形坐标）
 * 
 * 返回值：
 *
 * BOOL						- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 将类中图像绘入目标设备描述表的指定区域（矩形）的中央，即图像中点与矩
 * 形中点重合
 * 
 * 如果矩形的尺寸小于图像尺寸，则图像的四周部分会不可见
 * 如果入口参数crArea是NULL，函数将返回FALSE
 * 如果类中没有图像，它什么也不作，直接返回
 * 如果矩形坐标不符合规范（比如左边的坐标大于右边的坐标）则函数将自动将
 * 其规范化。如果矩形坐标的宽度或高度是0，则函数将返回FALSE
 * 
 ************************************************************************/


BOOL CBmpProc::DrawCenter(CDC& dc, CRect* crArea)
{
	if (!crArea)
		return FALSE;

	// 如果类中没有图像，直接返回
	if (!IsValid())
		return TRUE;

	CRect rect = *crArea;

	// 矩形坐标规范化
	rect.NormalizeRect();

	if ((rect.Width()==0)||(rect.Height()==0))
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_hObject);

	// 取指定矩形的中点坐标
	CPoint cpce = rect.CenterPoint();
	CRect  tprc(CPoint(cpce.x-Width()/2,cpce.y-Height()/2), Size());

	// 因为下面的代码将改变设备描述表中剪贴区域的设置，所以先将其保存
	int savedc = dc.SaveDC();

	CRgn newrgn;
	newrgn.CreateRectRgnIndirect(&rect);
	// 只显示指定矩形中可见的部分（用AND操作）
	dc.SelectClipRgn(&newrgn, RGN_AND);

	Draw(dc, &tprc);

	dc.RestoreDC(savedc);
	newrgn.CGdiObject::DeleteObject();

	return TRUE;
}



/*************************************************************************
 *
 * DrawTranCenter()
 *
 * 参数说明：
 *
 * CDC& dc					- 显示位图的设备描述表
 * CRect* crArea			- 显示范围（矩形坐标）
 * COLORREF crColour		- 要滤掉或保留的颜色（缺省值是白色）
 * int mode					- 如果是0则将不显示指定的颜色，如果是1则保留指定
 *						      的颜色（缺省值为0）
 * 
 * 返回值：
 *
 * BOOL						- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 将类中图像透明的绘入目标设备描述表的指定区域（矩形）的中央，即图像中点
 * 与矩形中点重合
 * 
 * 如果矩形的尺寸小于图像尺寸，则超出矩形的图像部分会不可见
 * 如果入口参数crArea是NULL，函数将返回FALSE
 * 如果类中没有图像，它什么也不作，直接返回
 * 如果矩形坐标不符合规范（比如左边的坐标大于右边的坐标）则函数将自动将
 * 其规范化。如果矩形坐标的宽度或高度是0，则函数将返回FALSE
 * 
 ************************************************************************/


BOOL CBmpProc::DrawTranCenter(CDC& dc, 
							  CRect* crArea, 
							  COLORREF crColour, 
							  int mode)
{
	if (!crArea)
		return FALSE;

	// 如果类中没有图像，直接返回
	if (!IsValid())
		return TRUE;

	CRect rect = *crArea;

	// 矩形坐标规范化
	rect.NormalizeRect();

	if ((rect.Width()==0)||(rect.Height()==0))
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_hObject);

	// 取指定矩形的中点坐标
	CPoint cpce = rect.CenterPoint();
	CRect  tprc(CPoint(cpce.x-Width()/2,cpce.y-Height()/2), Size());

	// 因为下面的代码将改变设备描述表中剪贴区域的设置，所以先将其保存
	int savedc = dc.SaveDC();

	CRgn newrgn;
	newrgn.CreateRectRgnIndirect(&rect);
	// 只显示指定矩形中可见的部分（用AND操作）
	dc.SelectClipRgn(&newrgn, RGN_AND);

	DrawTransparentInRect(dc, crColour, &tprc, NULL, mode);	// 显示透明图像

	dc.RestoreDC(savedc);
	newrgn.CGdiObject::DeleteObject();

	return TRUE;
}



/*************************************************************************
 *
 * DrawTranTile()
 *
 * 参数说明：
 *
 * CDC& dc					- 显示位图的设备描述表
 * CRect* crArea			- 显示范围（矩形坐标）
 * COLORREF crColour		- 要滤掉或保留的颜色（缺省值是白色）
 * int mode					- 如果是0则将不显示指定的颜色，如果是1则保留指定
 *						      的颜色（缺省值为0）
 * 
 * 返回值：
 *
 * BOOL						- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 将类中的图像以平铺并且是透明的方式排满指定区域
 * 
 * 如果入口参数crArea是NULL,则函数返回FALSE，如果矩形坐标不符合规范（比如
 * 左边的坐标大于右边的坐标）则函数将自动将其规范化。如果矩形坐标的宽度
 * 或高度是0，则函数将返回FALSE
 * 
 * 如果类中没有图像，它什么也不作，直接返回
 * 
 ************************************************************************/


BOOL CBmpProc::DrawTranTile(CDC& dc, 
							CRect* crArea, 
							COLORREF crColour, 
							int mode)
{
	if (!crArea)
		return FALSE;

	// 如果类中没有图像，直接返回
	if (!IsValid())
		return TRUE;

	CRect rect = *crArea;
	CRect tprc;

	// 矩形坐标规范化
	rect.NormalizeRect();

	if ((rect.Width()==0)||(rect.Height()==0))
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_hObject);

	// 因为下面的代码将改变设备描述表中剪贴区域的设置，所以先将其保存
	int savedc = dc.SaveDC();

	CRgn newrgn;
	newrgn.CreateRectRgnIndirect(&rect);
	// 只显示指定矩形中可见的部分（用AND操作）
	dc.SelectClipRgn(&newrgn, RGN_AND);

	for (int y=rect.top;y<rect.bottom;y+=Height())
	{
		for (int x=rect.left;x<rect.right;x+=Width())
		{
			// 以瓦片方式重复显示图像
			tprc.SetRect(x,y,x+Width(),y+Height());
			// 显示透明图像
			DrawTransparentInRect(dc, crColour, &tprc, NULL, mode);
		}
	}

	dc.RestoreDC(savedc);
	newrgn.CGdiObject::DeleteObject();

	return TRUE;
}



/*************************************************************************
 *
 * Clear()
 *
 * 参数说明：无
 *
 *
 * 返回值：无
 *
 *
 * 描述：
 *
 * 清除类中的图像
 * 
 * 这个函数将删除类中的位图句柄，并清除类中的描述变量
 * 
 * 如果类中没有图像，它什么也不作，直接返回
 * 
 ************************************************************************/


void CBmpProc::Clear()
{
	// 如果本类中没有有效的BMP，则直接返回
	if (m_mark == FALSE)
	{
		ASSERT(m_type == IT_NULL);
		ASSERT(m_pInfo == NULL);
		ASSERT(m_hObject == NULL);
		return;
	}

	switch(m_type)
	{
	case	IT_NULL:
		ASSERT(FALSE);	// 此时m_type不应该是IT_NULL
		break;
	case	IT_DISKFILE:
		ASSERT(!m_cFileName.IsEmpty());	// 文件来源的位图应该有文件名
	case	IT_RESOURCE:
	case	IT_MEMORY:
	case	IT_CLIP:
	case	IT_CREATE:
		m_cFileName.Empty();

		ASSERT(m_pInfo);
		::free((void*)m_pInfo);
		m_pInfo = NULL;

		// 先分离出原来的BMP句柄，然后删除
		ASSERT((HBITMAP)m_hObject);
		::DeleteObject(CGdiObject::Detach());
		m_hObject = NULL;

		m_addInfo = 0;
		m_type = IT_NULL;
		m_mark = FALSE;
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}



/*************************************************************************
 *
 * Attach()
 *
 * 参数说明：
 * 
 * HBITMAP hBmp				- 待粘贴的位图句柄
 * 
 * 返回值：
 * 
 * BOOL						- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 将指定的位图贴入本身类中。
 * 
 * 注：
 * 
 * # 如果hBmp是一个无效的句柄或是NULL，函数返回FALSE
 * # 如果本身类中原来存在图像，则调用函数成功之后，原来的图像将被删除
 * # hBmp必须是一个兼容于当前显示器颜色格式的DDB位图句柄
 * # 如果函数执行成功后，调用它的程序不应再使用hBmp句柄，因为它已被贴入类中
 * # 如果函数失败，调用它的程序仍可以正常使用hBmp句柄
 * 
 ************************************************************************/


BOOL CBmpProc::Attach(HBITMAP hBmp)
{
	ASSERT(hBmp);
	// 如果hBmp是NULL，则立即返回
	if (!hBmp)
		return FALSE;

	HWND hWnd = ::GetDesktopWindow();
	ASSERT(hWnd);
	HDC hDC;
	BITMAP	bmp;

	// 获取给定位图的宽度、高度及颜色格式信息
	if (::GetObject(hBmp, sizeof(BITMAP), (LPVOID)&bmp) != sizeof(BITMAP))
		return FALSE;

	// 创建临时位图信息块（不带颜色表）
	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)CreateMemoryBlockFromAddress(
		(LPVOID)NULL, sizeof(BITMAPINFOHEADER));
	if (!lpbi)
		return FALSE;

	// 设置DIB信息块内容
	lpbi->biSize = sizeof(BITMAPINFOHEADER);
	lpbi->biWidth = bmp.bmWidth;
	lpbi->biHeight = bmp.bmHeight;
	lpbi->biPlanes = 1;
	// 在16色系统中，平面数是4，而位数是1，但这在DIB中是不允许的
	lpbi->biBitCount = max(bmp.bmPlanes, bmp.bmBitsPixel);
	if ((lpbi->biBitCount==16)||(lpbi->biBitCount==32))
		lpbi->biCompression = BI_BITFIELDS;
	else
		lpbi->biCompression = BI_RGB;

	// 计算设备分辩率
	hDC = ::GetDC(hWnd);
	lpbi->biXPelsPerMeter = \
		(GetDeviceCaps(hDC,HORZRES)*1000)/GetDeviceCaps(hDC,HORZSIZE);
	lpbi->biYPelsPerMeter = \
		(GetDeviceCaps(hDC,VERTRES)*1000)/GetDeviceCaps(hDC,VERTSIZE);
	::ReleaseDC(hWnd, hDC);

	lpbi->biClrUsed = 0;
	lpbi->biClrImportant = 0;
	lpbi->biSizeImage = CalculateDIBitsSize(lpbi);

	// 计算颜色表的尺寸
	WORD wPalSize = PaletteSize(lpbi);

	// 创建带颜色表的信息块，并复制lpbi信息块中的内容
	LPBITMAPINFO pInfo = (LPBITMAPINFO)CreateMemoryBlockFromAddress(
		(LPVOID)lpbi, sizeof(BITMAPINFOHEADER)+wPalSize);
	if (!pInfo)
	{
		::free((void*)lpbi);
		return FALSE;
	}

	ASSERT(pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	// 此时该结构已经无用，释放
	::free((void*)lpbi);

	// 创建存放DIB位数据的缓冲区
	LPSTR pBits = (LPSTR)CreateMemoryBlockFromAddress((LPVOID)NULL, 
		pInfo->bmiHeader.biSizeImage);
	if (!pBits)
	{
		::free((void*)pInfo);
		return FALSE;
	}

	// 下面的代码并不是要获取位图的DIB位数据，而是要取得位图的颜色表
	// 颜色表数据在函数（GetDIBits()）调用成功后被填写在pInfo结构中
	hDC = ::GetDC(hWnd);
	if (!::GetDIBits(hDC,hBmp,0,pInfo->bmiHeader.biHeight, \
			(LPVOID)pBits,pInfo,DIB_RGB_COLORS))
	{
		::free((void*)pBits);
		::free((void*)pInfo);
		::ReleaseDC(hWnd, hDC);
		return FALSE;
	}
	::ReleaseDC(hWnd, hDC);

	// 此时并不需要DIB位数据，释放
	::free((void*)pBits);

	// 删除原来的图像，并且设置新的位图
	if (!ClearAndSetData(IT_MEMORY,0,(LPCTSTR)"",pInfo,hBmp))
	{
		::free((void*)pInfo);
		// hBmp指定的位图并不删除
		return FALSE;
	}

	return TRUE;
}



/*************************************************************************
 *
 * Detach()
 *
 * 参数说明：无
 * 
 * 
 * 返回值：
 * 
 * HBITMAP					- 如果成功，返回分离出的位图句柄，否则返回NULL
 * 
 * 描述：
 *
 * 将本身类中的位图分离出来。
 * 
 * 注：
 * 
 * # 如果本身类中原来存在位图，函数成功调用之后，本身类将被清空
 * # 如果本身类中原来没有位图，函数返回NULL
 * 
 ************************************************************************/


HBITMAP CBmpProc::Detach()
{
	if (!IsValid())
		return NULL;

	ASSERT(m_pInfo);
	ASSERT(m_hObject);

	HBITMAP hBmp = (HBITMAP)CGdiObject::Detach();
	ASSERT(hBmp);
	if (!hBmp)
		return NULL;

	m_hObject = NULL;

	switch(m_type)
	{
	case	IT_NULL:
		ASSERT(FALSE);	// 此时m_type不应该是IT_NULL
		break;
	case	IT_DISKFILE:
		ASSERT(!m_cFileName.IsEmpty());	// 文件来源的位图应该有文件名
	case	IT_RESOURCE:
	case	IT_MEMORY:
	case	IT_CLIP:
	case	IT_CREATE:
		m_cFileName.Empty();

		ASSERT(m_pInfo);
		::free((void*)m_pInfo);
		m_pInfo = NULL;

		m_addInfo = 0;
		m_type = IT_NULL;
		m_mark = FALSE;
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	return hBmp;
}



/*************************************************************************
 *
 * Save()
 *
 * 参数说明：
 * 
 * LPCTSTR lpszNewFileName	- 指定的新文件名（可以是NULL）
 * WORD nBitsPerPixel		- 指定的新颜色格式（即每像素多少位，可以是0）
 * 
 * 返回值：
 * 
 * BOOL						- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 将本身类中的位图以新的格式或新的名称保存到文件中
 * 
 * 注：
 * 
 * # 如果没有指定新的文件名（入口参数lpszNewFileName被指定为NULL），函数
 *   就使用类中原来的文件名，如果类中原来没有文件名则函数失败，返回FALSE
 * # 如果没有指定新的位数（入口参数nBitsPerPixel被指定为0），函数就使用类
 *   中原来的位数值。如果新指定的位数值非法则函数失败，返回FALSE。合法的
 *   位数值是1,4,8,16,24,32 其中之一
 * # 如果本身类中原来没有位图（空类），函数返回FALSE
 * # 如果文件没有打开或是写文件时出错，函数返回FALSE
 * # 函数执行成功之后，本身类中的文件名或位数将改为新值
 * # 无论函数成功与否，本身类中的位图不会改变
 * 
 ************************************************************************/


BOOL CBmpProc::Save(LPCTSTR lpszNewFileName, WORD nBitsPerPixel)
{
	// 如果本身类中原来没有位图，函数返回FALSE
	if (!IsValid())
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	LPBITMAPINFO pInfo;
	BOOL bRet;
	CString cOldFN;

	// 以原来的格式保存到原来的文件中
	if ((lpszNewFileName==NULL)&&(nBitsPerPixel==0))
	{
		// 应该有文件名
		if (m_cFileName.IsEmpty())
			return FALSE;

		return SaveBaseSpecifyFileName((LPCTSTR)m_cFileName);
	}

	// 以原来的格式保存到新的文件中
	if ((lpszNewFileName!=NULL)&&(nBitsPerPixel==0))
	{
		bRet = SaveBaseSpecifyFileName((LPCTSTR)lpszNewFileName);

		if (bRet)
			m_cFileName = lpszNewFileName;

		return bRet;
	}

	// 以新的格式保存到原来的文件中
	if ((lpszNewFileName==NULL)&&(nBitsPerPixel!=0))
	{
		// 检测位数合法性
		if (!IsValidDibFormat(nBitsPerPixel))
			return FALSE;

		// 应该有文件名
		if (m_cFileName.IsEmpty())
			return FALSE;

		bRet = SaveBaseSpecifyFormat(nBitsPerPixel);

		if (bRet)
		{
			pInfo = GetSpecifyFormatInfo(nBitsPerPixel);
			if (!pInfo)
				return FALSE;
			::free((void*)m_pInfo);
			m_pInfo = pInfo;
		}
		return bRet;
	}

	// 以新的格式保存到新的文件中
	if ((lpszNewFileName!=NULL)&&(nBitsPerPixel!=0))
	{
		// 检测位数合法性
		if (!IsValidDibFormat(nBitsPerPixel))
			return FALSE;

		cOldFN = m_cFileName;
		m_cFileName = lpszNewFileName;

		bRet = SaveBaseSpecifyFormat(nBitsPerPixel);

		if (bRet)
		{	// 获取指定格式的信息块
			pInfo = GetSpecifyFormatInfo(nBitsPerPixel);
			if (!pInfo)
			{
				m_cFileName = cOldFN;
				return FALSE;
			}
			::free((void*)m_pInfo);
			m_pInfo = pInfo;
		}
		else
			m_cFileName = cOldFN;

		return bRet;
	}

	return FALSE;
}



/*************************************************************************
 *
 * SaveToClipboard()
 *
 * 参数说明：无
 * 
 * 
 * 返回值：
 * 
 * BOOL						- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 保存本身类中的位图到剪贴板
 * 
 * 注：
 * 
 * # 如果本身类中原来没有位图，函数返回FALSE
 * # 如果在打开或写入剪贴板时出错，函数返回FALSE
 * # 无论函数成功与否，本身类中的内容都不会改变
 * 
 ************************************************************************/


BOOL CBmpProc::SaveToClipboard()
{
	// 如果本身类中原来没有位图，函数返回FALSE
	if (!IsValid())
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	// 打开剪贴板
	if (::OpenClipboard(NULL))
	{
		// 清空剪贴板
		if (::EmptyClipboard())
		{
			DWORD DibSize = CalculateDIBSize((LPBITMAPINFOHEADER)m_pInfo);
			DWORD infosize =CalculateDIBInfoSize((LPBITMAPINFOHEADER)m_pInfo);

			// 分配DIB内部格式内存块
			HANDLE hDib = ::GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE, DibSize);
			if (hDib)
			{
				LPSTR lpDib = (LPSTR)::GlobalLock(hDib);
				if (lpDib)
				{
					::memcpy((void*)lpDib, (const void*)m_pInfo, infosize);

					// 获取DIB位数据
					HWND hWnd = ::GetDesktopWindow();
					ASSERT(hWnd);
					HDC hDC = ::GetDC(hWnd);
					if (!::GetDIBits(hDC,(HBITMAP)m_hObject,0,m_pInfo->bmiHeader.biHeight,\
							(LPVOID)((DWORD)lpDib+infosize),(LPBITMAPINFO)lpDib,DIB_RGB_COLORS))
					{
						::GlobalUnlock(hDib);
						::GlobalFree(hDib);
						::CloseClipboard();
						::ReleaseDC(hWnd, hDC);
						return FALSE;
					}
					::ReleaseDC(hWnd, hDC);

					::GlobalUnlock(hDib);

					// 将DIB数据贴入剪贴板
					if (::SetClipboardData(CF_DIB, hDib))
					{
						// 如果成功，DIB句柄不应释放
						::CloseClipboard();
						return TRUE;		// 成功
					}
					else
					{
						::GlobalFree(hDib);
						::CloseClipboard();
						return FALSE;		// 失败
					}
				}
				::GlobalFree(hDib);
			}
		}
		::CloseClipboard();
	}

	return FALSE;	// 失败
}



/*************************************************************************
 *
 * SaveBaseSpecifyFileName()
 *
 * 参数说明：
 * 
 * LPCTSTR fn				- 指定的文件名称
 * 
 * 返回值：
 * 
 * BOOL						- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 保存本身类中的位图到指定的文件中
 * 
 * 注：
 * 
 * # 如果指定的文件已经存在，则函数将覆盖该文件
 * # 如果本身类中原来没有位图，函数返回FALSE
 * # 如果文件没有打开或是写文件时出错，函数返回FALSE
 * # 函数并不修改类中文件名
 * 
 ************************************************************************/


BOOL CBmpProc::SaveBaseSpecifyFileName(LPCTSTR fn)
{
	// 如果本身类中原来没有位图，函数返回FALSE
	if (!IsValid())
		return FALSE;

	ASSERT(m_pInfo);

	// 如果没有指定文件名，返回FALSE
	if (!fn)
		return FALSE;

	FILE *file;

	// 用创建方式打开文件（二进制）
	file = ::fopen(fn, "w+b");
	if (!file)
		return FALSE;
	
	::fseek(file, 0, SEEK_SET);

	BITMAPFILEHEADER	bmf;

	bmf.bfType = DIB_HEADER_MARKER;		//((WORD) ('M' << 8) | 'B')
    bmf.bfSize = CalculateDIBFileSize((LPBITMAPINFOHEADER)m_pInfo);
    bmf.bfReserved1 = 0; 
    bmf.bfReserved2 = 0; 
    bmf.bfOffBits   = CalculateDIBitsOff((LPBITMAPINFOHEADER)m_pInfo);

	// 写入文件头信息
	if (!::fwrite((const void *)&bmf, sizeof(BITMAPFILEHEADER), 1, file))
	{
		::fclose(file);
		return FALSE;
	}

	// 复制信息块内容，之所以复制信息块而不直接使用m_pInfo，是因为
	// GetDIBits()函数有可能改变信息块颜色表的内容
	int infosize = CalculateDIBInfoSize((LPBITMAPINFOHEADER)m_pInfo);
	ASSERT(infosize == (int)::_msize((void*)m_pInfo));
	LPBITMAPINFO pInfo = (LPBITMAPINFO)CreateMemoryBlockFromAddress(
		(LPVOID)m_pInfo);
	if (!pInfo)
	{
		::fclose(file);
		return FALSE;
	}

	// 写入信息块内容
	if (!::fwrite((const void *)pInfo, infosize, 1, file))
	{
		::free((void*)pInfo);
		::fclose(file);
		return FALSE;
	}

	LPSTR pBits = (LPSTR)CreateMemoryBlockFromAddress((LPVOID)NULL, 
		CalculateDIBitsSize((LPBITMAPINFOHEADER)pInfo));
	if (!pBits)
	{
		::free((void*)pInfo);
		::fclose(file);
		return FALSE;
	}

	// 获取DIB位数据
	HWND hWnd = ::GetDesktopWindow();
	ASSERT(hWnd);
	HDC hDC = ::GetDC(hWnd);
	if (!::GetDIBits(hDC,(HBITMAP)m_hObject,0,pInfo->bmiHeader.biHeight,\
			(LPVOID)pBits,pInfo,DIB_RGB_COLORS))
	{
		::free((void*)pInfo);
		::free((void*)pBits);
		::fclose(file);
		::ReleaseDC(hWnd, hDC);
		return FALSE;
	}
	::ReleaseDC(hWnd, hDC);

	// 信息块已无用，释放
	::free((void*)pInfo);

	DWORD dwA, dwB, dwC;

	LPSTR lp = pBits;

	// 以分段方式写入位数据，每个段长度为32KB。
	dwA = CalculateDIBitsSize((LPBITMAPINFOHEADER)m_pInfo);	// 总长度
	dwB = dwA/32768;			// 段数（32768）
	dwC = dwA - (dwB*32768);	// 余数

	for (;dwB!=0;dwB--)
	{
		if (!::fwrite((const void *)lp, 32768, 1, file))
		{
			::free((void*)pBits);
			::fclose(file);
			return FALSE;
		}
		lp = (LPSTR)((DWORD)lp+32768UL);
	}

	// 写入剩余的位数据
	if (!::fwrite((const void *)lp, dwC, 1, file))
	{
		::free((void*)pBits);
		::fclose(file);
		return FALSE;
	}

	// 位数据已无用，释放
	::free((void*)pBits);

	// 关闭文件
	::fclose(file);

	return TRUE;
}



/*************************************************************************
 *
 * SaveBaseSpecifyFormat()
 *
 * 参数说明：
 * 
 * WORD nBitsPerPixel		- 指定的格式（即每像素占多少位）
 * 
 * 返回值：
 * 
 * BOOL						- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 以指定的颜色格式保存本身类中的位图到原来的文件中
 * 
 * 注：
 * 
 * # 如果指定的文件已经存在，则函数将覆盖该文件
 * # 如果本身类中原来没有位图，函数返回FALSE
 * # 如果文件没有打开或是写文件时出错，函数返回FALSE
 * # 如果指定的位数是非法值（1,4,8,16,24,32是合法值），函数返回FALSE
 * # 函数并不修改类中信息块的内容
 * 
 ************************************************************************/


BOOL CBmpProc::SaveBaseSpecifyFormat(WORD nBitsPerPixel)
{
	// 如果本身类中原来没有位图，函数返回FALSE
	if (!IsValid())
		return FALSE;

	ASSERT(m_pInfo);

	// 应该有文件名
	if (m_cFileName.IsEmpty())
		return FALSE;

	// 检测位数合法性
	if (!IsValidDibFormat(nBitsPerPixel))
		return FALSE;

	LPBITMAPINFO pTempInfo;
	BOOL bRet;

	// 获取指定格式DIB的信息块
	LPBITMAPINFO pInfo = GetSpecifyFormatInfo(nBitsPerPixel);
	if (!pInfo)
		return FALSE;

	// 保存原始信息块
	pTempInfo = m_pInfo;

	m_pInfo = pInfo;			   // 以原文件名存储
	bRet = SaveBaseSpecifyFileName((LPCTSTR)m_cFileName);
	m_pInfo = pTempInfo;

	::free((void*)pInfo);

	return bRet;
}



/*************************************************************************
 *
 * Create()
 *
 * 参数说明：
 * 
 * int width				- 创建位图的宽度
 * int height				- 创建位图的高度
 * WORD nBitCount			- 创建位图中每个像素所占的位数
 * const void *lpBits		- 新位图的初始化位数据（可以是NULL）
 * 
 * 返回值：
 * 
 * BOOL						- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 根据指定的数据创建一个位图，并贴入本身类中
 * 
 * 注：
 * 
 * # 入口参数width和height不能是0，如果指定了0，函数返回FALSE
 * # 入口参数nBitCount的有效值可以是1,4,8,16,24,32，其它的值将视为非法
 * # 如果本身类中原来存在图像，则调用函数成功之后，原来的图像将被删除
 * # 如果入口参数lpBits是NULL，则新创建的位不被初始化
 * 
 ************************************************************************/


BOOL CBmpProc::Create(CSize size, 
					  WORD nBitCount, 
					  const void *lpBits)
{ 
	return Create(size.cx, size.cy, nBitCount, lpBits); 
}

BOOL CBmpProc::Create(int width, 
					  int height, 
					  WORD nBitCount, 
					  const void *lpBits)
{
	// 检测宽度和高度值
	if ((width==0)||(height==0))
		return FALSE;

	// 检测位数合法性
	if (!IsValidDibFormat(nBitCount))
		return FALSE;

	CBitmap		cBmp;
	HBITMAP		hBmp;
	int			iCurDevBitPix, iCurDevPlanes;

	// 获取当前系统显示器的颜色格式（位数和平面数）
	HWND hWnd = ::GetDesktopWindow();
	ASSERT(hWnd);
	HDC hDC = ::GetDC(hWnd);
	iCurDevBitPix = GetDeviceCaps(hDC,BITSPIXEL);
	iCurDevPlanes = GetDeviceCaps(hDC,PLANES);
	::ReleaseDC(hWnd, hDC);

	// 创建指定尺寸并兼容当前显示器颜色格式的位图（DDB）
	if (!cBmp.CreateBitmap(width, height, iCurDevPlanes, iCurDevBitPix, lpBits))
		return FALSE;

	hBmp = (HBITMAP)cBmp.Detach();
	ASSERT(hBmp);
	if (!hBmp)
		return FALSE;

	// 创建临时位图信息块（不带颜色表）
	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)CreateMemoryBlockFromAddress(
		(LPVOID)NULL, sizeof(BITMAPINFOHEADER));
	if (!lpbi)
	{
		::DeleteObject(hBmp);
		return FALSE;
	}

	// 设置DIB信息块内容
	lpbi->biSize = sizeof(BITMAPINFOHEADER);
	lpbi->biWidth = width;
	lpbi->biHeight = height;
	lpbi->biPlanes = 1;
	lpbi->biBitCount = nBitCount;
	if ((nBitCount==16)||(nBitCount==32))
		lpbi->biCompression = BI_BITFIELDS;
	else
		lpbi->biCompression = BI_RGB;

	// 计算设备分辩率
	hDC = ::GetDC(hWnd);
	lpbi->biXPelsPerMeter = \
		(GetDeviceCaps(hDC,HORZRES)*1000)/GetDeviceCaps(hDC,HORZSIZE);
	lpbi->biYPelsPerMeter = \
		(GetDeviceCaps(hDC,VERTRES)*1000)/GetDeviceCaps(hDC,VERTSIZE);
	::ReleaseDC(hWnd, hDC);

	lpbi->biClrUsed = 0;
	lpbi->biClrImportant = 0;
	lpbi->biSizeImage = CalculateDIBitsSize(lpbi);

	// 计算颜色表的尺寸
	WORD wPalSize = PaletteSize(lpbi);

	// 创建带颜色表的信息块，并复制lpbi信息块中的内容
	LPBITMAPINFO pInfo = (LPBITMAPINFO)CreateMemoryBlockFromAddress(
		(LPVOID)lpbi, sizeof(BITMAPINFOHEADER)+wPalSize);
	if (!pInfo)
	{
		::free((void*)lpbi);
		::DeleteObject(hBmp);
		return FALSE;
	}

	ASSERT(pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	// 此时该结构已经无用，释放
	::free((void*)lpbi);

	// 创建存放DIB位数据的缓冲区
	LPSTR pBits = (LPSTR)CreateMemoryBlockFromAddress((LPVOID)NULL, 
		pInfo->bmiHeader.biSizeImage);
	if (!pBits)
	{
		::free((void*)pInfo);
		::DeleteObject(hBmp);
		return FALSE;
	}

	// 下面的代码并不是要获取位图的DIB位数据，而是要取得位图的颜色表
	// 颜色表数据在函数（GetDIBits()）调用成功后被填写在pInfo结构中
	hDC = ::GetDC(hWnd);
	if (!::GetDIBits(hDC,hBmp,0,height,(LPVOID)pBits,pInfo,DIB_RGB_COLORS))
	{
		::free((void*)pBits);
		::free((void*)pInfo);
		::ReleaseDC(hWnd, hDC);
		::DeleteObject(hBmp);
		return FALSE;
	}
	::ReleaseDC(hWnd, hDC);

	// 此时并不需要DIB位数据，释放
	::free((void*)pBits);

	// 删除原来的图像，并且设置新的位图
	if (!ClearAndSetData(IT_CREATE,0,(LPCTSTR)"",pInfo,hBmp))
	{
		::free((void*)pInfo);
		::DeleteObject(hBmp);
		return FALSE;
	}

	return TRUE;
}



/*************************************************************************
 *
 * CreateCompatible()
 *
 * 参数说明：
 * 
 * int width				- 创建位图的宽度
 * int height				- 创建位图的高度
 * CDC *pDC, 				- 设备描述表（可以是NULL）
 * const void *lpBits		- 新位图的初始化位数据（可以是NULL）
 * 
 * 返回值：
 * 
 * BOOL						- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 根据指定的数据创建一个颜色格式兼容于指定描述表的位图，并贴入本身类中
 * 
 * 注：
 * 
 * # 入口参数width和height不能是0，如果指定了0，函数返回FALSE
 * # 入口参数pDC可以是NULL，这时函数将按当前显示器的颜色格式创建位图，如果
 *   pDC不是NULL，则它必须指向一个有效的设备描述表
 * # 如果本身类中原来存在图像，则调用函数成功之后，原来的图像将被删除
 * # 如果入口参数lpBits是NULL，则新创建的位不被初始化
 * 
 ************************************************************************/


BOOL CBmpProc::CreateCompatible(int width, 
								int height, 
								CDC *pDC, 
								const void *lpBits)
{
	// 检测宽度和高度值
	if ((width==0)||(height==0))
		return FALSE;

	int	iCurDevBitPix, iCurDevPlanes;

	// 获取当前系统显示器的颜色格式（位数和平面数）
	HWND hWnd = ::GetDesktopWindow();
	ASSERT(hWnd);
	if (pDC == NULL)
	{
		// 如果没有指定设备描述表，则使用当前显示器的颜色格式
		HDC hDC = ::GetDC(hWnd);
		iCurDevBitPix = GetDeviceCaps(hDC,BITSPIXEL);
		iCurDevPlanes = GetDeviceCaps(hDC,PLANES);
		::ReleaseDC(hWnd, hDC);
	}
	else
	{
		iCurDevBitPix = GetDeviceCaps(pDC->GetSafeHdc(),BITSPIXEL);
		iCurDevPlanes = GetDeviceCaps(pDC->GetSafeHdc(),PLANES);
	}

	// 在16色的系统中，每像素的位数是1，而平面是4，将其颠倒后容易处理
	if ((iCurDevPlanes!=1)&&(iCurDevBitPix==1))
	{
		iCurDevBitPix = iCurDevPlanes;
		iCurDevPlanes = 1;
	}

	// 创建位图（DDB）
	return Create(width, height, iCurDevBitPix, lpBits);
}



/*************************************************************************
 *
 * CopyFromObject()
 *
 * 参数说明：
 * 
 * CBmpProc& souBmp			- 源位图目标
 * CRect *pDesRect			- 目标矩形
 * CRect *pSouRect			- 源矩形
 * 
 * 返回值：
 * 
 * BOOL						- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 复制源位图目标中指定区域（矩形）的图像到本身类中图像的指定区域
 * 
 * 如果入口参数pDesRect设为NULL,则目标矩形等于源图像的尺寸
 * 如果入口参数pSouRect设为NULL,则源目标矩形等于源图像的尺寸
 * 如果入口参数pDesRect和pSouRect不同，该函数可能会产生拉伸或压缩动作
 * 
 * 注：# 本身类中原来必需已存在位图，且调用之后的位图不会被删除
 *     # 如果源位图类是空的，则返回FALSE
 *	   # 如果本身类中没有位图，则函数返回FALSE
 * 
 ************************************************************************/


BOOL CBmpProc::CopyFromObject(CBmpProc& souBmp, CRect *pDesRect, CRect *pSouRect)
{
	if (!IsValid())
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	if (!souBmp.IsValid())
		return FALSE;

	ASSERT(souBmp.m_pInfo);
	ASSERT(souBmp.m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));


	// 如果未指定矩形数据，则使用源图的矩形数据
	CRect dr;
	CRect sr;

	if (pDesRect == NULL)
		dr = souBmp.Rect();
	else
		dr = *pDesRect;

	if (pSouRect == NULL)
		sr = souBmp.Rect();
	else
		sr = *pSouRect;


	// 复制源图
	CDC			compDC, compDC2;
	CBitmap		*pOldBmp, *pOldBmp2;

	// 创建与当前显示设备兼容的内存设备描述表
	compDC.CreateCompatibleDC(NULL);
	compDC2.CreateCompatibleDC(NULL);

	pOldBmp	= compDC.SelectObject((CBitmap*)this);
	pOldBmp2= compDC2.SelectObject((CBitmap*)&souBmp);

	// 设置目标DC的拉伸模式为COLORONCOLOR，也就是不显示拉伸掉的图像
	compDC.SetStretchBltMode(COLORONCOLOR);

	// 复制指定尺寸的源位图到目标位图
	compDC.StretchBlt(dr.left, dr.top, dr.Width(), dr.Height(), 
			&compDC2, sr.left, sr.top, sr.Width(), sr.Height(), SRCCOPY);

	compDC2.SelectObject(pOldBmp2);
	compDC.SelectObject(pOldBmp);

	return TRUE;
}



/*************************************************************************
 *
 * CopyFromHbmp()
 *
 * 参数说明：
 * 
 * HBITMAP souBmp			- 源位图句柄
 * CRect *pDesRect			- 目标矩形
 * CRect *pSouRect			- 源矩形
 * 
 * 返回值：
 * 
 * BOOL						- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 复制源位图中指定区域（矩形）的图像到本身类中图像的指定区域
 * 
 * 如果入口参数pDesRect设为NULL,则目标矩形等于源图像的尺寸
 * 如果入口参数pSouRect设为NULL,则源目标矩形等于源图像的尺寸
 * 如果入口参数pDesRect和pSouRect不同，该函数可能会产生拉伸或压缩动作
 * 
 * 注：# 本身类中原来必需已存在位图，且调用之后的位图不会被删除
 *     # 如果源位图类是空的，则返回FALSE
 *	   # 如果本身类中没有位图，则函数返回FALSE
 * 
 ************************************************************************/


BOOL CBmpProc::CopyFromHbmp(HBITMAP souBmp, 
							CRect *pDesRect, 
							CRect *pSouRect)
{
	// 检查入口参数
	if (!souBmp)
		return FALSE;

	// 检查本类中是否有图像
	if (!IsValid())
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	CBmpProc tmp;

	// 将句柄贴入临时类中
	if (!tmp.Attach(souBmp))
		return FALSE;

	BOOL ret = CopyFromObject(tmp, pDesRect, pSouRect);

	tmp.Detach();

	return ret;
}



/*************************************************************************
 *
 * CopyTran()
 *
 * 参数说明：
 * 
 * CBmpProc& souBmp			- 源位图目标
 * COLORREF crColour		- 要滤掉或保留的颜色（缺省值是白色）
 * int x					- 目标矩形左上角X坐标（缺省值为0）
 * int y					- 目标矩形左上角Y坐标（缺省值为0）
 * CRect *pSouRect			- 源矩形（缺省值为NULL）
 * int mode					- 如果是0则将不拷贝指定的颜色，如果是1则保留指定
 *						      的颜色（缺省值为0）
 * 
 * 返回值：
 * 
 * BOOL						- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 透明的复制源位图目标中指定矩形的图像到本身类中图像的指定区域
 * 
 * 如果入口参数pSouRect设为NULL,则源目标矩形等于源图像的尺寸。这样该
 * 函数将不产生拉伸或压缩动作。
 * 如果入口参数pSouRect不设为NULL，该函数会产生拉伸或压缩动作
 * 
 * 注：# 本身类中原来必需已存在位图，且调用之后的位图不会被删除
 *     # 如果源位图类是空的，则返回FALSE
 *	   # 如果本身类中不存在位图，函数将返回FALSE
 * 
 ************************************************************************/


BOOL CBmpProc::CopyTran(CBmpProc& souBmp, 
						COLORREF crColour, 
						int x, int y, 
						CRect *pSouRect, 
						int mode)
{
	if (!IsValid())
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	if (!souBmp.IsValid())
		return FALSE;

	ASSERT(souBmp.m_pInfo);
	ASSERT(souBmp.m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	CRect dr(souBmp.Rect());
	dr.OffsetRect(x, y);

	return CopyTran(souBmp, crColour, &dr, pSouRect, mode);
}



/*************************************************************************
 *
 * CopyTran()
 *
 * 参数说明：
 * 
 * CBmpProc& souBmp			- 源位图目标
 * COLORREF crColour		- 要滤掉或保留的颜色（缺省值是白色）
 * CRect *pDesRect			- 目标矩形（缺省值为NULL）
 * CRect *pSouRect			- 源矩形（缺省值为NULL）
 * int mode					- 如果是0则将不拷贝指定的颜色，如果是1则保留指定
 *						      的颜色（缺省值为0）
 * 
 * 返回值：
 * 
 * BOOL						- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 透明的复制源位图目标中指定矩形的图像到本身类中图像的指定区域
 * 
 * 如果入口参数pDesRect设为NULL,则目标矩形等于源图像的尺寸。
 * 如果入口参数pSouRect设为NULL,则源目标矩形等于源图像的尺寸。
 * 如果pDesRect和pSouRect相同，则该函数将不会产生拉伸或压缩动作。
 * 
 * 注：# 本身类中原来必需已存在位图，且调用之后的位图不会被删除
 *     # 如果源位图类是空的，则返回FALSE
 *	   # 如果本身类中不存在位图，函数将返回FALSE
 * 
 ************************************************************************/


BOOL CBmpProc::CopyTran(CBmpProc& souBmp, 
						COLORREF crColour, 
						CRect *pDesRect, 
						CRect *pSouRect, 
						int mode)
{
	if (!IsValid())
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	if (!souBmp.IsValid())
		return FALSE;

	ASSERT(souBmp.m_pInfo);
	ASSERT(souBmp.m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));


	// 如果未指定矩形数据，则使用源图的矩形数据
	CRect dr;
	CRect sr;

	if (pDesRect == NULL)
		dr = souBmp.Rect();
	else
		dr = *pDesRect;

	if (pSouRect == NULL)
		sr = souBmp.Rect();
	else
		sr = *pSouRect;

	// 复制源图
	CDC			compDC;
	CBitmap		*pOldBmp;

	// 创建与当前显示设备兼容的内存设备描述表
	compDC.CreateCompatibleDC(NULL);
	pOldBmp	= compDC.SelectObject((CBitmap*)this);

	// 复制指定尺寸的源位图到目标位图
	souBmp.DrawTransparentInRect(compDC, crColour, pDesRect, pSouRect, mode);

	compDC.SelectObject(pOldBmp);

	return TRUE;
}



/*************************************************************************
 *
 * CopyTranCenter()
 *
 * 参数说明：
 * 
 * CBmpProc& souBmp			- 源位图目标
 * CRect* crArea 			- 居中的范围（缺省值为NULL）
 * COLORREF crColour		- 要滤掉或保留的颜色（缺省值是白色）
 * int mode					- 如果是0则将不拷贝指定的颜色，如果是1则保留指定
 *						      的颜色（缺省值为0）
 * 
 * 返回值：
 * 
 * BOOL						- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 将源位图透明的复制到本类中指定矩形的中央
 * 
 * 如果入口参数crArea设为NULL,则居中矩形等于本身类中图像的尺寸。
 * 该函数不会产生拉伸或压缩动作。
 * 
 * 注：# 本身类中原来必需已存在位图，且调用之后的位图不会被删除
 *     # 如果源位图类是空的，则返回FALSE
 *	   # 如果本身类中不存在位图，函数将返回FALSE
 * 
 ************************************************************************/


BOOL CBmpProc::CopyTranCenter(CBmpProc& souBmp, 
							  CRect* crArea, 
							  COLORREF crColour, 
							  int mode)
{
	if (!IsValid())
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	if (!souBmp.IsValid())
		return FALSE;

	ASSERT(souBmp.m_pInfo);
	ASSERT(souBmp.m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	CRect rect(Rect());

	if (crArea)
		rect = *crArea;

	// 矩形坐标规范化
	rect.NormalizeRect();

	if ((rect.Width()==0)||(rect.Height()==0))
		return FALSE;

	// 复制源图
	CDC			compDC;
	CBitmap		*pOldBmp;

	// 创建与当前显示设备兼容的内存设备描述表
	compDC.CreateCompatibleDC(NULL);
	pOldBmp	= compDC.SelectObject((CBitmap*)this);

	// 复制指定尺寸的源位图到目标位图
	souBmp.DrawTranCenter(compDC, &rect, crColour, mode);

	compDC.SelectObject(pOldBmp);

	return TRUE;
}



/*************************************************************************
 *
 * CopyTranTile()
 *
 * 参数说明：
 * 
 * CBmpProc& souBmp			- 源位图目标
 * CRect* crArea 			- 平铺的范围（缺省值为NULL）
 * COLORREF crColour		- 要滤掉或保留的颜色（缺省值是白色）
 * int mode					- 如果是0则将不拷贝指定的颜色，如果是1则保留指定
 *						      的颜色（缺省值为0）
 * 
 * 返回值：
 * 
 * BOOL						- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 将源位图透明的平铺到本类的指定矩形中
 * 
 * 如果入口参数crArea设为NULL,则平铺矩形等于本身类中图像的尺寸。
 * 该函数不会拉伸或压缩源位图图像。
 * 
 * 注：# 本身类中原来必需已存在位图，且调用之后的位图不会被删除
 *     # 如果源位图类是空的，则返回FALSE
 *	   # 如果本身类中不存在位图，函数将返回FALSE
 * 
 ************************************************************************/


BOOL CBmpProc::CopyTranTile(CBmpProc& souBmp, 
							CRect* crArea, 
							COLORREF crColour, 
							int mode)
{
	// 运行环境检测
	if (!IsValid())
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	if (!souBmp.IsValid())
		return FALSE;

	ASSERT(souBmp.m_pInfo);
	ASSERT(souBmp.m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	// 缺省情况下铺设范围就是本身位图的尺寸
	CRect rect(Rect());

	if (crArea)
		rect = *crArea;	// 另外指定尺寸

	// 矩形坐标规范化
	rect.NormalizeRect();

	if ((rect.Width()==0)||(rect.Height()==0))
		return FALSE;

	// 复制源图
	CDC			compDC;
	CBitmap		*pOldBmp;

	// 创建与当前显示设备兼容的内存设备描述表
	compDC.CreateCompatibleDC(NULL);
	pOldBmp	= compDC.SelectObject((CBitmap*)this);

	// 复制指定尺寸的源位图到目标位图
	souBmp.DrawTranTile(compDC, &rect, crColour, mode);

	compDC.SelectObject(pOldBmp);
	
	return TRUE;
}



/*************************************************************************
 *
 * LoadFromClipboard()
 *
 * 参数说明：无
 * 
 * 
 * 返回值：
 * 
 * BOOL						- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 读取剪贴板中的DIB位图到类中
 * 
 * 如果函数成功，则类中原来的位图（如果有的话）将被删除
 * 
 ************************************************************************/


BOOL CBmpProc::LoadFromClipboard()
{
	// 检查剪贴板的有效性
	if (!PasteAvailable())
		return FALSE;

	// 打开剪贴板
	if (::OpenClipboard(NULL))
	{
		// 获取剪贴板中的DIB数据
		HANDLE h = ::GetClipboardData(CF_DIB);

		if (!h)
		{
			CloseClipboard();
			return FALSE;
		}

		LPSTR lpDIB = (LPSTR)CreateMemoryBlockFromHandle(h);
		if (!lpDIB)
		{	// 读入的剪贴板数据句柄不需要释放，系统会自动释放
			CloseClipboard();
			return FALSE;
		}

		CloseClipboard();

		// 检查DIB信息块的有效性
		if (*((DWORD*)lpDIB) != sizeof(BITMAPINFOHEADER))
		{
			::free((void*)lpDIB);
			return FALSE;
		}

		// 复制信息块
		ASSERT(CalculateDIBInfoSize((LPBITMAPINFOHEADER)lpDIB) >= sizeof(BITMAPINFOHEADER));
		LPBITMAPINFO pInfo = (LPBITMAPINFO)CreateMemoryBlockFromAddress(
			(LPVOID)lpDIB, CalculateDIBInfoSize((LPBITMAPINFOHEADER)lpDIB));
		if (!pInfo)
		{
			::free((void*)lpDIB);
			return FALSE;
		}

		// 计算位数据首地址
		LPSTR pBits = (LPSTR)FindDIBBits(lpDIB);
		ASSERT(pBits);

		// 制作DDB位图
		HBITMAP hBmp;
		HWND hWnd = ::GetDesktopWindow();
		ASSERT(hWnd);
		HDC hDC = ::GetDC(hWnd);

		hBmp = ::CreateDIBitmap(hDC,		// 创建DDB位图。（将DIB位数据
			(BITMAPINFOHEADER *)pInfo,		// 转换为DDB位数据）
			CBM_INIT, pBits, 
			(BITMAPINFO *)pInfo, 
			DIB_RGB_COLORS);

		::ReleaseDC(hWnd, hDC);

		// 创建完逻辑位图后，从剪贴板中读取的数据已无用，释放
		::free((void*)lpDIB);

		if (hBmp)	// 成功
		{
			// 删除原来的图像，并且设置新的位图
			if (!ClearAndSetData(IT_CLIP,0,(LPCTSTR)"",pInfo,hBmp))
			{
				::free((void*)pInfo);
				::DeleteObject(hBmp);
				return FALSE;
			}
			return TRUE;
		}
		else	// 失败
		{
			::free((void*)pInfo);
			return FALSE;
		}
	}

  return FALSE;
}



/*************************************************************************
 *
 * LoadFromFile()
 *
 * 参数说明：
 * 
 * LPCTSTR lpszPathName		- 待读入的位图文件名称（路径）
 * 
 * 返回值：
 * 
 * BOOL						- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 读取指定位图文件到类中
 * 
 * 如果函数成功，则类中原来的位图（如果有的话）将被删除
 * 如果入口参数指定的文件不存在或不是一个位图文件，则函数返回FALSE
 * 
 ************************************************************************/


BOOL CBmpProc::LoadFromFile(LPCTSTR lpszPathName)
{
	CFile file;
	LPBITMAPINFO lpbi;
	LPSTR pBits;
	HBITMAP hBmp;

	if (!file.Open(lpszPathName, CFile::modeRead | CFile::shareDenyWrite))
		return FALSE;

	// 判断文件是否是有效的BMP文件
	if (IsValidBmpFile(file) == FALSE)
	{
		file.Close();
		return FALSE;
	}

	// 读取BMP文件信息
	if ((lpbi=GetBmpFileInfo(file)) == NULL)
	{
		file.Close();
		return FALSE;
	}

	// 本类不支持压缩位图，所以如果是压缩位图则返回FALSE
	if ((lpbi->bmiHeader.biCompression==BI_RLE4)||(lpbi->bmiHeader.biCompression==BI_RLE8))
	{
		::free((void*)lpbi);
		file.Close();
		return FALSE;
	}

	// 提取BMP位数据
	if ((pBits=GetBmpFileBits(file)) == NULL)
	{
		::free((void*)lpbi);
		file.Close();
		return FALSE;
	}


	HWND hWnd = ::GetDesktopWindow();
	ASSERT(hWnd);
	HDC hDC = ::GetDC(hWnd);

	hBmp = ::CreateDIBitmap(hDC,		// 创建DDB位图。（将DIB位数据
		(BITMAPINFOHEADER *)lpbi,		// 转换为DDB位数据）
		CBM_INIT, pBits, 
		(BITMAPINFO *)lpbi, 
		DIB_RGB_COLORS);

	::ReleaseDC(hWnd, hDC);

	// 创建完逻辑位图后，从文件中读取的位数据已无用，释放
	::free((void*)pBits);

	file.Close();

	if (hBmp)	// 成功
	{
		// 删除原来的图像，并且设置新的位图
		if (!ClearAndSetData(IT_DISKFILE,0,lpszPathName,lpbi,hBmp))
		{
			::free((void*)lpbi);
			::DeleteObject(hBmp);
			return FALSE;
		}
		return TRUE;
	}
	else	// 失败
	{
		::free((void*)lpbi);
		return FALSE;
	}
}



/*************************************************************************
 *
 * LoadFromResourceID()
 *
 * 参数说明：
 * 
 * UINT uID					- 待读入的位图资源名称（标识值）
 * HINSTANCE hInstance		- 位图资源所在实例的句柄（如果它被设为NULL，则
 *							  函数将该值设为当前应用程序进程）
 * 
 * 返回值：
 * 
 * BOOL						- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 读取指定位图资源到类中
 * 
 * 如果函数成功，则类中原来的位图（如果有的话）将被删除
 * 
 ************************************************************************/


BOOL CBmpProc::LoadFromResourceID(UINT uID, HINSTANCE hInst) 
{ 
	return LoadFromResourceName((LPCTSTR)MAKEINTRESOURCE(uID), hInst); 
}



/*************************************************************************
 *
 * LoadFromResourceName()
 *
 * 参数说明：
 * 
 * LPCTSTR lpResourceName	- 待读入的位图资源名称（字符串）
 * HINSTANCE hInstance		- 位图资源所在实例的句柄（如果它被设为NULL，则
 *							  函数将该值设为当前应用程序进程）
 * 
 * 返回值：
 * 
 * BOOL						- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 读取指定位图资源到类中
 * 
 * 如果函数成功，则类中原来的位图（如果有的话）将被删除
 * 
 ************************************************************************/


BOOL CBmpProc::LoadFromResourceName(LPCTSTR lpResourceName, 
									HINSTANCE hInstance)
{
	HINSTANCE	hInst = hInstance;
	HBITMAP hBmp = NULL;

	//如果入口参数hInstance是NULL，则函数将用当前应用程序进程的实例句柄
	if (hInst == NULL)
		hInst = ::AfxGetResourceHandle();

	ASSERT(hInst);

	HRSRC hSrc = ::FindResource(hInst, lpResourceName, RT_BITMAP);
	BOOL bSuccess = FALSE;

	if (!hSrc)
		return bSuccess;

	// 由LoadResource()函数读入的资源目标不需要显示的删除它，Windows
	// 系统会自动删除。LockResource()锁定的资源目标也不需要显示的解锁
	HGLOBAL hResData = LoadResource(hInst, hSrc);
	if (hResData)
	{
		LPVOID lpResData = LockResource(hResData);
		if (lpResData)
		{
			// 如果是有效的DIB资源，则资源的首位应该是BITMAPINFOHEADER
			// 结构
			ASSERT(*((DWORD*)lpResData) == sizeof(BITMAPINFOHEADER));

			LPSTR pBits = FindDIBBits((LPSTR)lpResData);
			ASSERT(pBits > (LPSTR)lpResData);

			HWND hWnd = ::GetDesktopWindow();
			ASSERT(hWnd);
			HDC hDC = ::GetDC(hWnd);

			hBmp = CreateDIBitmap(hDC,			// 创建DDB位图。（将DIB位数据
				(BITMAPINFOHEADER *)lpResData,	// 转换为DDB位数据）
				CBM_INIT, pBits, 
				(BITMAPINFO *)lpResData, 
				DIB_RGB_COLORS);

			::ReleaseDC(hWnd, hDC);

			if (hBmp)	// 成功
			{
				DWORD dwInfoSize = (DWORD)((LPSTR)pBits-(LPSTR)lpResData);
				ASSERT(dwInfoSize > (DWORD)(sizeof(BITMAPFILEHEADER)));

				LPBITMAPINFO lpbi = (LPBITMAPINFO)CreateMemoryBlockFromAddress(
					(LPVOID)NULL, dwInfoSize);
				if (lpbi)
				{
					// 复制位图信息块，因为lpResData所指的内存不是运行时态库
					// 函数分配的内存块，所以未用CreateMemoryBlockFromAddress()
					// 的复制功能
					::memcpy((void*)lpbi, (const void*)lpResData, dwInfoSize);
				}
				else
				{
					::DeleteObject(hBmp);
					return FALSE;
				}

				if ((((DWORD)lpResourceName)>>16) == 0)
				{// 如果指定资源时使用的是标识，则串指针的高16位是0
					// 删除原来的图像，并且设置新的位图
					if (!ClearAndSetData(IT_RESOURCE,(DWORD)lpResourceName,(LPCTSTR)"",lpbi,hBmp))
					{
						::free((void*)lpbi);
						::DeleteObject(hBmp);
						return FALSE;
					}
				}
				else
				{
					// 删除原来的图像，并且设置新的位图
					if (!ClearAndSetData(IT_RESOURCE,0, lpResourceName,lpbi,hBmp))
					{
						::free((void*)lpbi);
						::DeleteObject(hBmp);
						return FALSE;
					}
				}
				bSuccess = TRUE;
			}
		}
	}

	return bSuccess;
}



/*************************************************************************
 *
 * LoadFromObject()
 *
 * 参数说明：
 * 
 * CBmpProc& souBmp			- 源位图目标
 * CRect *pSouRect			- 复制矩形
 * 
 * 返回值：
 * 
 * BOOL						- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 读取源位图目标中指定区域（矩形）的图像到类中
 * 
 * 如果入口参数pSouRect设为NULL,则复制源目标的整个位图
 * 入口参数pSouRect必须是一个正常的矩形坐标数据（即左上角坐标小于右下角
 * 坐标）
 * 
 * 如果函数成功，则类中原来的位图（如果有的话）将被删除
 * 
 ************************************************************************/


BOOL CBmpProc::LoadFromObject(CBmpProc& souBmp, CRect *pSouRect)
{
	CRect souRect;

	// 一般情况下，源图必需存在
	ASSERT(souBmp.IsValid());	
	if (souBmp.IsValid() == FALSE)
		return FALSE;

	// 确定要拷贝源位图的哪些部分
	if (pSouRect == NULL)
		souRect = souBmp.Rect();	// 缺省为所有
	else
	{
		CRect sou = souBmp.Rect();
		souRect = *pSouRect;
		// 如果指定的矩形有超出源图的地方，则将其剪裁掉
		if (!sou.PtInRect((POINT)CPoint(souRect.left, souRect.top)))
			souRect.OffsetRect(-souRect.left, -souRect.top);

		if (!sou.PtInRect((POINT)CPoint(souRect.right, souRect.bottom)))
		{
			if (souRect.right > sou.right)
				souRect.right = sou.right;
			if (souRect.bottom > sou.bottom)
				souRect.bottom = sou.bottom;
		}
	}

	ASSERT(souBmp.m_pInfo);
	LPBITMAPINFO lpSouInfo = souBmp.m_pInfo;

	// 复制源图像的信息块
	LPBITMAPINFO pInfo = (LPBITMAPINFO)CreateMemoryBlockFromAddress(
		(LPVOID)lpSouInfo);
	if (!pInfo)
		return FALSE;

	// 确定源图信息块的有效性
	ASSERT(pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	// 修改新信息块中的数据
	pInfo->bmiHeader.biWidth = souRect.Width();
	pInfo->bmiHeader.biHeight = souRect.Height();
	pInfo->bmiHeader.biSizeImage = CalculateDIBitsSize((LPBITMAPINFOHEADER)pInfo);

	CBitmap		tempBmp;
	HBITMAP		hBmp;

	HWND hWnd = ::GetDesktopWindow();
	ASSERT(hWnd);
	HDC hDC = ::GetDC(hWnd);
	CDC dc;
	dc.Attach(hDC);

	if (tempBmp.CreateCompatibleBitmap(&dc, souRect.Width(), souRect.Height()))
	{
		CDC			compDC, compDC2;
		CBitmap		*pOldBmp, *pOldBmp2;

		// 创建与当前显示设备兼容的内存设备描述表
		compDC.CreateCompatibleDC(NULL);
		compDC2.CreateCompatibleDC(NULL);

		pOldBmp	= compDC.SelectObject((CBitmap*)&tempBmp);
		pOldBmp2= compDC2.SelectObject((CBitmap*)&souBmp);

		// 复制指定尺寸的源位图到目标位图
		compDC.BitBlt(0, 0, souRect.Width(), souRect.Height(), 
					&compDC2, souRect.left, souRect.top, SRCCOPY);

		compDC2.SelectObject(pOldBmp2);
		compDC.SelectObject(pOldBmp);

		::ReleaseDC(hWnd, dc.Detach());

		hBmp = (HBITMAP)tempBmp.Detach();
		// 如果分离操作失败，返回FALSE
		ASSERT(hBmp);
		if (!hBmp)
		{
			::free((void*)pInfo);
			return FALSE;
		}

		// 删除原来的图像，并且设置新的位图
		if (!ClearAndSetData(IT_MEMORY,0,(LPCTSTR)"",pInfo,hBmp))
		{
			::free((void*)pInfo);
			::DeleteObject(hBmp);
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		::ReleaseDC(hWnd, dc.Detach());
		::free((void*)pInfo);

		return FALSE;
	}
}



/*************************************************************************
 *
 * LoadFromHbmp()
 *
 * 参数说明：
 * 
 * HBITMAP sou				- 源位图句柄（不能为NULL）
 * CRect *pSouRect			- 复制矩形
 * 
 * 返回值：
 * 
 * BOOL						- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 读取源位图句柄中指定矩形的图像到类中
 * 
 * 如果入口参数pSouRect设为NULL,则复制源目标的整个位图
 * 入口参数pSouRect必须是一个正常的矩形坐标数据（即左上角坐标小于右下角
 * 坐标）
 * 入口参数指定的位图句柄无论成功与否都不会受影响
 * 
 * 如果函数成功，则类中原来的位图（如果有的话）将被删除
 * 
 ************************************************************************/


BOOL CBmpProc::LoadFromHbmp(HBITMAP sou, CRect *pSouRect)
{
	// 检查入口参数
	if (!sou)
		return FALSE;

	CBmpProc tmp;

	// 将句柄贴入临时类中
	if (!tmp.Attach(sou))
		return FALSE;

	BOOL ret = LoadFromObject(tmp, pSouRect);

	tmp.Detach();

	return ret;
}



/*************************************************************************
 *
 * CalculateDIBFileSize()
 *
 * 参数说明：
 *
 * LPBITMAPINFOHEADER lpbi	- 指向内存DIB的BITMAPINFOHEADER结构
 *
 * 返回值：
 *
 * DWORD					- 返回DIB文件的尺寸
 *
 * 描述：
 *
 * 计算具有指定信息格式的DIB位图的尺寸（以字节为单位）
 *
 ************************************************************************/


DWORD CBmpProc::CalculateDIBFileSize(LPBITMAPINFOHEADER lpbi) const
{
	ASSERT(lpbi);
	ASSERT(lpbi->biSize == sizeof(BITMAPINFOHEADER));

	DWORD filesize;
	DWORD dwScanWidth = DIBSCANLINE_WIDTHBYTES(DIBWidth(lpbi)*GetBitsPerPixel(lpbi));
	DWORD dwPaletteSize = (DWORD)PaletteSize(lpbi);

	// 文件头尺寸＋信息头尺寸＋颜色表尺寸＋位数据尺寸
	filesize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 
		dwPaletteSize + dwScanWidth*DIBHeight(lpbi);

	return filesize;
}



/*************************************************************************
 *
 * CalculateDIBSize()
 *
 * 参数说明：
 *
 * LPBITMAPINFOHEADER lpbi	- 指向内存DIB的BITMAPINFOHEADER结构
 *
 * 返回值：
 *
 * DWORD					- 返回DIB内部形式的尺寸
 *
 * 描述：
 *
 * 计算具有指定信息格式的DIB内部形式的尺寸（以字节为单位）
 *
 * 内部形式是指除去文件头结构的DIB文件内容在内存中的实现
 *
 ************************************************************************/


DWORD CBmpProc::CalculateDIBSize(LPBITMAPINFOHEADER lpbi) const
{
	ASSERT(lpbi);
	ASSERT(lpbi->biSize == sizeof(BITMAPINFOHEADER));

	DWORD dwScanWidth = DIBSCANLINE_WIDTHBYTES(DIBWidth(lpbi)*GetBitsPerPixel(lpbi));
	DWORD dwPaletteSize = (DWORD)PaletteSize(lpbi);

	// 信息头尺寸＋颜色表尺寸＋位数据尺寸
	return sizeof(BITMAPINFOHEADER)+dwPaletteSize+dwScanWidth*DIBHeight(lpbi);
}



/*************************************************************************
 *
 * CalculateDIBitsOff()
 *
 * 参数说明：
 *
 * LPBITMAPINFOHEADER lpbi	- 指向内存DIB的BITMAPINFOHEADER结构
 *
 * 返回值：
 *
 * DWORD					- 返回在DIB文件中位的偏移
 *
 * 描述：
 *
 * 计算具有指定信息格式的DIB位图中位的偏移（以字节为单位）
 *
 ************************************************************************/


DWORD CBmpProc::CalculateDIBitsOff(LPBITMAPINFOHEADER lpbi) const
{
	DWORD bitsoff;

	ASSERT(lpbi);
	ASSERT(lpbi->biSize == sizeof(BITMAPINFOHEADER));

	DWORD dwPaletteSize = (DWORD)PaletteSize(lpbi);

	// 文件头尺寸＋信息头尺寸＋颜色表尺寸
	bitsoff = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + \
		dwPaletteSize;

	return bitsoff;
}



/*************************************************************************
 *
 * CalculateDIBInfoSize()
 *
 * 参数说明：
 *
 * LPBITMAPINFOHEADER lpbi	- 指向内存DIB的BITMAPINFOHEADER结构
 *
 * 返回值：
 *
 * DWORD					- 返回在DIB文件中信息块的长度
 *
 * 描述：
 *
 * 计算具有指定信息格式的DIB位图中信息块的长度（以字节为单位）
 *
 * 信息块是指BITMAPINFO结构。注意，其中含有颜色表!
 *
 ************************************************************************/


DWORD CBmpProc::CalculateDIBInfoSize(LPBITMAPINFOHEADER lpbi) const
{
	ASSERT(lpbi);
	ASSERT(lpbi->biSize == sizeof(BITMAPINFOHEADER));
	// 信息头尺寸＋颜色表尺寸
	return sizeof(BITMAPINFOHEADER)+(DWORD)PaletteSize(lpbi);
}



/*************************************************************************
 *
 * CalculateDIBitsSize()
 *
 * 参数说明：
 *
 * LPBITMAPINFOHEADER lpbi	- 指向内存DIB的BITMAPINFOHEADER结构
 *
 * 返回值：
 *
 * DWORD					- 返回在DIB文件中位的尺寸
 *
 * 描述：
 *
 * 计算具有指定信息格式的DIB位图中位的（以字节为单位）
 *
 ************************************************************************/


DWORD CBmpProc::CalculateDIBitsSize(LPBITMAPINFOHEADER lpbi) const
{
	ASSERT(lpbi);
	ASSERT(lpbi->biSize == sizeof(BITMAPINFOHEADER));

	DWORD s = DIBSCANLINE_WIDTHBYTES(DIBWidth(lpbi)*GetBitsPerPixel(lpbi));

	return s*DIBHeight(lpbi);
}



/*************************************************************************
 *
 * IsValidBmpFile()
 *
 * 参数说明：
 *
 * CFile& file      - 待判断的文件（文件必须已被打开）
 *
 * 返回值：
 *
 * BOOL             - 返回TRUE表明文件是有效的BMP文件，否则返回FALSE
 *
 * 描述：
 *
 * 判断指定的文件是否是一个有效的BMP文件
 *
 * 注：它不支持OS/2系统产生的BMP文件，如果是这种文件，它将返回FALSE
 *
 ************************************************************************/


BOOL CBmpProc::IsValidBmpFile(CFile& file)
{
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;
	DWORD dwSize, dwCurPos;
	UINT uRet, uRet2;

	/* 获取DIB文件的长度（以字节为单位）*/
	dwSize = file.GetLength();
	if (dwSize < (sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)))
		return FALSE;

	/* 读取DIB的文件头结构，并检查它的有效性 */
	dwCurPos = file.GetPosition();
	file.SeekToBegin();
	uRet  = file.Read((LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER));
	uRet2 = file.Read((LPSTR)&bmiHeader, sizeof(BITMAPINFOHEADER));
	file.Seek(dwCurPos, CFile::begin);

	if (uRet != sizeof(BITMAPFILEHEADER))
		return FALSE;
	if (bmfHeader.bfType != DIB_HEADER_MARKER) //((WORD) ('M' << 8) | 'B')
		return FALSE;

	if (uRet2 != sizeof(BITMAPINFOHEADER))
		return FALSE;
	// 它不支持OS/2格式的位图
	if (bmiHeader.biSize != sizeof(BITMAPINFOHEADER))
		return FALSE;

	int size = PaletteSize(&bmiHeader);

	if (bmfHeader.bfOffBits != \
		(sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+size))
		return FALSE;

	return TRUE;	// TRUE表明文件是有效的BMP文件
}



/*************************************************************************
 *
 * GetBmpFileInfo()
 *
 * 参数说明：
 *
 * CFile& file      - 待取得信息的位图文件（文件必须已被打开）
 *
 * 返回值：
 *
 * LPBITMAPINFO		- 返回位图信息块地址，如果失败返回NULL
 *
 * 描述：
 *
 * 获取位图文件的图像信息，并返回信息结构的地址
 *
 * 注：位图信息块就是一块内存。调用本函数前应检查位图文件的有效性
 *
 ************************************************************************/


LPBITMAPINFO CBmpProc::GetBmpFileInfo(CFile& file)
{
	LPBITMAPINFO pDIB;
	BITMAPFILEHEADER bmfHeader;
	DWORD dwCurPos;
	UINT uRet, uDibInfoSize;

	/* 读取DIB的文件头结构 */
	dwCurPos = file.GetPosition();
	file.SeekToBegin();
	uRet  = file.Read((LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER));
	file.Seek(dwCurPos, CFile::begin);

	if (uRet != sizeof(BITMAPFILEHEADER))
		return NULL;

	uDibInfoSize = bmfHeader.bfOffBits-sizeof(BITMAPFILEHEADER);

	// 分配内存，用于存放信息头及颜色表（如果有）
	pDIB = (LPBITMAPINFO)CreateMemoryBlockFromAddress((LPVOID)NULL, 
		(DWORD)uDibInfoSize);
	if (!pDIB)
		return NULL;

	// 读取数据
	dwCurPos = file.GetPosition();
	file.Seek((LONG)sizeof(BITMAPFILEHEADER), CFile::begin);
	uRet = file.Read((void *)pDIB, uDibInfoSize);
	file.Seek(dwCurPos, CFile::begin);

	if (uRet != uDibInfoSize)
	{
		::free((void*)pDIB);
		return NULL;
	}

	return pDIB;
}



/*************************************************************************
 *
 * GetBmpFileBits()
 *
 * 参数说明：
 *
 * CFile& file      - 待取得位数据的位图文件（文件必须已被打开）
 *
 * 返回值：
 *
 * LPSTR			- 返回位图位信息的内存块地址，如果失败返回NULL
 *
 * 描述：
 *
 * 获取位图文件的位数据，并返回它的地址
 *
 * 注：调用本函数前应检查该文件的有效性
 *
 ************************************************************************/


LPSTR CBmpProc::GetBmpFileBits(CFile& file)
{
	BITMAPFILEHEADER bmfHeader;
	LPSTR pBits;
	DWORD dwSize, dwRet, dwCurPos;
	UINT uRet;

	/* 读取DIB的文件头结构 */
	dwCurPos = file.GetPosition();
	file.SeekToBegin();
	uRet  = file.Read((LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER));
	file.Seek(dwCurPos, CFile::begin);

	if (uRet != sizeof(BITMAPFILEHEADER))
		return NULL;

	// 获取文件长度
	dwSize = file.GetLength();

	// 分配内存，用于存放位数据
	pBits = (LPSTR)CreateMemoryBlockFromAddress((LPVOID)NULL, 
		(DWORD)(dwSize-bmfHeader.bfOffBits));
	if (!pBits)
		return NULL;

	// 读取位数据
	dwCurPos = file.GetPosition();
	file.Seek(bmfHeader.bfOffBits, CFile::begin);
	dwRet = file.ReadHuge((void*)pBits, (dwSize-bmfHeader.bfOffBits));
	file.Seek(dwCurPos, CFile::begin);

	if (dwRet != (dwSize-bmfHeader.bfOffBits))
	{
		::free(pBits);
		return NULL;
	}

	return pBits;
}



/*************************************************************************
 *
 * GetSpecifyFormatInfo()
 *
 * 参数说明：
 *
 * WORD newBitCount			- 新指定的格式（即每像素占多少位，可以指定0）
 *
 * 返回值：
 *
 * LPBITMAPINFO				- 如果成功返回新的信息块地址，如果失败返回NULL
 *
 * 描述：
 *
 * 根据类中的信息块和指定的新格式创建一个新的信息块
 *
 * 注：位图信息块就是一块内存。
 *     当不再需要这个内存块时，应释放（用free()函数）
 *     该函数并不影响类中的数据
 *     如果原来类中没有位图，函数返回NULL
 *     如果入口参数指定0值，则函数返回类中已存在的信息块的一个副本
 *
 ************************************************************************/


LPBITMAPINFO CBmpProc::GetSpecifyFormatInfo(WORD newBitCount)
{
	// 如果本身类中原来没有位图，函数返回NULL
	if (!IsValid())
		return NULL;

	ASSERT(m_pInfo);
	ASSERT(m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	int infosize;
	HWND hWnd = ::GetDesktopWindow();
	ASSERT(hWnd);
	HDC hDC;
	BITMAPINFOHEADER bmi;
	LPBITMAPINFO pInfo;
	LPSTR pBits;

	// 如果没有指定新的格式（即新的位数指定了0），返回类中信息块的副本
	if (newBitCount == 0)
	{
		pInfo = (LPBITMAPINFO)CreateMemoryBlockFromAddress((LPVOID)m_pInfo);
		return pInfo;
	}

	// 检测位数合法性
	if (!IsValidDibFormat(newBitCount))
		return NULL;

	::memcpy((void*)&bmi, (const void*)m_pInfo, sizeof(BITMAPINFOHEADER));
	ASSERT(bmi.biSize == sizeof(BITMAPINFOHEADER));

	bmi.biBitCount = newBitCount;

	if ((bmi.biBitCount==16)||(bmi.biBitCount==32))
		bmi.biCompression = BI_BITFIELDS;
	else
		bmi.biCompression = BI_RGB;

	bmi.biSizeImage = CalculateDIBitsSize(&bmi);

	infosize = CalculateDIBInfoSize(&bmi);
	pInfo = (LPBITMAPINFO)CreateMemoryBlockFromAddress((LPVOID)NULL, 
		(DWORD)infosize);
	if (!pInfo)
		return NULL;

	::memcpy((void*)pInfo, (const void*)&bmi, sizeof(BITMAPINFOHEADER));
	ASSERT(pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	pBits = (LPSTR)CreateMemoryBlockFromAddress((LPVOID)NULL, 
		CalculateDIBitsSize((LPBITMAPINFOHEADER)pInfo));
	if (!pBits)
	{
		::free((void*)pInfo);
		return NULL;
	}

	// 获取DIB颜色表数据
	hDC = ::GetDC(hWnd);
	if (!::GetDIBits(hDC,(HBITMAP)m_hObject,0,pInfo->bmiHeader.biHeight,\
			(LPVOID)pBits,pInfo,DIB_RGB_COLORS))
	{
		::free((void*)pInfo);
		::free((void*)pBits);
		::ReleaseDC(hWnd, hDC);
		return NULL;
	}
	::ReleaseDC(hWnd, hDC);

	// 位数据已无用，释放
	::free((void*)pBits);

	return pInfo;
}



/****************************** DIB Function ****************************/

/*
 DIB 函数注释：

  # 不支持OS/2操作系统产生的DIB位图
  # 不支持压缩位图（RLE压缩编码）
  # FindDIBBits()的入口参数lpbi是指向包括信息头、颜色表、位数据的内存块
  # 其它函数的入口参数lpbi是指向信息块，即BITMAPINFOHEADER结构
  # 函数的入口参数lpbi中的数据必需是有效的!

*/

/*************************************************************************
 *
 * FindDIBBits()
 *
 * 参数说明：
 *
 * LPSTR lpbi       - 指向内存DIB的首地址
 *
 * 返回值：
 *
 * LPSTR            - 指向DIB位数据的地址
 *
 * 描述：
 *
 * 这个函数计算DIB的位数据地址，并将它返回。
 *
 ************************************************************************/


LPSTR CBmpProc::FindDIBBits(LPSTR lpbi) const
{
	ASSERT(lpbi);
	ASSERT(((LPBITMAPINFOHEADER)lpbi)->biSize == sizeof(BITMAPINFOHEADER));
	return (lpbi + *(LPDWORD)lpbi + PaletteSize((LPBITMAPINFOHEADER)lpbi));
}



/*************************************************************************
 *
 * DIBWidth()
 *
 * 参数说明：
 *
 * LPBITMAPINFOHEADER lpbi	- 指向内存DIB的BITMAPINFOHEADER 结构
 *
 * 返回值：
 *
 * DWORD				- DIB的宽度
 *
 * 描述：
 *
 * 获取DIB的宽度。（像素）
 *
 * 它从BITMAPINFOHEADER结构的biWidth成员提取宽度值。
 *
 ************************************************************************/


DWORD CBmpProc::DIBWidth(LPBITMAPINFOHEADER lpbi) const
{
	ASSERT(lpbi);
	ASSERT(lpbi->biSize == sizeof(BITMAPINFOHEADER));
	return 	lpbi->biWidth;
}



/*************************************************************************
 *
 * DIBHeight()
 *
 * 参数说明：
 *
 * LPBITMAPINFOHEADER lpbi	- 指向内存DIB的BITMAPINFOHEADER结构
 *
 * 返回值：
 *
 * DWORD					- DIB的高度
 *
 * 描述：
 *
 * 获取DIB的高度。（像素）
 *
 * 它从BITMAPINFOHEADER结构的biHeight成员提取高度值。
 *
 ************************************************************************/


DWORD CBmpProc::DIBHeight(LPBITMAPINFOHEADER lpbi) const
{
	ASSERT(lpbi);
	ASSERT(lpbi->biSize == sizeof(BITMAPINFOHEADER));
	return 	lpbi->biHeight;
}



/*************************************************************************
 *
 * PaletteSize()
 *
 * 参数说明：
 *
 * LPBITMAPINFOHEADER lpbi	- 指向内存DIB的BITMAPINFOHEADER结构
 *
 * 返回值：
 *
 * WORD						- DIB调色板的尺寸（以字节为单位）
 *
 * 描述：
 *
 * 获取指定DIB的调色板尺寸（以字节为单位）
 *
 * 这个函数先获取DIB的颜色个数，然后用颜色个数值乘与RGBQUAD的结构尺寸。
 *
 ************************************************************************/


WORD CBmpProc::PaletteSize(LPBITMAPINFOHEADER lpbi) const
{
	WORD size, wBitCount;

	ASSERT(lpbi);
	ASSERT(lpbi->biSize == sizeof(BITMAPINFOHEADER));
	// 获得每个像素所占的位数
	wBitCount = GetBitsPerPixel(lpbi);
				
	// 16位和32位位图在颜色表中占用三个DWORD数值来表示
	// 红、绿、蓝在位数据中的掩码
	if ((wBitCount == 16)||(wBitCount == 32))
	{
		ASSERT(lpbi->biCompression == BI_BITFIELDS);
		size = sizeof(DWORD)*3;
	}
	else
	{
		// 不支持压缩位图
		ASSERT(lpbi->biCompression == BI_RGB);
		size = (WORD)(DIBNumColors(lpbi) * sizeof(RGBQUAD));
	}

	return size;
}



/*************************************************************************
 *
 * DIBNumColors()
 *
 * 参数说明：
 *
 * LPBITMAPINFOHEADER lpbi	- 指向内存DIB的BITMAPINFOHEADER结构
 *
 * 返回值：
 *
 * WORD						- 颜色表中颜色的个数
 *
 * 描述：
 *
 * 获取DIB颜色表中的颜色个数。
 * 
 * 这个函数通过DIB中每个像素所占的位数来计算它的颜色表中颜色的个数。比如
 * 一个像素占用一位，则有两种颜色，占用四位，则有16种颜色。占用8位，则有
 * 256种颜色，而占用16或更多位，则颜色表中就没有颜色数据了。
 *
 * （注意：16位和32位位图的颜色表并不是空的，其中有三个DWORD型的数值，用
 * 以表示红、绿、蓝在位数据中的掩码，而24位位图的颜色表则是空的，即颜色
 * 表的尺寸为0，但本函数并不将掩码作为颜色，所以如果是16位和32位位图，本
 * 函数仍将返回0。如果位图信息中biClrUsed成员不是0，本函数将使用这个值，而
 * 不去计算）
 ************************************************************************/

WORD CBmpProc::DIBNumColors(LPBITMAPINFOHEADER lpbi) const
{
	ASSERT(lpbi);
	ASSERT(lpbi->biSize == sizeof(BITMAPINFOHEADER));
	WORD wBitCount = GetBitsPerPixel(lpbi);  // DIB位计数

	// 如果位图指定了颜色表中颜色的个数，则直接使用这个值
	if (lpbi->biClrUsed)
		return (WORD)(lpbi->biClrUsed);

	/*  通过DIB中每个像素所占的位数来计算它的颜色表中颜色的个数 */
	switch (wBitCount)
	{
		case 1:
			return 2;		// 单色位图，只有黑白两种颜色
		case 4:
			return 16;		// 标准VGA位图，有16种颜色
		case 8:
			return 256;		// SVGA位图，有256种颜色
		case	16:			// 64K色位图
		case	24:			// 16M色位图（真彩色）
		case	32:			// 16M+色位图（真彩色）
			return 0;		// 颜色表中没有颜色数据返回0
		default:
			ASSERT(FALSE);
			return 0;
	}
}



/*************************************************************************
 *
 * CreateMemoryBlockFromAddress()
 *
 * 参数说明：
 *
 * LPVOID pSou		- 待复制的内存块地址（可以是NULL，参见注释）
 * DWORD dwCount	- 复制范围（即复制字节的个数，可以是0，参见注释）
 *
 * 返回值：
 *
 * LPVOID           - 新的内存块地址，如果失败，返回NULL
 *
 * 描述：
 *
 * 复制内存块。（也可用于创建内存块）
 *
 * 该函数先创建一个长度为dwCount的内存，并将新创建的内存初始化为0，然后
 * 复制pSou中长度为dwCount的内容到新的内存块。如果pSou为NULL，则不复制
 *
 * 如果入口参数dwCount被指定为0，则函数将复制整个原地址的内容，新内存块
 * 的尺寸将与源地址内存块的尺寸相同
 * 入口参数pSou和dwCount不能同时为0，否则函数将返回NULL
 * 
 * 函数返回的内存块不再使用时，应调用free()函数将其释放
 *
 * pSou参数指定的内存块必须是由malloc(),calloc(),realloc()函数分配的!!!
 * 
 ************************************************************************/


LPVOID CBmpProc::CreateMemoryBlockFromAddress(LPVOID pSou, DWORD dwCount)
{
	// 入口参数pSou和dwCount不能同时为0
	if ((pSou==NULL)&&(dwCount==0))
		return NULL;

	LPSTR lpt;

	// 只创建，不复制
	if (pSou == NULL)
	{
		ASSERT(dwCount);
		lpt = (LPSTR)::malloc((size_t)dwCount);
		if (!lpt)
			return NULL;

		::memset((void *)lpt, 0, dwCount);

		return lpt;
	}

	DWORD dwCopyNum, dwAllocNum;

	if (dwCount == 0)
	{
		dwCopyNum = (DWORD)::_msize((void*)pSou);
		dwAllocNum = dwCopyNum;
	}
	else
	{
		// 如果pSou内存块的尺寸小于dwCount指定的值，则将pSou中的内容
		// 全部复制到新的内存块中，否则只复制dwCount长度的内容
		dwCopyNum = min((DWORD)::_msize((void*)pSou), dwCount);
		dwAllocNum = dwCount;
	}

	lpt = (LPSTR)::malloc((size_t)dwAllocNum);

	if (!lpt)
		return NULL;

	// 初始化新内存块
	::memset((void *)lpt, 0, dwAllocNum);

	// 复制源内存中的内容到新的内存中
	::memcpy((void*)lpt, (const void*)pSou, (size_t)dwCopyNum);

	return (LPVOID)lpt;
}



/*************************************************************************
 *
 * CreateMemoryBlockFromHandle()
 *
 * 参数说明：
 *
 * HANDLE handle	- 待复制的内存句柄（可以是NULL，参见注释）
 * DWORD dwCount	- 新创内存块的尺寸（可以是0，参见注释）
 *
 * 返回值：
 *
 * LPVOID           - 新的内存块地址，如果失败，返回NULL
 *
 * 描述：
 *
 * 复制指定尺寸的内存句柄内容到一个新的内存块中（也可用于创建内存块）
 *
 * 注：
 *
 * 该函数先创建一个指定长度的内存块，并将新创建的内存初始化为0，然后
 * 复制句柄handle中指定大小的内容到新的内存块中。
 *
 * 如果入口参数handle被指定为NULL，则函数只创建内存，不复制
 * 如果入口参数dwCount被指定为0，则函数将复制整个原句柄的内容，新内存块
 * 的尺寸将与源句柄的尺寸相同
 * 入口参数handle和dwCount不能同时为0，否则函数将返回NULL
 *
 * 函数返回的内存块不再使用时，应调用free()函数将其释放
 *
 ************************************************************************/


LPVOID CBmpProc::CreateMemoryBlockFromHandle(HANDLE handle, DWORD dwCount)
{
	// 入口参数handle和dwCount不能同时为0
	if ((handle==NULL)&&(dwCount==0))
		return NULL;

	LPSTR lpt;

	// 只创建，不复制
	if (handle == NULL)
	{
		ASSERT(dwCount);
		lpt = (LPSTR)::malloc((size_t)dwCount);
		if (!lpt)
			return NULL;

		::memset((void *)lpt, 0, dwCount);

		return lpt;
	}

	DWORD dwCopyNum, dwAllocNum;

	if (dwCount == 0)
	{
		dwCopyNum = ::GlobalSize((HGLOBAL)handle);
		dwAllocNum = dwCopyNum;
	}
	else
	{
		// 如果指定的尺寸值大于句柄尺寸，则使用句柄尺寸值
		dwCopyNum = min(::GlobalSize((HGLOBAL)handle), dwCount);
		dwAllocNum = dwCount;
	}

	lpt = (LPSTR)::malloc((size_t)dwAllocNum);

	if (!lpt)
		return NULL;

	::memset((void *)lpt, 0, dwAllocNum);

	// 复制内存句柄的内容到新的内存块中
	void* lp = ::GlobalLock((HGLOBAL)handle);
	if (!lp)
	{
		::free((void*)lpt);
		return NULL;
	}
	::memcpy((void*)lpt, (const void*)lp, (size_t)dwCopyNum);
	::GlobalUnlock(handle);

	return (LPVOID)lpt;
}



/*************************************************************************
 *
 * GetBitsPerPixel()
 * 
 * 参数说明：
 *
 * LPBITMAPINFOHEADER lpbi	- 指向内存DIB的BITMAPINFOHEADER结构
 *
 * 返回值：
 *
 * int						- 返回每个像素所占的位数。
 *
 * 描述：
 *
 * 获取指定DIB中每个像素所占的位数。
 *
 ************************************************************************/


int	CBmpProc::GetBitsPerPixel(LPBITMAPINFOHEADER lpbi) const
{
	ASSERT(lpbi);
	ASSERT(lpbi->biSize == sizeof(BITMAPINFOHEADER));
	return lpbi->biBitCount;  // DIB位计数
}






/***************************** 内部访问函数 **************************/

/*************************************************************************
 *
 * Size()
 * 
 * 参数说明：无
 *
 *
 * 返回值：
 *
 * CSize				- 返回包含图像尺寸的CSize目标，如果失败，CSize目标
 *						  中的宽度和高度都是0
 *
 * 描述：
 *
 * 获取类中图像的宽度和高度（以像素为单位）
 *
 ************************************************************************/


CSize CBmpProc::Size() const
{ 
	if (m_mark == FALSE)
		return CSize(0,0);

	ASSERT(m_pInfo);
	ASSERT(DIBWidth((LPBITMAPINFOHEADER)m_pInfo));	// 宽度和高度不能为0
	ASSERT(DIBHeight((LPBITMAPINFOHEADER)m_pInfo));

	return CSize(DIBWidth((LPBITMAPINFOHEADER)m_pInfo), DIBHeight((LPBITMAPINFOHEADER)m_pInfo));
}



/*************************************************************************
 *
 * Rect()
 * 
 * 参数说明：无
 *
 *
 * 返回值：
 *
 * CRect				- 返回包含图像尺寸的CRect目标，如果失败，CRect目标
 *						  中的数据均是0
 *
 * 描述：
 *
 * 获取类中图像的尺寸数据（以像素为单位）
 *
 ************************************************************************/


CRect CBmpProc::Rect() const
{ 
	if (m_mark == FALSE)
		return CRect(0,0,0,0);

	ASSERT(m_pInfo);

	return CRect(CPoint(0, 0), Size()); 
}



/*************************************************************************
 *
 * Width()
 * 
 * 参数说明：无
 *
 *
 * 返回值：
 *
 * int					- 返回图像的宽度，如果失败返回0
 *
 * 描述：
 *
 * 获取类中图像的宽度数据（以像素为单位）
 *
 ************************************************************************/


int CBmpProc::Width() const
{
	if (m_mark == FALSE)
		return 0;

	ASSERT(m_pInfo);

	return (int)DIBWidth((LPBITMAPINFOHEADER)m_pInfo);
}



/*************************************************************************
 *
 * Height()
 * 
 * 参数说明：无
 *
 *
 * 返回值：
 *
 * int					- 返回图像的高度，如果失败返回0
 *
 * 描述：
 *
 * 获取类中图像的高度数据（以像素为单位）
 *
 ************************************************************************/


int CBmpProc::Height() const
{ 
	if (m_mark == FALSE)
		return 0;

	ASSERT(m_pInfo);

	return (int)DIBHeight((LPBITMAPINFOHEADER)m_pInfo);
}



/*************************************************************************
 *
 * ScanWidth()
 * 
 * 参数说明：无
 *
 *
 * 返回值：
 *
 * int					- 返回图像每一扫描行的宽度，如果失败返回0
 *
 * 描述：
 *
 * 获取类中图像的扫描行宽度（以字节为单位）
 *
 * 注：DIB位图扫描行是DWORD型对齐的，DDB位图扫描行是WORD对齐的
 *
 ************************************************************************/


int CBmpProc::ScanWidth() const
{ 
	if (m_mark == FALSE)
		return 0;

	ASSERT(m_pInfo);

	return DIBSCANLINE_WIDTHBYTES(Width()*GetBitsPerPixel((LPBITMAPINFOHEADER)m_pInfo));
}



/*************************************************************************
 *
 * BitsPerPixel()
 * 
 * 参数说明：无
 *
 *
 * 返回值：
 *
 * int					- 返回图像中每个像素所占的位数，如果失败返回0
 *
 * 描述：
 *
 * 获取类中图像每个像素所占的位的个数，比如16色位图每个像素占4位
 *
 ************************************************************************/


int CBmpProc::BitsPerPixel() const
{ 
	if (m_mark == FALSE)
		return 0;

	ASSERT(m_pInfo);

	return GetBitsPerPixel((LPBITMAPINFOHEADER)m_pInfo);
}



/*************************************************************************
 *
 * IsValid()
 * 
 * 参数说明：无
 *
 *
 * 返回值：
 *
 * BOOL					- 如果类中存在位图则返回TRUE，否则返回FALSE
 *
 * 描述：
 *
 * 判断类中是否存在有效的位图
 *
 ************************************************************************/


BOOL  CBmpProc::IsValid() const
{ 
	return m_mark;
}



/*************************************************************************
 *
 * GetImageType()
 * 
 * 参数说明：无
 *
 *
 * 返回值：
 *
 * int					- 图像的类型编码
 *
 * 描述：
 *
 * 获取类中位图的来源类型编码
 * 
 * 注：返回的数值可用CBmpProc::IT_MEMORY等类似的方法来判别，它可能是下列
 *     值之一：
 *
 * 		CBmpProc::IT_NULL,		— 无位图类型	0   （类中没有位图）
 *		CBmpProc::IT_DISKFILE,	— 文件位图		1
 *		CBmpProc::IT_RESOURCE,	— 资源位图		2
 *		CBmpProc::IT_MEMORY,	— 内存位图		3
 *		CBmpProc::IT_CLIP,		— 剪贴板位图	4
 *		CBmpProc::IT_CREATE		— 创建型位图	5
 *
 ************************************************************************/


int CBmpProc::GetImageType() const
{ 
	return (int)m_type; 
}



/*************************************************************************
 *
 * GetInAddInfo()
 * 
 * 参数说明：无
 *
 *
 * 返回值：
 *
 * DWORD				- 附加信息内容
 *
 * 描述：
 *
 * 获取类中位图的附加信息
 * 
 * 注：附加信息的内容会随位图来源类型的不同而不同，当位图来源编码等于
 *     CBmpProc::IT_RESOURCE，并且使用的是标识，则附加信息就等于这个
 *     标识的值，其它情况下它是0（没有意义）
 *
 ************************************************************************/


DWORD CBmpProc::GetInAddInfo() const
{ 
	return m_addInfo; 
}



/*************************************************************************
 *
 * GetFileName()
 * 
 * 参数说明：无
 *
 *
 * 返回值：
 *
 * const CString&		- 文件名存放类的引用
 *
 * 描述：
 *
 * 获取类中位图的文件名称
 * 
 * 注：不要用本函数的返回值来改变文件名称！（但可使用SetFileName()函数）
 *
 ************************************************************************/


const CString& CBmpProc::GetFileName() const
{ 
	return m_cFileName; 
}



/*************************************************************************
 *
 * SetFileName()
 * 
 * 参数说明：
 *
 * LPSTR pSou			- 新的文件名称
 *
 * 返回值：
 *
 * BOOL					- 如果设置成功返回TRUE，否则返回FALSE
 *
 * 描述：
 *
 * 重新设置位图的文件名称
 * 如果原来类中没有位图，函数执行失败
 * 
 ************************************************************************/


BOOL CBmpProc::SetFileName(LPSTR pSou)
{
	if (m_mark == FALSE)
		return FALSE;

	ASSERT(pSou);

	m_cFileName = pSou; 

	return TRUE;
}



/*************************************************************************
 *
 * GetObject()
 * 
 * 参数说明：
 *
 * LPBITMAP lpBitmap	- 接收信息数据的结构变量地址
 *
 * 返回值：
 *
 * int					- 如果成功返回以获取的字节个数，如果失败，返回0
 * 
 * 描述：
 *
 * 获取位图的BITMAP信息
 *
 * 注：# 函数成功返回之后，BITMAP结构中只有width,height及颜色格式信息有效
 *     # 如果类中原来没有位图，函数失败，返回0
 * 
 ************************************************************************/


int CBmpProc::GetObject(LPBITMAP lpBitmap) const
{ 
	if (m_mark == FALSE)
		return 0;

	ASSERT(m_hObject);
	ASSERT(lpBitmap);

	return CGdiObject::GetObject(sizeof(BITMAP), (LPVOID)lpBitmap); 
}



/*************************************************************************
 *
 * GetVersion()
 * 
 * 参数说明：无
 *
 *
 * 返回值：
 *
 * WORD					- 版本号
 * 
 * 描述：
 *
 * 获取CBmpProc类的版本号
 *
 * 注：返回值中的高8位是主版本号，低8位是子版本号
 * 
 ************************************************************************/


WORD CBmpProc::GetVersion() const
{ 
	return (WORD)BMPPROC_VERSION; 
}



/*************************************************************************
 *
 * PasteAvailable()
 * 
 * 参数说明：无
 *
 *
 * 返回值：
 *
 * BOOL					- 如果剪贴板中存在DIB格式的数据，则返回TRUE，否则
 *						  返回FALSE
 * 
 * 描述：
 *
 * 判断剪贴板中是否存在DIB格式的数据。
 *
 ************************************************************************/


BOOL CBmpProc::PasteAvailable()
{
	return (IsClipboardFormatAvailable(CF_DIB)) ? TRUE:FALSE;
}



/*************************************************************************
 *
 * IsFileExist()
 * 
 * 参数说明：
 *
 * LPCTSTR lpFileName	- 待判断的文件路径和名称（文件名）
 *
 * 返回值：
 *
 * BOOL					- 如果指定的文件存在返回TRUE，否则返回FALSE。
 * 
 * 描述：
 *
 * 判断指定的文件是否存在
 * 
 * 该文件必需可以被读和写
 *
 ************************************************************************/


BOOL CBmpProc::IsFileExist(LPCTSTR lpFileName)
{
	CFile	file;
	BOOL	bExist = FALSE;	// 文件存在是TRUE，不存在是FALSE

	// 确定指定的文件是否存在
	if (file.Open(lpFileName, CFile::modeReadWrite))
	{
		bExist = TRUE;
		file.Close();
	}

	return bExist;
}



/*************************************************************************
 *
 * GetHDib()
 * 
 * 参数说明：无
 *
 *
 * 返回值：
 *
 * HANDLE				- 如果成功返回DIB内部形式的内存块句柄
 * 
 * 描述：
 *
 * 获取本身类位图的DIB内部形式的内存块句柄
 * 
 * # 如果本身类中原来没有位图，函数返回NULL
 * # 返回的内存句柄是用GlobalAlloc()函数分配的，如果不再使用时应及时释放
 * # 返回的句柄可以贴入剪贴板，或与其它的DIB类通信
 *
 ************************************************************************/


HANDLE CBmpProc::GetHDib() const
{ 
	// 如果本身类中原来没有位图，函数返回NULL
	if (!IsValid())
		return NULL;

	ASSERT(m_pInfo);

	// DIB内部形式尺寸（dibsize）
	DWORD infosize = CalculateDIBInfoSize((LPBITMAPINFOHEADER)m_pInfo);
	DWORD dibsize = CalculateDIBSize((LPBITMAPINFOHEADER)m_pInfo);

	// 创建DIB内存块，带GMEM_DDESHARE标识可以使该句柄用于剪贴板
	HANDLE h = ::GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE, dibsize);
	if (!h)
		return NULL;

	LPSTR lp = (LPSTR)::GlobalLock(h);
	if (lp)
	{
		// 拷入DIB信息
		::memcpy((void *)lp, (const void *)m_pInfo, (size_t)infosize);

		// 获取DIB位数据
		HWND hWnd = ::GetDesktopWindow();
		ASSERT(hWnd);
		HDC hDC = ::GetDC(hWnd);
		if (!::GetDIBits(hDC,(HBITMAP)m_hObject,0,m_pInfo->bmiHeader.biHeight,\
				(LPVOID)((DWORD)lp+infosize),(LPBITMAPINFO)lp,DIB_RGB_COLORS))
		{
			::GlobalUnlock(h);
			::GlobalFree(h);
			::ReleaseDC(hWnd, hDC);
			return NULL;
		}
		::ReleaseDC(hWnd, hDC);
		::GlobalUnlock(h);

		return h;	// 成功
	}

	::GlobalFree(h);

	return NULL;
}



/*************************************************************************
 *
 * IsValidDibFormat()
 * 
 * 参数说明：
 *
 * WORD nBitsPerPixel	- 待判定的位数值
 *
 * 返回值：
 *
 * BOOL					- 是有效值则返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 判断指定的位数值（即每像素占多少位）是否是合法值
 * 
 * # CBmpProc类当前支持1,4,8,16,24,32位格式的位图
 *
 ************************************************************************/


BOOL CBmpProc::IsValidDibFormat(WORD nBitsPerPixel)
{
	// 检测位数合法性
	switch(nBitsPerPixel)
	{
	case	1:
	case	4:
	case	8:
	case	16:
	case	24:
	case	32:
		// 以上格式的位图是合法的
		return TRUE;
	default:
		ASSERT(FALSE);	// 设计错误
		break;
	}

	return FALSE;
}



/*************************************************************************
 *
 * ClearAndSetData()
 * 
 * 参数说明：
 *
 * int type				- 新位图来源类型
 * DWORD addinfo		- 新位图的附加信息值
 * LPCTSTR lpFn			- 新位图的文件名字符串
 * LPBITMAPINFO pInfo	- 新位图的信息块地址
 * HBITMAP hBmp			- 新位图的位图句柄
 *
 * 返回值：
 *
 * BOOL					- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 清除类中原来的位图，并用指定的数据设置位图
 * 
 * # 如果本身类中原来有位图，函数将清除它们
 * # 入口指定的参数必须保证是有效的数据
 *
 ************************************************************************/


BOOL CBmpProc::ClearAndSetData(int type, 
							   DWORD addinfo, 
							   LPCTSTR lpFn, 
							   LPBITMAPINFO pInfo, 
							   HBITMAP hBmp)
{
	ASSERT(pInfo);
	ASSERT(hBmp);

	// 如果原来存在位图
	if (m_mark == TRUE)
	{
		// 分离出原来的位图句柄
		ASSERT((HBITMAP)m_hObject);
		HBITMAP hOldBmp = (HBITMAP)CGdiObject::Detach();
		ASSERT(hOldBmp);
		if (!CGdiObject::Attach((HGDIOBJ)hBmp))
		{
			// 如果失败，恢复原来的位图句柄。
			CGdiObject::Attach((HGDIOBJ)hOldBmp);
			return FALSE;
		}
		// 如果成功，原来的位图句柄将无用，释放
		::DeleteObject((HGDIOBJ)hOldBmp);

		m_type = type,			// 位图来源
		m_addInfo = addinfo;	// 附加信息
		m_cFileName.Empty();
		m_cFileName = lpFn;		// 文件路径。
		ASSERT(m_pInfo);
		::free((void*)m_pInfo);
		m_pInfo = pInfo;		// DIB信息结构

		return TRUE;
	}
	else	// 原来没有位图
	{
		ASSERT(m_pInfo == NULL);
		ASSERT(m_hObject == NULL);

		if (!CGdiObject::Attach((HGDIOBJ)hBmp))
			return FALSE;

		m_mark = TRUE;			// 有效标志置
		m_type = type,			// 位图来源
		m_addInfo = addinfo;	// 附加信息
		m_cFileName = lpFn;		// 文件路径。
		m_pInfo = pInfo;		// DIB信息结构

		return TRUE;
	}
}



/*************************************************************************
 *
 * DrawTransparentInPoint()
 * 
 * 参数说明：
 *
 * CDC& dc					- 显示位图的设备描述表
 * COLORREF crColour		- 图像中要被滤掉的颜色（缺省值为白色）
 * int x 					- 目标矩形的左上角坐标 X（缺省值为0）
 * int y 					-           ''         Y（缺省值为0）
 * CRect* crSou				- 源图像待显示部分的矩形坐标（缺省值是NULL）
 * int mode					- 如果是0则将不显示指定的颜色，如果是1则保留指定
 *						      的颜色（缺省值为0）
 *
 * 返回值：
 *
 * BOOL					- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 将类中的图像透明的绘入目标设备描述表指定区域，如有必要可以产生拉伸或压缩
 * 动作
 * 
 * 如果类中没有图像，它什么也不作，直接返回
 * 如果待绘制的图像不在当前剪贴区域内，则直接返回
 * 如果入口参数crSou被设为NULL，则函数将不会产生图像的拉伸或压缩
 * 
 ************************************************************************/


BOOL CBmpProc::DrawTransparentInPoint(CDC& dc, 
									  COLORREF crColour, 
									  int x, 
									  int y, 
									  CRect* crSou, 
									  int mode)
{
	// 如果类中没有图像，直接返回 
	if (!IsValid())
		return TRUE;

	ASSERT(m_hObject);

	CRect DCRect(Rect());			// 目标位置
	DCRect.OffsetRect(x, y);

	return DrawTransparentInRect(dc, crColour, &DCRect, crSou, mode);
}



/*************************************************************************
 *
 * DrawTransparentInRect()
 * 
 * 参数说明：
 *
 * CDC& dc					- 显示位图的设备描述表
 * COLORREF crColour		- 图像中要被滤掉的颜色（缺省值为白色）
 * CRect* crDest			- 目标矩形坐标数据（缺省值是NULL）
 * CRect* crSou				- 源图像待显示部分的矩形坐标（缺省值是NULL）
 * int mode					- 如果是0则将不显示指定的颜色，如果是1则保留指定
 *						      的颜色（缺省值为0）
 *
 * 返回值：
 *
 * BOOL					- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 将类中的图像透明的绘入目标设备描述表指定区域，如有必要可以产生拉伸或压缩
 * 动作
 * 
 * 如果类中没有图像，它什么也不作，直接返回
 * 如果待绘制的图像不在当前剪贴区域内，则直接返回
 * 如果入口参数crDest和crSou都是NULL，则函数将在设备描述表的0,0位置不变形的
 * 绘制类中位图的整个内容
 * 如果入口参数crDest和crSou的宽度或高度值不同，则绘制出来的图像会变形（被
 * 拉伸或被压缩）
 * 
 ************************************************************************/


BOOL CBmpProc::DrawTransparentInRect(CDC& dc, 
							   COLORREF crColour, 
							   CRect* crDest, 
							   CRect* crSou, 
							   int mode)
{
	// 如果类中没有图像，直接返回 
	if (!IsValid())
		return TRUE;

	ASSERT(m_hObject);

	// 缺省矩形等于图像尺寸
	CRect DCRect(Rect());			// 目标位置
	CRect SORect(Rect());			// 源位置

	if (crDest)
		DCRect = *crDest;
	if (crSou)
		SORect = *crSou;

	// 如果待绘制的图像不在当前剪贴区域内，则直接返回
	if (!dc.RectVisible(&DCRect))
		return TRUE;

	COLORREF crOldBack;
	COLORREF crOldText;

	if (mode==0)
	{	// 去除指定的颜色
		crOldBack = dc.SetBkColor(RGB(255,255,255));
		crOldText = dc.SetTextColor(RGB(0,0,0));
	}
	else
	{	// 保留指定的颜色
		crOldBack = dc.SetBkColor(RGB(0,0,0));
		crOldText = dc.SetTextColor(RGB(255,255,255));
	}

	CDC dcImage, dcTrans;

	// 创建内存设备描述表
	dcImage.CreateCompatibleDC(&dc);
	dcTrans.CreateCompatibleDC(&dc);

	// 选择原始图像到DC中
	CBitmap* pOldBitmapImage = dcImage.SelectObject(this);

	// 创建掩码位图
	CBitmap bitmapTrans;
	int nWidth = SORect.Width();
	int nHeight = SORect.Height();
	bitmapTrans.CreateBitmap(nWidth, nHeight, 1, 1, NULL);

	// 选择掩码位图到DC中
	CBitmap* pOldBitmapTrans = dcTrans.SelectObject(&bitmapTrans);

	// 创建掩码图像（基于指定的颜色）
	dcImage.SetBkColor(crColour);
	dcTrans.BitBlt(0, 0, nWidth, nHeight, &dcImage, SORect.left, SORect.top, SRCCOPY);

	// 显示透明位图

	/* 设置目标DC的拉伸模式为STRETCH_DELETESCANS，也就是不显示拉伸掉的图像 */
	int srlold = dc.SetStretchBltMode(STRETCH_DELETESCANS);

	// 显示位图
	dc.StretchBlt(DCRect.left,DCRect.top,DCRect.Width(),DCRect.Height(),
		&dcImage,SORect.left,SORect.top,SORect.Width(),SORect.Height(),SRCINVERT);
	dc.StretchBlt(DCRect.left,DCRect.top,DCRect.Width(),DCRect.Height(),
		&dcTrans,SORect.left,SORect.top,SORect.Width(),SORect.Height(),SRCAND);
	dc.StretchBlt(DCRect.left,DCRect.top,DCRect.Width(),DCRect.Height(),
		&dcImage,SORect.left,SORect.top,SORect.Width(),SORect.Height(),SRCINVERT);

	// 恢复设备描述表原来的设置
	dc.SetStretchBltMode(srlold);

	// 恢复设置
	dcImage.SelectObject(pOldBitmapImage);
	dcTrans.SelectObject(pOldBitmapTrans);
	dc.SetBkColor(crOldBack);
	dc.SetTextColor(crOldText);

	return TRUE;
}



/*************************************************************************
 *
 * ExportRegion()
 * 
 * 参数说明：
 *
 * COLORREF cTranColor		- 指定要滤掉的颜色（缺省值是白色）
 *
 * 返回值：
 *
 * HRGN 					- 如果成功返回位图的区域句柄，否则返回NULL
 * 
 * 描述：
 *
 * 输出类中位图的区域句柄
 * 
 * 该函数就是将指定的颜色不划入区域之内，而由其它颜色的坐标组成一个区域
 * 如果类中没有图像，它什么也不作，直接返回NULL
 * 当长时间不需要这个返回的区域句柄时，应调用DeleteObject()函数将其释放
 * 
 ************************************************************************/


HRGN CBmpProc::ExportRegion(COLORREF cTranColor)
{
	HRGN hRgn = NULL;

	// 如果类中没有图像，直接返回
	if (!IsValid())
		return NULL;

	ASSERT(m_hObject);

	int wi, he;

	wi = Width();
	he = Height();

	// 创建内存设备表述表用于获取本类中位图的位数据
	HDC hMemDC = ::CreateCompatibleDC(NULL);
	if (hMemDC)
	{
		BITMAPINFOHEADER bmi = {sizeof(BITMAPINFOHEADER), wi, he, 1, 32, BI_RGB, 0, 0, 0, 0, 0};
		VOID * pbits32; 
		// 创建位图部件(Section)，获取位数据
		HBITMAP hbm32 = ::CreateDIBSection(hMemDC, (BITMAPINFO *)&bmi, DIB_RGB_COLORS, &pbits32, NULL, 0);
		if (hbm32)
		{
			HBITMAP holdBmp = (HBITMAP)::SelectObject(hMemDC, hbm32);
			HDC hDC = ::CreateCompatibleDC(hMemDC);
			if (hDC)
			{
				BITMAP bm32;
				::GetObject(hbm32, sizeof(BITMAP), &bm32);
				while (bm32.bmWidthBytes % 4)	// DIB扫描行为DWORD对齐
					bm32.bmWidthBytes++;

				HBITMAP hob = (HBITMAP)::SelectObject(hDC, (HBITMAP)m_hObject);
				::BitBlt(hMemDC, 0, 0, wi, he, hDC, 0, 0, SRCCOPY);
				::SelectObject(hDC, hob);
				::DeleteDC(hDC);

				// 区域矩形个数的步长值设为200
				#define ALLOC_UNIT	200

				DWORD maxRects = ALLOC_UNIT;
				RGNDATA *pData = (RGNDATA *)CreateMemoryBlockFromAddress(NULL, sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRects));
				if (!pData)
				{
					::DeleteObject(::SelectObject(hMemDC, holdBmp));
					::DeleteDC(hMemDC);
					return NULL;
				}

				// 初始化区域数据结构
				pData->rdh.dwSize = sizeof(RGNDATAHEADER);
				pData->rdh.iType = RDH_RECTANGLES;
				pData->rdh.nCount = 0;
				pData->rdh.nRgnSize = 0;
				::SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

				// 创建区域
				BYTE *p32 = (BYTE *)bm32.bmBits + (bm32.bmHeight-1)*bm32.bmWidthBytes;
				for (int y = 0; y < he; y++)
				{
					for (int x = 0; x < wi; x++)
					{
						int x0 = x;
						LONG *p = (LONG *)p32 + x;
						while (x < wi)
						{	// 指定要滤掉的颜色
							if (((*p)&0xFFFFFF)==((LONG)cTranColor&0xFFFFFF))
								break;
							p++;
							x++;
						}

						if (x > x0)	// 表明有非滤掉的颜色
						{
							if (pData->rdh.nCount >= maxRects)
							{	// 如果区域结构中不能再容纳新的矩形数据，则再追加内存
								ASSERT(pData);
								ASSERT(pData->rdh.dwSize == sizeof(RGNDATAHEADER));
								maxRects += ALLOC_UNIT;
								RGNDATA *p = (RGNDATA *)CreateMemoryBlockFromAddress((LPVOID)pData, sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRects));
								::free((void*)pData);
								if (!p)
								{
									if (hRgn)
										::DeleteObject(hRgn);
									::DeleteObject(::SelectObject(hMemDC, holdBmp));
									::DeleteDC(hMemDC);
									return NULL;
								}
								ASSERT(p->rdh.dwSize == sizeof(RGNDATAHEADER));
								pData = p;
							}

							// 刷新区域结构中的数据
							RECT *pr = (RECT *)&pData->Buffer;
							SetRect(&pr[pData->rdh.nCount], x0, y, x, y+1);
							if (x0 < pData->rdh.rcBound.left)
								pData->rdh.rcBound.left = x0;
							if (y < pData->rdh.rcBound.top)
								pData->rdh.rcBound.top = y;
							if (x > pData->rdh.rcBound.right)
								pData->rdh.rcBound.right = x;
							if (y+1 > pData->rdh.rcBound.bottom)
								pData->rdh.rcBound.bottom = y+1;
							pData->rdh.nCount++;

							// 因为创建区域的函数ExtCreateRegion()要求区域数据结构中的矩形个数不能超过4000个，
							// 所以这里取极限值的一半。
							if (pData->rdh.nCount == 2000)
							{
								HRGN h = ::ExtCreateRegion(NULL, sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRects), pData);
								if (hRgn)
								{
									::CombineRgn(hRgn, hRgn, h, RGN_OR);
									::DeleteObject(h);
								}
								else
									hRgn = h;
								pData->rdh.nCount = 0;
								::SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
							}
						}
					}
					// 指向下一行的首地址（DIB扫描行是从下到上）
					p32 -= bm32.bmWidthBytes;
				}

				ASSERT(pData);

				// 如果整张位图全是指定的要滤掉的颜色，或者区域矩形个数少于
				// 2000，则由以下代码完成创建区域的工作
				HRGN hg = ::ExtCreateRegion(NULL, sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRects), pData);
				if (hRgn)
				{
					::CombineRgn(hRgn, hRgn, hg, RGN_OR);
					::DeleteObject(hg);
				}
				else
					hRgn = hg;

				::free((void*)pData);
			}
			::DeleteObject(::SelectObject(hMemDC, holdBmp));
		}
		::DeleteDC(hMemDC);
	}	

	return hRgn;
}



/*************************************************************************
 *
 * Fill()
 * 
 * 参数说明：
 *
 * COLORREF crColour		- 指定要填充的颜色（缺省值是白色）
 * CRect *crArea			- 填充的区域矩形（缺省值是NULL）
 *
 * 返回值：
 *
 * BOOL 					- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 用指定的颜色填充本身类中位图的指定区域（矩形）
 * 
 * 如果入口参数crArea被设为NULL，则函数将填充整个的位图
 * 如果类中没有图像，直接返回FALSE
 * 
 ************************************************************************/


BOOL CBmpProc::Fill(COLORREF crColour, CRect *crArea)
{
	// 本身类中必须存在位图
	if (!IsValid())
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	CRect	rect(Rect());

	if (crArea)
		rect = *crArea;

	// 复制源图
	CDC			compDC;
	CBitmap		*pOldBmp;

	// 创建与当前显示设备兼容的内存设备描述表
	compDC.CreateCompatibleDC(NULL);
	pOldBmp	= compDC.SelectObject((CBitmap*)this);

	// 填充指定颜色
	compDC.FillSolidRect((LPCRECT)&rect, crColour);

	compDC.SelectObject(pOldBmp);

	return TRUE;
}



/*************************************************************************
 *
 * ReSetColor()
 *
 * 参数说明：
 *
 * COLORREF oldcolr			- 待替换的旧颜色
 * COLORREF newcolr			- 将要替换上去的新的颜色
 * CRect *crArea			- 要替换的范围（缺省值是NULL）
 *
 * 返回值：
 *
 * BOOL 					- 如果成功返回TRUE，否则返回FALSE
 * 
 * 描述：
 *
 * 用一种颜色替换另一种颜色，在位图的指定矩形中
 * 
 * 如果入口参数crArea被设为NULL，则函数将替换整个的位图指定颜色
 * 如果类中没有图像，直接返回FALSE
 * 
 ************************************************************************/


BOOL CBmpProc::ReSetColor(COLORREF oldcolr, COLORREF newcolr, CRect *crArea)
{
	// 本身类中必须存在位图
	if (!IsValid())
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	// 复制本身类中的位图作为副本
	CBmpProc tmp;
	
	tmp = *this;

	if (!tmp.IsValid())
		return FALSE;

	// 填充指定矩形为新的颜色
	Fill(newcolr, crArea);
	// 复制去掉旧颜色的图像
	CopyTran(tmp, oldcolr, crArea, crArea, 0);

	return TRUE;
}



