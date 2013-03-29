///////////////////////////////////////////////////////////////////////
/*
  目的：BMP位图处理类。（定义文件）

  创建：张伟(SXZ)
  日期：2000.01
  版本：2.4

  SXZ software workgroup. No.200001
*/
///////////////////////////////////////////////////////////////////////


#ifndef __BMPPROC_H__
#define __BMPPROC_H__

/* 版本 */
#define BMPPROC_VERSION   0x240

// DIBSCANLINE_WIDTHBYTES 执行DIB扫描行的DWORD对齐操作。宏参数“bits”是
// DIB信息结构中成员biWidth和biBitCount的乘积。宏的结果是经过对齐后一个
// 扫描行所占的字节数。
#define DIBSCANLINE_WIDTHBYTES(bits)    (((bits)+31)/32*4)
// DDBSCANLINE_WIDTHBYTES 执行DDB扫描行的WORD对齐操作。宏参数“bits”是
// DDB信息结构中成员bmWidth和bmBitCount的乘积。宏的结果是经过对齐后一个
// 扫描行所占的字节数。
#define DDBSCANLINE_WIDTHBYTES(bits)    (((bits)+15)/16*2)

/* DIB 文件头标志（"BM") */
#define DIB_HEADER_MARKER   ((WORD)('M'<<8)|'B')

// 常用颜色定义
#define CBPCOLOR_BLACK		((COLORREF)RGB(0,0,0))
#define CBPCOLOR_GRAY		((COLORREF)RGB(128,128,128))
#define CBPCOLOR_HIGHGRAY	((COLORREF)RGB(192,192,192))
#define CBPCOLOR_WHITE		((COLORREF)RGB(255,255,255))
#define CBPCOLOR_RED		((COLORREF)RGB(255,0,0))
#define CBPCOLOR_GREEN		((COLORREF)RGB(0,255,0))
#define CBPCOLOR_BLUE		((COLORREF)RGB(0,0,255))


// 注意：类CBmpProc是从CBitmap类公有继承而来的，但最好不要使用类CBitmap
// 中的函数！如果使用了（尤其是读入或创建型的函数），则就不要再使用CBmpProc
// 类中的函数，以免混乱。

class CBmpProc : public CBitmap
{
public:
	CBmpProc();
	~CBmpProc();

	BOOL Create(int width, int height, WORD nBitCount, const void *lpBits=NULL);
	BOOL Create(CSize size, WORD nBitCount, const void *lpBits=NULL);
	BOOL CreateCompatible(int width, int height, CDC *pDC=NULL, const void *lpBits=NULL);

	BOOL CopyFromObject(CBmpProc& souBmp, CRect *pDesRect=NULL, CRect *pSouRect=NULL);
	BOOL CopyFromHbmp(HBITMAP souBmp, CRect *pDesRect=NULL, CRect *pSouRect=NULL);
	BOOL CopyTran(CBmpProc& souBmp, COLORREF crColour=CBPCOLOR_WHITE, 
			CRect *pDesRect=NULL, CRect *pSouRect=NULL, int mode=0);
	BOOL CopyTran(CBmpProc& souBmp, COLORREF crColour=CBPCOLOR_WHITE, 
			int x=0, int y=0, CRect *pSouRect=NULL, int mode=0);
	BOOL CopyTranCenter(CBmpProc& souBmp, CRect* crArea=NULL, 
			COLORREF crColour=CBPCOLOR_WHITE, int mode=0);
	BOOL CopyTranTile(CBmpProc& souBmp, CRect* crArea=NULL, 
			COLORREF crColour=CBPCOLOR_WHITE, int mode=0);

	BOOL LoadFromFile(LPCTSTR lpszPathName);
	BOOL LoadFromResourceID(UINT uID, HINSTANCE hInst);
	BOOL LoadFromResourceName(LPCTSTR lpResourceName, HINSTANCE hInst);
	BOOL LoadFromObject(CBmpProc& souBmp, CRect *pSouRect=NULL);
	BOOL LoadFromHbmp(HBITMAP sou, CRect *pSouRect=NULL);
	BOOL LoadFromClipboard();

	BOOL Save(LPCTSTR lpszNewFileName=NULL, WORD nBitsPerPixel=0);
	BOOL SaveToClipboard();

	BOOL Draw(CDC& dc, const CRect* rcDst=NULL, const CRect* rcSrc=NULL);
	BOOL DrawTile(CDC& dc, CRect* crArea);
	BOOL DrawCenter(CDC& dc, CRect* crArea);
	BOOL DrawTransparentInRect(CDC& dc, COLORREF crColour=CBPCOLOR_WHITE, 
			CRect* crDest=NULL, CRect* crSou=NULL, int mode=0);
	BOOL DrawTransparentInPoint(CDC& dc, COLORREF crColour=CBPCOLOR_WHITE, 
			int x=0, int y=0, CRect* crSou=NULL, int mode=0);
	BOOL DrawTranCenter(CDC& dc, CRect* crArea, COLORREF crColour=CBPCOLOR_WHITE, int mode=0);
	BOOL DrawTranTile(CDC& dc, CRect* crArea, COLORREF crColour=CBPCOLOR_WHITE, int mode=0);

