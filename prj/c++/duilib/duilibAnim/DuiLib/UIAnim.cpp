#include "stdafx.h"
#include "UIAnim.h"
#include <math.h>
/////////////////////////////////////////////////////////////////////////////////////

namespace DuiLib{

#ifndef PI
#define PI 3.1415926535897932384626433832795029L
#endif

const RECT CAnimJobUI::s_cEmptyRect = {0,0,0,0};

extern /*static*/ BOOL WINAPI AlphaBitBlt(HDC hDC, int nDestX, int nDestY, int dwWidth, int dwHeight, HDC hSrcDC, int nSrcX, int nSrcY, int wSrc, int hSrc, BLENDFUNCTION ftn);
/////////////////////////////////////////////////////////////////////////////////////

CAnimJobUI::CAnimJobUI(const CAnimJobUI& src)
{   
	*this = src;
}
CAnimJobUI::CAnimJobUI(
	DWORD dwStartTick,
	DWORD dwDuration,
	POINT ptStartCenter,
	POINT ptXYtrans,
	float fStartZtrans /* = 1.0f */,
	float fEndZtrans /* = 1.0f */,
	bool bBeforePaint /*= false*/,
	bool bClip /*= false*/,
	RECT rcClip /*= s_cEmptyRect*/,
	RECT rcCaptureSrc /* = s_cEmptyRect */,
	UINT uCallBackMsg /*= WM_USER*/,
	LPVOID pMsgCallBackData /*= NULL*/):
m_dwStartTick(dwStartTick),
m_dwDuration(dwDuration),
m_ptStartCenter(ptStartCenter),
m_ptXYtrans(ptXYtrans),
m_fStartZtrans(fStartZtrans),
m_fEndZtrans(fEndZtrans),
m_rcCaptureSrc(rcCaptureSrc),
m_bBeforePaint(bBeforePaint),
m_bClip(bClip),
m_rcClip(rcClip),
m_uCallBackMsg(uCallBackMsg),
m_pMsgCallBackData(pMsgCallBackData)
{
	m_fRotateAngle = 0.0f;
	m_fRotateVecX = 0.0f;
	m_fRotateVecY = 0.0f;
	m_fRotateVecZ = 0.0f;

	m_clrBack = CLR_INVALID;
	m_iAlpha = 0;

	m_eSpeedType = SPEED_HYPERBOLA;//SPEED_PARABOLA;//SPEED_LINEAR;

	m_eTexFormType = TEXTURE_CAPTURE;
	m_strImgPath = _T("");

	m_bUseMaskBk = false;
	m_bIsCompleted = false;
	m_hMaskDC = NULL;
	m_hMaskBmp = NULL;
	m_hDestDC = NULL;
	m_hDestBmp = NULL;
}

CAnimJobUI::~CAnimJobUI()
{
	if (m_hMaskBmp != NULL)
	{
		DeleteBitmap(m_hMaskBmp);
	}
	if (m_hMaskDC != NULL)
	{
		DeleteDC(m_hMaskDC);
	}
	if (m_hDestBmp != NULL)
	{
		DeleteBitmap(m_hDestBmp);
	}
	if (m_hDestDC != NULL)
	{
		DeleteDC(m_hDestDC);
	}
}

void CAnimJobUI::SetRotateInfo( float fRotateAngle,float fRotateVecX,float fRotateVecY,float fRotateVecZ )
{
	m_fRotateAngle = fRotateAngle;
	m_fRotateVecX = fRotateVecX;
	m_fRotateVecY = fRotateVecY;
	m_fRotateVecZ = fRotateVecZ;
}

bool CAnimJobUI::InitBkInfo(HDC hDC)
{
	int cx = m_rcCaptureSrc.right-m_rcCaptureSrc.left;
	int cy = m_rcCaptureSrc.bottom-m_rcCaptureSrc.top;
	if (m_hMaskBmp == NULL)
	{
		m_hMaskBmp = CreateCompatibleBitmap(hDC,cx,cy);
	}
	if (m_hMaskDC == NULL)
	{
		m_hMaskDC = CreateCompatibleDC(hDC);
	}
	SelectBitmap(m_hMaskDC,m_hMaskBmp);
	BitBlt(m_hMaskDC,0,0,cx,cy,hDC,m_rcCaptureSrc.left,m_rcCaptureSrc.top,SRCCOPY);

	return true;
}

bool CAnimJobUI::InitDestInfo( HDC hDC )
{
	int cx = m_rcCaptureSrc.right - m_rcCaptureSrc.left;
	int cy = m_rcCaptureSrc.bottom - m_rcCaptureSrc.top;
	if (m_hDestBmp == NULL)
	{
		m_hDestBmp = CreateCompatibleBitmap(hDC,cx,cy);
	}
	if (m_hDestDC == NULL)
	{
		m_hDestDC = CreateCompatibleDC(hDC);
		SelectBitmap(m_hDestDC,m_hDestBmp);
	}
	BitBlt(m_hDestDC,0,0,cx,cy,hDC,m_rcCaptureSrc.left,m_rcCaptureSrc.top,SRCCOPY);
	return true;
}

bool CAnimJobUI::AdjustTargetTexture(HDC hDC)
{
	int cx = m_rcCaptureSrc.right - m_rcCaptureSrc.left;
	int cy = m_rcCaptureSrc.bottom - m_rcCaptureSrc.top;
	BITMAPINFO MyBMInfo = {0};
	// set up the structure of the buffer to receive by setting up the BITMAPINFOHEADER structure
	MyBMInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	// Get the BITMAPINFO structure from the bitmap
	if (0 == GetDIBits(m_hDestDC, m_hDestBmp, 0, 0, NULL, &MyBMInfo, DIB_RGB_COLORS))
	{
		return false;
	}
	MyBMInfo.bmiHeader.biBitCount = 32;  // no stride necessary, although you won't use the alpha value provided
	MyBMInfo.bmiHeader.biCompression = BI_RGB;  // no compression -> easier to parse
	MyBMInfo.bmiHeader.biPlanes = 1;
	MyBMInfo.bmiHeader.biHeight = (MyBMInfo.bmiHeader.biHeight < 0) ? (-MyBMInfo.bmiHeader.biHeight) : (MyBMInfo.bmiHeader.biHeight);  // correct the bottom-up ordering of lines
	BYTE* lpPixelsDest = NULL;
	BYTE* lpPixelsMask = NULL;
	if (!m_bUseMaskBk)
	{
		// create the bitmap buffer
		lpPixelsDest = new BYTE[MyBMInfo.bmiHeader.biSizeImage];
		if (lpPixelsDest == NULL)
		{
			return false;
		}
		// get the actual bitmap buffer
		if(0 == GetDIBits(m_hDestDC, m_hDestBmp, 0, MyBMInfo.bmiHeader.biHeight, (LPVOID)lpPixelsDest, &MyBMInfo, DIB_RGB_COLORS))
		{
			delete []lpPixelsDest;
			return false;
		}
		DWORD* pDestData = (DWORD*)lpPixelsDest;
		DWORD dwLen = MyBMInfo.bmiHeader.biSizeImage/4;
		for (DWORD i=0;i < dwLen;++i)
		{
			pDestData[i] |= 0xFF000000; 
		}
	}
	else
	{
		if (!InitBkInfo(hDC))
		{
			return false;
		}
		lpPixelsDest = new BYTE[MyBMInfo.bmiHeader.biSizeImage];
		if (lpPixelsDest == NULL)
		{
			return false;
		}
		lpPixelsMask = new BYTE[MyBMInfo.bmiHeader.biSizeImage];
		if (lpPixelsMask == NULL)
		{
			return false;
		}
		
		// get the actual bitmap buffer
		if(0 == GetDIBits(m_hDestDC, m_hDestBmp, 0, MyBMInfo.bmiHeader.biHeight, (LPVOID)lpPixelsDest, &MyBMInfo, DIB_RGB_COLORS))
		{
			delete []lpPixelsDest;
			return false;
		}
		if (0 == GetDIBits(m_hMaskDC,m_hMaskBmp, 0, MyBMInfo.bmiHeader.biHeight, (LPVOID)lpPixelsMask, &MyBMInfo, DIB_RGB_COLORS))
		{
			delete []lpPixelsMask;
			delete []lpPixelsDest;
			return false;
		}
		DWORD* pDestData = (DWORD*)lpPixelsDest;
		DWORD* pMaskData = (DWORD*)lpPixelsMask;
		DWORD dwLen = MyBMInfo.bmiHeader.biSizeImage/4;
		for (DWORD i=0;i < dwLen;++i)
		{
			if (pMaskData[i] == pDestData[i])
			{
				pDestData[i] = 0x00000000;
			}
			else
			{
				pDestData[i] |= 0xFF000000; 
			}
		}
	}

	//StretchDIBits(m_hDestDC,0,0,cx,cy,0,0,cx,cy,lpPixelsDest,&MyBMInfo,DIB_RGB_COLORS,SRCCOPY);
	SetDIBits(m_hDestDC,m_hDestBmp,0,cy,lpPixelsDest,&MyBMInfo,DIB_RGB_COLORS);
	delete []lpPixelsMask;
	delete []lpPixelsDest;
	return true;
}

void CAnimJobUI::SetBkColor( COLORREF clrBack )
{
	m_clrBack = clrBack;
}

void CAnimJobUI::SetAlpha( int iAlpha )
{
	m_iAlpha = iAlpha;
}

void CAnimJobUI::SetSpeedType( ANIM_SPEED_TYPE eSpeedType )
{
	m_eSpeedType = eSpeedType;
}

void CAnimJobUI::SetTextureFromFile( CStdString strImgPath )
{
	m_eTexFormType = TEXTURE_FILE;
	m_strImgPath = strImgPath;
}

void CAnimJobUI::SetTextureFromDC(HDC hSrcDC)
{
	m_hTexFormDC = hSrcDC;
	m_eTexFormType = TEXTURE_DC;
}

void CAnimJobUI::SetUseMaskBk()
{
	m_bUseMaskBk = true;
}


/////////////////////////////////////////////////////////////////////////////////////
CAnimationSpooler::CAnimationSpooler()
{
	m_hWnd = NULL;
	m_bIsAnimating = false;
	m_bIsInitialized = false;
	m_hSurfaceDC = NULL;
	m_hSurfaceBmp = NULL;
	m_hCanvasDC = NULL;
	m_hCanvasBmp = NULL;
}

CAnimationSpooler::~CAnimationSpooler()
{
	RemoveJobs();
}

bool CAnimationSpooler::Init(HWND hWnd)
{	
	if( m_bIsInitialized ) return true;

	if (!IsWindow(hWnd))
	{
		return false;
	}
	m_hWnd = hWnd;
	m_bIsInitialized = true;

	return true;
}

bool CAnimationSpooler::AddJob(CAnimJobUI* pJob)
{
	if (pJob->m_eTexFormType == TEXTURE_CAPTURE && pJob->m_bBeforePaint)
	{
		if (!CaptureSurface())
		{
			return false;
		}
		if (!pJob->InitDestInfo(m_hSurfaceDC))
		{
			return false;
		}
	}
	else if (pJob->m_eTexFormType == TEXTURE_DC)
	{
		if (!pJob->InitDestInfo(pJob->m_hTexFormDC))
		{
			return false;
		}
	}
	::InvalidateRect(m_hWnd, NULL, FALSE);
	return m_aJobs.Add((LPVOID)pJob);
}

void CAnimationSpooler::RemoveJobs()
{	
	// Get rid of the animation jobs
	int nSize = m_aJobs.GetSize();
	for( int i = 0; i < nSize; i++ ) 
	{
		CAnimJobUI* pAniJobUI = static_cast<CAnimJobUI*>(m_aJobs[i]);
 		if (pAniJobUI->m_uCallBackMsg > WM_USER && IsWindow(m_hWnd))
 		{
			WPARAM wParam = (pAniJobUI->m_bIsCompleted) ? 1:0;
 			SendMessage(m_hWnd,pAniJobUI->m_uCallBackMsg,(WPARAM)wParam,(LPARAM)pAniJobUI->m_pMsgCallBackData);
 		}
		delete pAniJobUI;
	}
	m_aJobs.Empty();
	// Almost done...
	ReleaseGDI();
	m_bIsAnimating = false;
	m_bIsInitialized = false;	
}
bool CAnimationSpooler::IsAnimating() const
{
	return m_bIsAnimating;
}

bool CAnimationSpooler::IsJobsEmpty() const
{
	return m_aJobs.IsEmpty();
}

bool CAnimationSpooler::Prepare()
{
	if (!CaptureSurface())
	{
		return false;
	}
	
	// Allow each job to prepare its dc
	int nSize = m_aJobs.GetSize();
	for( int i = 0; i < nSize; i++ )
	{
		CAnimJobUI* pJob = static_cast<CAnimJobUI*>(m_aJobs[i]);
		if( !PrepareJob_Flat(pJob) ) return false;
	}

	RECT rcClient;
	GetClientRect(m_hWnd,&rcClient);
	HDC hDC = GetDC(m_hWnd);
	m_hCanvasDC = CreateCompatibleDC(hDC);
	m_hCanvasBmp = CreateCompatibleBitmap(hDC,rcClient.right,rcClient.bottom);
	SelectBitmap(m_hCanvasDC,m_hCanvasBmp);

	// Assign start time
	DWORD dwTick = ::timeGetTime();
	for( int i = 0; i < nSize; i++ )
	{
		CAnimJobUI* pJob = static_cast<CAnimJobUI*>(m_aJobs[i]);
		pJob->m_dwStartTick += dwTick;
	}
	m_bIsAnimating = true;
	return true;
}

bool CAnimationSpooler::Render()
{	
	if( !m_bIsInitialized || !m_bIsAnimating) return false;
	// Get render target

	RECT rcClient;
	GetClientRect(m_hWnd,&rcClient);
	BitBlt(m_hCanvasDC,0,0,rcClient.right,rcClient.bottom,m_hSurfaceDC,0,0,SRCCOPY);
	bool bAnyMoreAnimate = false;
	DWORD dwTick = ::timeGetTime();
	for( int i = 0; i < m_aJobs.GetSize(); i++ )
	{
		CAnimJobUI* pJob = static_cast<CAnimJobUI*>(m_aJobs[i]);
		if( dwTick < pJob->m_dwStartTick )  //还没到要绘制时间
		{
			bAnyMoreAnimate = true; 
			continue;
		}
		DWORD dwTickNow = MIN(dwTick, pJob->m_dwStartTick + pJob->m_dwDuration);
		RenderJob_Flat(pJob,dwTickNow,m_hCanvasDC);
		if( dwTick < pJob->m_dwStartTick + pJob->m_dwDuration ) 
		{
			bAnyMoreAnimate = true;
		}
	}

	HDC hdc = GetDC(m_hWnd);
	BitBlt(hdc,0,0,rcClient.right,rcClient.bottom,m_hCanvasDC,0,0,SRCCOPY);
	ReleaseDC(m_hWnd,hdc);
	// No more frames to animate ?
	if(!bAnyMoreAnimate) 
	{
		RemoveJobs();
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////

bool CAnimationSpooler::CaptureSurface()
{
	if( !m_bIsInitialized) return false;
	// get the background

	HDC hDC = GetDC(m_hWnd);
	if (m_hSurfaceBmp == NULL)
	{	
		RECT rcClient;
		GetClientRect(m_hWnd, &rcClient);
		m_hSurfaceBmp = CreateCompatibleBitmap(hDC,rcClient.right,rcClient.bottom);
	}
	if (m_hSurfaceDC == NULL)
	{
		m_hSurfaceDC = CreateCompatibleDC(hDC);
		SelectBitmap(m_hSurfaceDC,m_hSurfaceBmp);
	}
	SendMessage(m_hWnd, WM_PRINTCLIENT, (WPARAM) m_hSurfaceDC, PRF_CHECKVISIBLE | PRF_CLIENT | PRF_ERASEBKGND | PRF_CHILDREN);
	ReleaseDC(m_hWnd,hDC);
// 
// #ifdef _DEBUG
// 	if (OpenClipboard(m_hWnd))
// 	{
// 		EmptyClipboard();
// 		SetClipboardData(CF_BITMAP,m_hSurfaceBmp);
// 		CloseClipboard();
// 	}
// #endif

	return true;
}
bool CAnimationSpooler::PrepareJob_Flat(CAnimJobUI* pJob)
{
	switch(pJob->m_eTexFormType)
	{
	case TEXTURE_CAPTURE:
		{
			if (!pJob->m_bBeforePaint && !pJob->InitDestInfo(m_hSurfaceDC))
			{
				return false;			
			}
		}break;
	case TEXTURE_FILE:
		{
			// todo
		}break;
	case TEXTURE_DC:
		{
			
		}break;
	default:return false;
	}

	// adjust target bitmap
	if (pJob->m_eTexFormType != TEXTURE_FILE && !pJob->AdjustTargetTexture(m_hSurfaceDC))
	{
		return false;
	}

	// Clear the background so the sprite can take its place
	COLORREF clrBack = pJob->m_clrBack;
	if( clrBack != CLR_INVALID && pJob->m_eTexFormType == TEXTURE_CAPTURE) {
		HDC hDC = GetDC(m_hWnd);
		RECT rcSrc = pJob->m_rcCaptureSrc;
		HBRUSH hBrush = CreateSolidBrush(clrBack);
		FillRect(hDC, &rcSrc, hBrush);
		DeleteObject(hBrush);
		ReleaseDC(m_hWnd,hDC);
	}
	return true;
}

bool CAnimationSpooler::RenderJob_Flat(CAnimJobUI* pJob, DWORD dwTick,HDC hDC)
{	
	if(pJob->m_bIsCompleted) return true; //动画执行完了,等待删除

	DWORD m = pJob->m_dwStartTick;
	DWORD n = pJob->m_dwDuration;

	FLOAT fPrecent = 0.0f;

	DWORD x = dwTick - m;

	if (pJob->m_eSpeedType == SPEED_LINEAR) //直线
	{
		// 过(0,1)和(n,0)直线,设直线方程 y = (n-x)/n
		fPrecent = (float)(double(n-x)/n);
	}
	else if (pJob->m_eSpeedType == SPEED_PARABOLA)//抛物线
	{
		// 设抛物线的方程为 y = k*x*x - (1 + k*n*n)*x + 1; 
		double k = 0.99f/(double)(n*n);
		fPrecent = (float)(k*x*x -(1.0f/(double)n+k*n)*x + 1.0f);
	}
	else if(pJob->m_eSpeedType == SPEED_HYPERBOLA) //双曲线
	{
		// 设双曲线方程为 y = k/(x+a)+b,其中a = -n*b,b = 0.5*(1-sqrt(4*k/n+1))
		float k = 50.0f;
		float b = 0.5f*(1.0f-sqrt(4*k/n+1.0f));
		float a = -(float)n*b;
		fPrecent = k/(x+a)+b;
	}

	if(fPrecent <= 0.000001f) //fPrecent = 0
	{
		fPrecent = 0.0f;
		pJob->m_bIsCompleted = true;
	}
	else if (fPrecent >= 1.0f)
	{
		fPrecent = 1.0f;
	}
	DWORD clrAlpha = ((DWORD)(0xFF - (FLOAT) abs(pJob->m_iAlpha) * (pJob->m_iAlpha >= 0 ? fPrecent : (1.0f-fPrecent))) << 24) | 0xffffff;
	RECT rcSrc = pJob->m_rcCaptureSrc;

	float cxHalfSrcRect = (float)((rcSrc.right - rcSrc.left)/2.0f);
	float cyHalfSrcRect = (float)((rcSrc.bottom - rcSrc.top)/2.0f);
	float fRotateVecX = pJob->m_fRotateVecX;
	float fRotateVecY = pJob->m_fRotateVecY;
	float fRotateVecZ = pJob->m_fRotateVecZ;
	float fRotateAngle = (float)(pJob->m_fRotateAngle/180.0f*PI*(1.0f-fPrecent));
	float xTrans = (float)pJob->m_ptXYtrans.x*(1.0f-fPrecent);
	float yTrans = (float)pJob->m_ptXYtrans.y*(1.0f-fPrecent);	
	float fStartZtrans = fabsf(pJob->m_fStartZtrans);
	float fEndZtrans = fabsf(pJob->m_fEndZtrans);
	float fZtrans = (float)(fStartZtrans + (fEndZtrans - fStartZtrans)*(1.0f-fPrecent));

// 	float fRadius = fZtrans*sqrt(cxHalfSrcRect*cxHalfSrcRect + cyHalfSrcRect*cyHalfSrcRect);  //旋转的半径
// 	float fRotateAngle = atan2f(cyHalfSrcRect,cxHalfSrcRect)*180/PI;						  //右上点与x轴所成角度

	float fRadiusX = fZtrans*cxHalfSrcRect;
	float fRadiusY = fZtrans*cyHalfSrcRect;
	POINT ptCenter = pJob->m_ptStartCenter;								  //原始控件中心点
	POINT ptC = {ptCenter.x+xTrans,ptCenter.y+yTrans};					  //移动后中心点

	POINT pt[4];
	//左上角
	pt[0].x = ptC.x - fRadiusX;
	pt[0].y = ptC.y - fRadiusY;
	//右上角
	pt[1].x = ptC.x + fRadiusX;
	pt[1].y = ptC.y - fRadiusY;
	//右下角
	pt[2].x = ptC.x + fRadiusX;
	pt[2].y = ptC.y + fRadiusY;
	//左下角
	pt[3].x = ptC.x - fRadiusX;
	pt[3].y = ptC.y + fRadiusY;

// 	//左上角
// 	vector3[0].x = ptC.x + fRadius*cos((fRotateVecZ+180-fRotateAngle)/180*PI);
// 	vector3[0].y = ptC.y - fRadius*sin((fRotateVecZ+180-fRotateAngle)/180*PI);
// 	//右上角
// 	vector3[1].x = ptC.x + fRadius*cos((fRotateVecZ+fRotateAngle)/180*PI);
// 	vector3[1].y = ptC.y - fRadius*sin((fRotateVecZ+fRotateAngle)/180*PI);
// 	//右下角
// 	vector3[2].x = ptC.x + fRadius*cos((fRotateVecZ+360-fRotateAngle)/180*PI);
// 	vector3[2].y = ptC.y - fRadius*sin((fRotateVecZ+360-fRotateAngle)/180*PI);
// 	//左下角
// 	vector3[3].x = ptC.x + fRadius*cos((fRotateVecZ+180+fRotateAngle)/180*PI);
// 	vector3[3].y = ptC.y + - fRadius*sin((fRotateVecZ+180+fRotateAngle)/180*PI);

	RECT rcDest = {pt[0].x,pt[0].y,pt[2].x,pt[2].y};
	OffsetRect(&rcSrc,-rcSrc.left,-rcSrc.top);
	if (pJob->m_bClip) //裁剪矩形区域
	{
		RECT rcTmp = rcDest;
		//IntersectRect(&rcDest,&rcDest,&(pJob->m_rcClip));  // 计算需要绘制的矩形位置
		if (rcDest.left < pJob->m_rcClip.left)
		{
			rcDest.left = pJob->m_rcClip.left;
		}
		if (rcDest.top < pJob->m_rcClip.top)
		{
			rcDest.top = pJob->m_rcClip.top;
		}
		if (rcDest.right > pJob->m_rcClip.right)
		{
			rcDest.right = pJob->m_rcClip.right;
		}
		if (rcDest.bottom > pJob->m_rcClip.bottom)
		{
			rcDest.bottom = pJob->m_rcClip.bottom;
		}
		rcSrc.left = (LONG)((rcDest.left - rcTmp.left)/fZtrans);
		rcSrc.top = (LONG)((rcDest.top - rcTmp.top)/fZtrans);
		rcSrc.right = rcSrc.right - (LONG)((rcTmp.right - rcDest.right)/fZtrans);
		rcSrc.bottom = rcSrc.bottom - (LONG)((rcTmp.bottom - rcDest.bottom)/fZtrans);
	}

	BLENDFUNCTION ftn = { 0 };
	ftn.BlendOp = AC_SRC_OVER;                                                        // 目前只能设置这个值
	ftn.AlphaFormat = AC_SRC_ALPHA;													  // 也只能设置这个值
	ftn.BlendFlags = 0;                                                               // 必须为0
	ftn.SourceConstantAlpha = 255;                                                    // 指定源图片的alpha

	typedef BOOL (WINAPI *LPALPHABLEND)(HDC, int, int, int, int,HDC, int, int, int, int, BLENDFUNCTION);
	static LPALPHABLEND lpAlphaBlend = (LPALPHABLEND) ::GetProcAddress(::GetModuleHandle(_T("msimg32.dll")), "AlphaBlend");

	if( lpAlphaBlend == NULL ) lpAlphaBlend = AlphaBitBlt;

	lpAlphaBlend(hDC,rcDest.left,rcDest.top,rcDest.right-rcDest.left,rcDest.bottom-rcDest.top,pJob->m_hDestDC,rcSrc.left,rcSrc.top,rcSrc.right-rcSrc.left,rcSrc.bottom-rcSrc.top,ftn);

	return true;
}

bool CAnimationSpooler::IsInitialized()
{
	return m_bIsInitialized;
}

void CAnimationSpooler::ReleaseGDI()
{
	if (m_hSurfaceBmp != NULL)
	{
		DeleteBitmap(m_hSurfaceBmp);
		m_hSurfaceBmp = NULL;
	}
	if (m_hSurfaceDC != NULL)
	{
		DeleteDC(m_hSurfaceDC);
		m_hSurfaceDC = NULL;
	}
	if (m_hCanvasBmp != NULL)
	{
		DeleteBitmap(m_hCanvasBmp);
		m_hCanvasBmp = NULL;
	}
	if (m_hCanvasDC != NULL)
	{
		DeleteDC(m_hCanvasDC);
		m_hCanvasDC = NULL;
	}
}

/*
double CAnimationSpooler::LinearInterpolate(double y1, double y2, double fPrecent)
{
	return y1 * (1.0 - fPrecent) + y2 * fPrecent;
}

double CAnimationSpooler::CosineInterpolate(double y1, double y2, double fPrecent)
{
	double mu2 = (1.0 - cos(fPrecent * PI)) / 2.0;
	return y1 * (1.0 - mu2) + y2 * mu2;
}
*/
}