	BOOL Attach(HBITMAP hBmp);
	HBITMAP Detach();
	void Clear();
	HRGN ExportRegion(COLORREF cTranColor=CBPCOLOR_WHITE);
	BOOL Fill(COLORREF crColour=CBPCOLOR_WHITE, CRect *crArea=NULL);
	BOOL ReSetColor(COLORREF oldcolr, COLORREF newcolr, CRect *crArea=NULL);
		   
	// 重载操作符函数
	CBmpProc& operator=(const CBmpProc& ds);
	CBmpProc& operator=(const HBITMAP sou);
	BOOL operator==(const CBmpProc& ds);
	BOOL operator!=(const CBmpProc& ds);
	operator HBITMAP() const { return (HBITMAP)m_hObject; };


	// 类数据访问函数
	CSize Size() const;
	CRect Rect() const;
	int   Width() const;
	int   Height() const;
	int   ScanWidth() const;
	int   BitsPerPixel() const;

	BOOL  IsValid() const;

	//访问类内部数据的介面函数
	int   GetImageType() const;
	DWORD GetInAddInfo() const;
	const CString& GetFileName() const;
	BOOL  SetFileName(LPSTR pSou);
	int   GetObject(LPBITMAP lpBitmap) const;

	HANDLE GetHDib() const;
	WORD GetVersion() const;

#ifdef _DEBUG
	static DWORD GetObjectCount() {return m_ObjectCount;};
#endif

protected:
	// 助手函数（与类无关）
	LPSTR FindDIBBits(LPSTR lpbi) const;
	DWORD DIBWidth(LPBITMAPINFOHEADER lpbi) const;
	DWORD DIBHeight(LPBITMAPINFOHEADER lpbi) const;
	WORD PaletteSize(LPBITMAPINFOHEADER lpbi) const;
	WORD DIBNumColors(LPBITMAPINFOHEADER lpbi) const;
	int	GetBitsPerPixel(LPBITMAPINFOHEADER lpbi) const;

	LPVOID CreateMemoryBlockFromHandle(HANDLE handle, DWORD dwCount=0);
	LPVOID CreateMemoryBlockFromAddress(LPVOID pSou, DWORD dwCount=0);

	BOOL IsValidBmpFile(CFile& file);
	LPBITMAPINFO GetBmpFileInfo(CFile& file);
	LPSTR GetBmpFileBits(CFile& file);
	BOOL IsValidDibFormat(WORD nBitsPerPixel);
	BOOL IsFileExist(LPCTSTR lpFileName);

	DWORD CalculateDIBFileSize(LPBITMAPINFOHEADER lpbi) const;
	DWORD CalculateDIBSize(LPBITMAPINFOHEADER lpbi) const;
	DWORD CalculateDIBitsOff(LPBITMAPINFOHEADER lpbi) const;
	DWORD CalculateDIBInfoSize(LPBITMAPINFOHEADER lpbi) const;
	DWORD CalculateDIBitsSize(LPBITMAPINFOHEADER lpbi) const;
	
	BOOL PasteAvailable();


	// 内部服务函数（与类有关）
	BOOL SaveBaseSpecifyFileName(LPCTSTR fn);
	BOOL SaveBaseSpecifyFormat(WORD nBitsPerPixel);
	LPBITMAPINFO GetSpecifyFormatInfo(WORD newBitCount=0);
	BOOL ClearAndSetData(int type, DWORD addinfo, LPCTSTR lpFn, LPBITMAPINFO pInfo, HBITMAP hBmp);

public:
	enum			// 位图的初始来源类型：
	{
		IT_NULL,		// 无位图类型
		IT_DISKFILE,	// 文件位图
		IT_RESOURCE,	// 资源位图
		IT_MEMORY,		// 内存位图
		IT_CLIP,		// 剪贴板位图
		IT_CREATE		// 创建型位图
	};

private:
	// 数据成员
	BOOL m_mark;		// 类的有效标志。（TRUE-本类中有位图，
						// FALSE-本类中没有位图）
	int  m_type;		// 图像类型，只能使用上面枚举类型值
	DWORD   m_addInfo;	// 附加信息。
	CString	m_cFileName;// 文件路径。
	LPBITMAPINFO  m_pInfo;	// DIB 信息结构

#ifdef _DEBUG
	static DWORD m_ObjectCount;	// 本类构造的实例目标个数，供调试使用
#endif
};

#endif /*__BMPPROC_H__*/
