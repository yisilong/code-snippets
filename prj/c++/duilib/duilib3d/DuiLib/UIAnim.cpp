
#include "stdafx.h"
#include "UIAnim.h"

#if (_MSC_VER < 1300)
#pragma comment(lib, "Delayimp.lib")
#pragma comment(linker, "/DelayLoad:d3d9.dll")
#pragma comment(linker, "/DelayLoad:D3DX9_43.dll")
#endif // _MSC_VER
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib,"d3dx9.lib")

/////////////////////////////////////////////////////////////////////////////////////

namespace DuiLib{

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#ifndef PI
#define PI 3.1415926535897932384626433832795029L
#endif

const RECT CAnimJobUI::s_cEmptyRect = {0,0,0,0};

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

	m_eSpeedType = SPEED_HYPERBOLA;

	m_eTexFormType = TEXTURE_CAPTURE;
	m_strImgPath = _T("");

	m_bDrawBack = false;
	m_bCullFace = false;

	m_bUseMaskBk = false;

	m_pTexture = NULL;
	m_bIsCompleted = false;
	m_hMaskDC = NULL;
	m_hMaskBmp = NULL;
	m_hDestDC = NULL;
	m_hDestBmp = NULL;
}

CAnimJobUI::~CAnimJobUI()
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
	}
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

bool CAnimJobUI::InitBkInfo( HWND hWnd )
{
	if (!IsWindow(hWnd))
	{
		return false;
	}

	RECT rcClient;
	GetClientRect(hWnd,&rcClient);
	HDC hdc= GetDC(hWnd);

	HDC hTmpDC = CreateCompatibleDC(hdc);
	HBITMAP hTmpBmp = CreateCompatibleBitmap(hdc,rcClient.right,rcClient.bottom);
	SelectBitmap(hTmpDC,hTmpBmp);
	SendMessage(hWnd,WM_PRINTCLIENT,(WPARAM)hTmpDC,PRF_CHECKVISIBLE | PRF_CLIENT | PRF_ERASEBKGND | PRF_CHILDREN);

	int cx = m_rcCaptureSrc.right-m_rcCaptureSrc.left;
	int cy = m_rcCaptureSrc.bottom-m_rcCaptureSrc.top;
	if (m_hMaskBmp == NULL)
	{
		m_hMaskBmp = CreateCompatibleBitmap(hdc,cx,cy);
	}
	if (m_hMaskDC == NULL)
	{
		m_hMaskDC = CreateCompatibleDC(hdc);
	}
	SelectBitmap(m_hMaskDC,m_hMaskBmp);
	BitBlt(m_hMaskDC,0,0,cx,cy,hTmpDC,m_rcCaptureSrc.left,m_rcCaptureSrc.top,SRCCOPY);

	DeleteBitmap(hTmpBmp);
	DeleteDC(hTmpDC);
	ReleaseDC(hWnd,hdc);

	return true;
}

bool CAnimJobUI::InitDestInfo( HDC hDC )
{
	int cx = m_rcCaptureSrc.right-m_rcCaptureSrc.left;
	int cy = m_rcCaptureSrc.bottom-m_rcCaptureSrc.top;
	if (m_hDestBmp == NULL)
	{
		m_hDestBmp = CreateCompatibleBitmap(hDC,cx,cy);
	}
	if (m_hDestDC == NULL)
	{
		m_hDestDC = CreateCompatibleDC(hDC);
	}
	SelectBitmap(m_hDestDC,m_hDestBmp);
	BitBlt(m_hDestDC,0,0,cx,cy,hDC,m_rcCaptureSrc.left,m_rcCaptureSrc.top,SRCCOPY);
	return true;
}


bool CAnimJobUI::AdjustTargetTexture(HWND hWnd)
{
	if (m_eTexFormType != TEXTURE_CAPTURE)
	{
		return true;
	}

	int cx = m_rcCaptureSrc.right-m_rcCaptureSrc.left;
	int cy = m_rcCaptureSrc.bottom-m_rcCaptureSrc.top;
	if (!m_bUseMaskBk)
	{
		BITMAPINFO MyBMInfo = {0};
		MyBMInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		// Get the BITMAPINFO structure from the bitmap
		int nRet = GetDIBits(m_hDestDC, m_hDestBmp, 0, 0, NULL, &MyBMInfo, DIB_RGB_COLORS);
		// create the bitmap buffer
		BYTE* lpPixelsDest = new BYTE[MyBMInfo.bmiHeader.biSizeImage];
		// set up the structure of the buffer to receive by setting up the BITMAPINFOHEADER structure
		MyBMInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		MyBMInfo.bmiHeader.biBitCount = 32;  // no stride necessary, although you won't use the alpha value provided
		MyBMInfo.bmiHeader.biCompression = BI_RGB;  // no compression -> easier to parse
		MyBMInfo.bmiHeader.biHeight = (MyBMInfo.bmiHeader.biHeight < 0) ? (-MyBMInfo.bmiHeader.biHeight) : (MyBMInfo.bmiHeader.biHeight);  // correct the bottom-up ordering of lines
		// get the actual bitmap buffer
		if(0 == GetDIBits(m_hDestDC, m_hDestBmp, 0, MyBMInfo.bmiHeader.biHeight, (LPVOID)lpPixelsDest, &MyBMInfo, DIB_RGB_COLORS))
		{
			delete []lpPixelsDest;
			return false;
		}

		// Lock the texture and scan through each pixel, replacing the with new pixels
		DWORD* pDestData = (DWORD*)lpPixelsDest;
		D3DLOCKED_RECT d3dlr;
		HRESULT Hr = m_pTexture->LockRect(0, &d3dlr, 0, 0);
		if( FAILED(Hr) ) 
		{
			delete []lpPixelsDest;
			return false;
		}
		DWORD* pBits = static_cast<DWORD*>(d3dlr.pBits);
		pBits+=(cy-1)*d3dlr.Pitch / sizeof(DWORD);
		for( int y = 0; y < cy; ++y ) {
			for( int x = 0; x < cx; ++x ) {
				pBits[x] = *pDestData | 0xFF000000;
				pDestData++;
			}
			pBits -= d3dlr.Pitch / sizeof(DWORD);
		}
		m_pTexture->UnlockRect(0);

#ifdef _DEBUG
		StretchDIBits(m_hDestDC,0,0,cx,cy,0,0,cx,cy,lpPixelsDest,&MyBMInfo,DIB_RGB_COLORS,SRCCOPY);
		if(OpenClipboard(hWnd))
		{
			EmptyClipboard();
			//复制数据到剪贴板
			SetClipboardData(CF_BITMAP,m_hDestBmp);
			CloseClipboard();
		}
#endif

		delete []lpPixelsDest;
		return true;
	}

	if (!InitBkInfo(hWnd))
	{
		return false;
	}

#ifdef _DEBUG
//	StretchDIBits(m_hDestDC,0,0,cx,cy,0,0,cx,cy,lpPixelsDest,&MyBMInfo,DIB_RGB_COLORS,SRCCOPY);
	if(OpenClipboard(hWnd))
	{
		EmptyClipboard();
		//复制数据到剪贴板
		SetClipboardData(CF_BITMAP,m_hMaskBmp);
		CloseClipboard();
	}
#endif

#ifdef _DEBUG
//	StretchDIBits(m_hDestDC,0,0,cx,cy,0,0,cx,cy,lpPixelsDest,&MyBMInfo,DIB_RGB_COLORS,SRCCOPY);
	if(OpenClipboard(hWnd))
	{
		EmptyClipboard();
		//复制数据到剪贴板
		SetClipboardData(CF_BITMAP,m_hDestBmp);
		CloseClipboard();
	}
#endif

	/******************************************************************************/
	BITMAPINFO MyBMInfo = {0};
	MyBMInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	// Get the BITMAPINFO structure from the bitmap
	int nRet = GetDIBits(m_hDestDC, m_hDestBmp, 0, 0, NULL, &MyBMInfo, DIB_RGB_COLORS);
	// create the bitmap buffer
	BYTE* lpPixelsDest = new BYTE[MyBMInfo.bmiHeader.biSizeImage];
	// set up the structure of the buffer to receive by setting up the BITMAPINFOHEADER structure
	MyBMInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	MyBMInfo.bmiHeader.biBitCount = 32;  // no stride necessary, although you won't use the alpha value provided
	MyBMInfo.bmiHeader.biCompression = BI_RGB;  // no compression -> easier to parse
	MyBMInfo.bmiHeader.biHeight = (MyBMInfo.bmiHeader.biHeight < 0) ? (-MyBMInfo.bmiHeader.biHeight) : (MyBMInfo.bmiHeader.biHeight);  // correct the bottom-up ordering of lines
	// get the actual bitmap buffer
	if(0 == GetDIBits(m_hDestDC, m_hDestBmp, 0, MyBMInfo.bmiHeader.biHeight, (LPVOID)lpPixelsDest, &MyBMInfo, DIB_RGB_COLORS))
	{
		delete []lpPixelsDest;
		return false;
	}

	BYTE* lpPixelsMask = new BYTE[MyBMInfo.bmiHeader.biSizeImage];
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

	D3DLOCKED_RECT d3dlr;
	HRESULT Hr = m_pTexture->LockRect(0, &d3dlr, 0, 0);
	if( FAILED(Hr) ) 
	{
		delete []lpPixelsMask;
		delete []lpPixelsDest;
		return false;
	}
	DWORD* pBits = static_cast<DWORD*>(d3dlr.pBits);
	pBits+=(cy-1)*d3dlr.Pitch / sizeof(DWORD);
	for( int y = 0; y < cy; ++y ) {
		for( int x = 0; x < cx; ++x ) {
			pBits[x] = *pDestData;
			pDestData++;
		}
		pBits -= d3dlr.Pitch / sizeof(DWORD);
	}
	m_pTexture->UnlockRect(0);

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

void CAnimJobUI::SetDrawMode( bool bDrawBack,bool bCullFace )
{
	m_bDrawBack = bDrawBack;
	m_bCullFace = bCullFace;
}

void CAnimJobUI::SetUseMaskBk()
{
	m_bUseMaskBk = true;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//
CAnimationSpooler::CAnimationSpooler() :
m_hWnd(NULL),
m_bIsAnimating(false),
m_bIsInitialized(false),
m_pD3D(NULL),
m_p3DDevice(NULL),
m_p3DSurface(NULL)
{
}

CAnimationSpooler::~CAnimationSpooler()
{
	RemoveJobs();
}

bool CAnimationSpooler::Init(HWND hWnd)
{
	ASSERT(::IsWindow(hWnd));
	if( m_bIsInitialized ) return true;
	// This is our master window
	m_hWnd = hWnd;
	// Gather window information
	RECT rcWindow = { 0 };
	::GetWindowRect(hWnd, &rcWindow);
	if( ::IsRectEmpty(&rcWindow) ) return false;
	// Is window topmost?
	HWND hWndFocus = hWnd;
	while( ::GetParent(hWndFocus) != NULL ) hWndFocus = ::GetParent(hWndFocus);
	// Is DirectX v9 available at all?
	HMODULE hMod = ::LoadLibrary(_T("D3D9.DLL"));
	if( hMod == NULL ) return false;
	::FreeLibrary(hMod);
	hMod = ::LoadLibrary(_T("D3DX9_43.DLL"));
	if( hMod == NULL ) return false;
	::FreeLibrary(hMod);
	// Initialize Direct3D
	// Fortunately we can delay-load the DirectX9 library so we
	// don't actually have a link dependency on it.
	if( m_pD3D != NULL ) m_pD3D->Release();
	m_pD3D = ::Direct3DCreate9(D3D_SDK_VERSION);
	if( m_pD3D == NULL ) return false;
	D3DDISPLAYMODE d3ddm = { 0 };
	HRESULT Hr = m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	if( FAILED(Hr) ) return false;
	m_clrFormat = d3ddm.Format;
	Hr = m_pD3D->CheckDeviceType(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		m_clrFormat,
		m_clrFormat,
		TRUE);
	if( FAILED(Hr) ) return false;

	D3DPRESENT_PARAMETERS d3dpp = { 0 };
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;; //D3DSWAPEFFECT_FLIP
	d3dpp.Windowed = TRUE;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = m_clrFormat;
	d3dpp.EnableAutoDepthStencil = FALSE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
//	d3dpp.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES;
	Hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWndFocus,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_p3DDevice);
	if( FAILED(Hr) ) return false;       
	// Check device caps
	D3DCAPS9 caps;
	Hr = m_p3DDevice->GetDeviceCaps(&caps);
	if( caps.MaxTextureWidth < 128 ) return false;
	if( (caps.Caps3 & D3DCAPS3_COPY_TO_VIDMEM) == 0 ) return false;
	if( FAILED(Hr) ) return false;
	// Set viewport
	D3DVIEWPORT9 vp;
	vp.X = 0;
	vp.Y = 0;
	vp.Width = rcWindow.right - rcWindow.left;
	vp.Height = rcWindow.bottom - rcWindow.top;
	vp.MinZ = 0.0;
	vp.MaxZ = 1.0;
	Hr = m_p3DDevice->SetViewport(&vp);
	if( FAILED(Hr) ) return false;
	// Set the render flags.
	m_p3DDevice->SetRenderState(D3DRS_COLORVERTEX, TRUE);
	m_p3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_p3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_p3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	
	// Set miscellaneous render states
	m_p3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_p3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_p3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_p3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_p3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
	m_p3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_p3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); 
	m_p3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);     // Use m_iAlpha from texture
	m_p3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);     // Use vertex m_iAlpha
	m_p3DDevice->SetVertexShader(NULL);

// 	m_p3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_ANISOTROPIC);
// 	m_p3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_ANISOTROPIC);
// 
// 	m_p3DDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	// Signal go...
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
		if (!MakeTexture(pJob))
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
	int s = m_aJobs.GetSize();
	for( int i = 0; i < m_aJobs.GetSize(); i++ ) 
	{
		CAnimJobUI* pAniJobUI = static_cast<CAnimJobUI*>(m_aJobs[i]);
 		if (pAniJobUI->m_uCallBackMsg > WM_USER && IsWindow(m_hWnd))
 		{
			WPARAM wParam = (pAniJobUI->m_bIsCompleted) ? 1:0;
 			SendMessage(m_hWnd,pAniJobUI->m_uCallBackMsg,(WPARAM)pAniJobUI->m_bIsCompleted,(LPARAM)pAniJobUI->m_pMsgCallBackData);
 		}	
// 
// 		if (pAniJobUI->m_pTexture != NULL)
// 		{
// 			pAniJobUI->m_pTexture->Release();
// 		}
		delete pAniJobUI;
	}
	m_aJobs.Empty();
	// Release Direct3D references
	if( m_p3DSurface != NULL ) m_p3DSurface->Release();
	m_p3DSurface = NULL;
	if( m_p3DDevice != NULL ) m_p3DDevice->Release();
	m_p3DDevice = NULL;
	if( m_pD3D != NULL ) m_pD3D->Release();
	m_pD3D = NULL;
	// Almost done...
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

bool CAnimationSpooler::PrepareAnimation()
{
	if (!CaptureSurface())
	{
		return false;
	}
	// Allow each job to prepare its 3D objects
	for( int i = 0; i < m_aJobs.GetSize(); i++ ) {
		CAnimJobUI* pJob = static_cast<CAnimJobUI*>(m_aJobs[i]);
		if( !PrepareJob_Flat(pJob) ) return false;
	}
	// Assign start time
	DWORD dwTick = ::timeGetTime();
	for( int j = 0; j < m_aJobs.GetSize(); j++ ) {
		CAnimJobUI* pJob = static_cast<CAnimJobUI*>(m_aJobs[j]);
		pJob->m_dwStartTick += dwTick;
	}
	m_bIsAnimating = true;
	return true;
}

bool CAnimationSpooler::Render()
{	
	if( !m_bIsInitialized) return false;
	if( !m_bIsAnimating ) return false;
	// Get render target
	LPDIRECT3DSURFACE9 p3DTargetSurface;
	HRESULT Hr = m_p3DDevice->GetRenderTarget(0, &p3DTargetSurface);
	if( FAILED(Hr) ) return false;
	CSafeRelease<IDirect3DSurface9> RefTargetSurface = p3DTargetSurface;
	// Copy backdrop
	Hr = m_p3DDevice->Clear(0, NULL, D3DCLEAR_TARGET/* | D3DCLEAR_ZBUFFER*/, D3DCOLOR_XRGB(0,0,255), 1.0f, 0L);
	Hr = m_p3DDevice->UpdateSurface(m_p3DSurface, NULL, p3DTargetSurface, NULL);
	// Here begins the rendering loop.
	Hr = m_p3DDevice->BeginScene();
	if( FAILED(Hr) ) return false;
	int nAnimated = 0;
	DWORD dwTick = ::timeGetTime();
	for( int i = 0; i < m_aJobs.GetSize(); i++ ) {
		CAnimJobUI* pJob = static_cast<CAnimJobUI*>(m_aJobs[i]);
		if( dwTick < pJob->m_dwStartTick ) {nAnimated++; continue;} //还没到要绘制时间
		DWORD dwTickNow = MIN(dwTick, pJob->m_dwStartTick + pJob->m_dwDuration);
		RenderJob_Flat(pJob, p3DTargetSurface, dwTickNow);
		if( dwTick < pJob->m_dwStartTick + pJob->m_dwDuration ) nAnimated++;
	}
	m_p3DDevice->EndScene();
	m_p3DDevice->Present(NULL, NULL, NULL, NULL);
	// No more frames to animate ?
	if( nAnimated == 0 ) RemoveJobs();
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//

COLORREF CAnimationSpooler::TranslateColor(LPDIRECT3DSURFACE9 pSurface, COLORREF clrColor) const
{
	ASSERT(pSurface);
	if( clrColor == CLR_INVALID ) return clrColor;
	// The only way to actually determine what color a certain RGB value gets, is
	// to put a pixel on the surface and taste it.
	HDC hDC = NULL;
	HRESULT Hr = pSurface->GetDC(&hDC);
	if( FAILED(Hr) ) return false;
	COLORREF clrOld = ::GetPixel(hDC, 0, 0);
	::SetPixel(hDC, 0, 0, clrColor);
	clrColor = ::GetPixel(hDC, 0,0);
	::SetPixel(hDC, 0, 0, clrOld);
	pSurface->ReleaseDC(hDC);
	return clrColor;
}

bool CAnimationSpooler::SetColorKey(LPDIRECT3DTEXTURE9 pTexture, LPDIRECT3DSURFACE9 pSurface, int iTexX,int iTexY, COLORREF clrColorKey)
{
	ASSERT(pTexture);
	ASSERT(pSurface);

	if( clrColorKey == CLR_INVALID ) return true;

	// Get colorkey's red, green, and blue components
	// and put the colorkey in the texture's native format
	BYTE r = GetRValue(clrColorKey);
	BYTE g = GetGValue(clrColorKey);
	BYTE b = GetBValue(clrColorKey);
	DWORD dwColorKey = D3DCOLOR_ARGB(255,r,g,b);

	LPDIRECT3DTEXTURE9 pTex = NULL;
	HRESULT Hr = m_p3DDevice->CreateTexture(iTexX, iTexY, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &pTex, NULL);
	if( FAILED(Hr) ) return false;
	CSafeRelease<IDirect3DTexture9> RefTex = pTex;
	LPDIRECT3DSURFACE9 pTexSurf = NULL;
	Hr = pTex->GetSurfaceLevel(0, &pTexSurf);
	if( FAILED(Hr) ) return false;
	CSafeRelease<IDirect3DSurface9> RefTexSurf = pTexSurf;
	Hr = m_p3DDevice->GetRenderTargetData(pSurface, pTexSurf);
	if( FAILED(Hr) ) return false;

	// Lock the texture and scan through each pixel, replacing the colorkey pixels
	D3DLOCKED_RECT d3dlr;
	Hr = pTex->LockRect(0, &d3dlr, 0, 0);
	if( FAILED(Hr) ) return false;
	DWORD* pBits = static_cast<DWORD*>(d3dlr.pBits);
	for( int y = 0; y < iTexY; y++ ) {
		for( int x = 0; x < iTexX; x++ ) {
			if( pBits[x] == dwColorKey ) pBits[x] = 0x00000000;
		}
		pBits += d3dlr.Pitch / sizeof(DWORD);
	}
	pTex->UnlockRect(0);

	// Copy modified data back
	POINT pt = { 0, 0 };
	RECT rcDest = { 0, 0, iTexX, iTexX };
	Hr = m_p3DDevice->UpdateSurface(pTexSurf, &rcDest, pSurface, &pt);
	return false;
}

bool CAnimationSpooler::CaptureSurface()
{
	if( !m_bIsInitialized) return false;
	// Release old image of window
	if( m_p3DSurface != NULL ) m_p3DSurface->Release();
	m_p3DSurface= NULL;
	// Create the backdrop surface
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	HRESULT Hr = m_p3DDevice->CreateOffscreenPlainSurface(rcClient.right, rcClient.bottom, m_clrFormat, D3DPOOL_SYSTEMMEM, &m_p3DSurface, NULL);
	if( FAILED(Hr) ) return false;
	// get the background
	HDC hDC = NULL;
	Hr = m_p3DSurface->GetDC(&hDC);
	if( FAILED(Hr) ) return false;
	::SendMessage(m_hWnd, WM_PRINTCLIENT, (WPARAM) hDC, PRF_CHECKVISIBLE | PRF_CLIENT | PRF_ERASEBKGND | PRF_CHILDREN);
	m_p3DSurface->ReleaseDC(hDC);
	return true;
}

bool CAnimationSpooler::MakeTexture(CAnimJobUI* pJob)
{
	RECT rcSrc = pJob->m_rcCaptureSrc;
	int cx = rcSrc.right - rcSrc.left;
	int cy = rcSrc.bottom - rcSrc.top;

	// Start building texture
	LPDIRECT3DTEXTURE9 pTextureSrc = NULL;
	HRESULT Hr = m_p3DDevice->CreateTexture(cx, cy, 1,D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8/*m_clrFormat*/, D3DPOOL_DEFAULT, &pTextureSrc, NULL);
	if( FAILED(Hr) ) return false;
// 	CSafeRelease<IDirect3DTexture9> RefTextureSrc = pTextureSrc;
// 	LPDIRECT3DSURFACE9 pSurfaceSrc = NULL;
// 	Hr = pTextureSrc->GetSurfaceLevel(0, &pSurfaceSrc);
// 	if( FAILED(Hr) ) return false;
// 	CSafeRelease<IDirect3DSurface9> RefSurfaceSrc = pSurfaceSrc;
// 	POINT pt = { 0, 0 };         
// 	Hr = m_p3DDevice->UpdateSurface(m_p3DSurface, &rcSrc, pSurfaceSrc, &pt); //拷贝指定区域
// 	if( FAILED(Hr) ) return false;
//	pJob->m_pTexture = RefTextureSrc.Detach();
	pJob->m_pTexture = pTextureSrc;
	// get the background
	HDC hDC = NULL;
	Hr = m_p3DSurface->GetDC(&hDC);
	if( FAILED(Hr) ) return false;
	bool bResult = pJob->InitDestInfo(hDC);
	m_p3DSurface->ReleaseDC(hDC);

	return bResult;
}
bool CAnimationSpooler::PrepareJob_Flat(CAnimJobUI* pJob)
{
	if (pJob->m_eTexFormType == TEXTURE_CAPTURE)
	{
		if(!pJob->m_bBeforePaint && !MakeTexture(pJob)) 
			return false;
	}
	else if (pJob->m_eTexFormType == TEXTURE_FILE)
	{
		D3DXIMAGE_INFO imgInfo;
		if (FAILED(D3DXGetImageInfoFromFile(pJob->m_strImgPath.GetData(),&imgInfo)))
		{
			return false;
		}
		pJob->m_rcCaptureSrc.top = 0;
		pJob->m_rcCaptureSrc.left = 0;
		pJob->m_rcCaptureSrc.right = imgInfo.Width;
		pJob->m_rcCaptureSrc.bottom = imgInfo.Height;
		LPDIRECT3DTEXTURE9 pTexture;
		if (FAILED(D3DXCreateTextureFromFile(m_p3DDevice,pJob->m_strImgPath.GetData(),&pTexture)))
		{
			return false;
		}
		pJob->m_pTexture = pTexture;
	}
	else
	{
		return false;
	}

	// adjust target bitmap
	if (!pJob->AdjustTargetTexture(m_hWnd))
	{
		return false;
	}

	// Clear the background so the sprite can take its place
	COLORREF clrBack = pJob->m_clrBack;
	if( clrBack != CLR_INVALID && pJob->m_eTexFormType == TEXTURE_CAPTURE) {
		HDC hDC = NULL;
		HRESULT Hr = m_p3DSurface->GetDC(&hDC);
		if( FAILED(Hr) ) return false;
		RECT rcSrc = pJob->m_rcCaptureSrc;
		HBRUSH hBrush = ::CreateSolidBrush(clrBack);
		::FillRect(hDC, &rcSrc, hBrush);
		::DeleteObject(hBrush);
		m_p3DSurface->ReleaseDC(hDC);
	}
	return true;
}

bool CAnimationSpooler::RenderJob_Flat(CAnimJobUI* pJob, LPDIRECT3DSURFACE9 /*pSurface*/, DWORD dwTick)
{	
	if(pJob->m_bIsCompleted) return true; //动画执行完了,等待删除
	if (pJob->m_pTexture == NULL) return false;

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

	float cxHalfSrcRect = (float)((rcSrc.right - rcSrc.left/*+1.0f*/)/2.0f);
	float cyHalfSrcRect = (float)((rcSrc.bottom - rcSrc.top/*+1.0f*/)/2.0f);
	float fRotateVecX = pJob->m_fRotateVecX;
	float fRotateVecY = pJob->m_fRotateVecY;
	float fRotateVecZ = pJob->m_fRotateVecZ;
	float fRotateAngle = (float)(pJob->m_fRotateAngle/180.0f*PI*(1.0f-fPrecent));
	float xTrans = (float)pJob->m_ptXYtrans.x*(1.0f-fPrecent);
	float yTrans = (float)pJob->m_ptXYtrans.y*(1.0f-fPrecent);	
	float fStartZtrans = fabsf(pJob->m_fStartZtrans);
	float fEndZtrans = fabsf(pJob->m_fEndZtrans);
	float fZtrans = (float)(fStartZtrans + (fEndZtrans - fStartZtrans)*(1.0f-fPrecent));

// 	if (pJob->m_fStartZtrans)
// 	{
// 		xTrans *= fPrecent;
// 		yTrans *= fPrecent;
// 		if(fZtrans > 1.0f)
// 		{
// 			fZtrans = (float)(1.0f + (fZtrans - 1.0f)*fPrecent);					//zTrans从最大减小到原大小
// 		}
// 		else if (fZtrans < 1.0f)
// 		{
// 			fZtrans = (float)(1.0f - (1.0f-fZtrans)*fPrecent);					//zTrans从最小增大到原大小
// 		}
// 		else																	//一直原大小
// 		{		
// 		}
// 	}
// 	else
// 	{
// 		xTrans = (float)(xTrans*(1.0f-fPrecent));
// 		yTrans = (float)(yTrans*(1.0f-fPrecent));
// 		if (fZtrans > 1.0f)
// 		{
// 			fZtrans = (float)(1.0f + (fZtrans - 1.0f)*(1.0f - fPrecent));			//zTrans从原始大小增大到最大
// 		}
// 		else if (fZtrans < 1.0f)
// 		{
// 			fZtrans = (float)(1.0f - (1.0f-fZtrans)*(1.0f - fPrecent));			//zTrans从原始大减小到最小
// 		}
// 		else																	//一直原大小
// 		{
// 		}
// 	}
// 	float fRadius = fZtrans*sqrt(cxHalfSrcRect*cxHalfSrcRect + cyHalfSrcRect*cyHalfSrcRect);  //旋转的半径
// 	float fRotateAngle = atan2f(cyHalfSrcRect,cxHalfSrcRect)*180/PI;						 //右上点与x轴所成角度

	D3DXVECTOR3 ptCenter((FLOAT)pJob->m_ptStartCenter.x,(FLOAT)pJob->m_ptStartCenter.y,0.0f);//原始控件中心点
	D3DXVECTOR3 ptC(ptCenter.x+xTrans,ptCenter.y+yTrans,0.0f);								 //移动后中心点

	D3DXVECTOR3 vector3[4];
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

	//左上角
	vector3[0].x = -fZtrans*cxHalfSrcRect;
	vector3[0].y = -fZtrans*cyHalfSrcRect;
	vector3[0].z = 0.0f;
	//右上角
	vector3[1].x = fZtrans*cxHalfSrcRect;
	vector3[1].y = -fZtrans*cyHalfSrcRect;
	vector3[1].z = 0.0f;
	//右下角
	vector3[2].x = fZtrans*cxHalfSrcRect;
	vector3[2].y = fZtrans*cyHalfSrcRect;
	vector3[2].z = 0.0f;
	//左下角
	vector3[3].x = -fZtrans*cxHalfSrcRect;
	vector3[3].y = fZtrans*cyHalfSrcRect;
	vector3[3].z = 0.0f;

 	D3DXVECTOR3 vec3(fRotateVecX,fRotateVecY,fRotateVecZ);
	D3DXMATRIX matWorld;
	D3DXMatrixRotationAxis(&matWorld,&vec3,fRotateAngle);
	D3DXVec3TransformCoordArray(vector3,sizeof(D3DXVECTOR3),vector3,sizeof(D3DXVECTOR3),&matWorld,4);

	for (int i=0;i<4;i++)
	{
		vector3[i].x += ptC.x;
		vector3[i].y += ptC.y;
		vector3[i].x -= 0.5f;
		vector3[i].y -= 0.5f;
	}

	FLOAT z = 0.1f;
	FLOAT rhw = 1.0f / (z * 990.0f + 10.0f);

	LPDIRECT3DVERTEXBUFFER9 pVertexBuffer = NULL;
	HRESULT Hr = m_p3DDevice->CreateVertexBuffer(sizeof(CUSTOMVERTEXRECT), D3DUSAGE_WRITEONLY, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &pVertexBuffer, NULL);
	if( FAILED(Hr) ) return false;
	CSafeRelease<IDirect3DVertexBuffer9> RefVertex = pVertexBuffer;
	void* pData;
	if (FAILED(pVertexBuffer->Lock(0,sizeof(CUSTOMVERTEXRECT),&pData,0)))
	{
		return false;
	}
	if (!pJob->m_bDrawBack)//正面画图
	{
		CUSTOMVERTEXRECT verts = 
		{
			{ vector3[0].x, vector3[0].y, vector3[0].z, rhw, clrAlpha, 0.0f, 0.0f }, //左上角
			{ vector3[1].x, vector3[1].y, vector3[1].z, rhw, clrAlpha, 1.0f, 0.0f }, //右上角
			{ vector3[2].x, vector3[2].y, vector3[2].z, rhw, clrAlpha, 1.0f, 1.0f }, //右下角
			{ vector3[3].x, vector3[3].y, vector3[3].z, rhw, clrAlpha, 0.0f, 1.0f }  //左下角
		};		
		memcpy(pData,verts,sizeof(CUSTOMVERTEXRECT));
	}
	else //在背面画图
	{
		CUSTOMVERTEXRECT verts = 
		{
			{ vector3[0].x, vector3[0].y, vector3[0].z, rhw, clrAlpha, 1.0f, 0.0f }, //左上角
			{ vector3[3].x, vector3[3].y, vector3[3].z, rhw, clrAlpha, 1.0f, 1.0f }, //右上角
			{ vector3[2].x, vector3[2].y, vector3[2].z, rhw, clrAlpha, 0.0f, 1.0f }, //右下角
			{ vector3[1].x, vector3[1].y, vector3[1].z, rhw, clrAlpha, 0.0f, 0.0f }  //左下角
		};
		memcpy(pData,verts,sizeof(CUSTOMVERTEXRECT));
	}

	pVertexBuffer->Unlock();

	if (pJob->m_bClip) //裁剪纹理
	{
		D3DVIEWPORT9 viewPort;
		viewPort.X = pJob->m_rcClip.left;
		viewPort.Y = pJob->m_rcClip.top;
		viewPort.Width = pJob->m_rcClip.right - pJob->m_rcClip.left;
		viewPort.Height = pJob->m_rcClip.bottom - pJob->m_rcClip.top;
		viewPort.MaxZ = 1.0f;
		viewPort.MinZ = 0.0f;
		m_p3DDevice->SetViewport(&viewPort);
	}

	if (pJob->m_bCullFace) // 剔除表面
	{
		m_p3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); //剔除表面
	}
	Hr = m_p3DDevice->SetStreamSource(0, pVertexBuffer, 0, sizeof(CUSTOMVERTEX));
	Hr = m_p3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	Hr = m_p3DDevice->SetTexture(0, pJob->m_pTexture);
	Hr = m_p3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
	if (pJob->m_bCullFace) // 剔除表面
	{
		m_p3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);//还原
	}
	if (pJob->m_bClip) //裁剪纹理
	{
		RECT rcWnd;
		GetWindowRect(m_hWnd,&rcWnd);
		D3DVIEWPORT9 viewPort;
		viewPort.X = 0;
		viewPort.Y = 0;
		viewPort.Width = rcWnd.right - rcWnd.left;
		viewPort.Height = rcWnd.bottom - rcWnd.top;
		viewPort.MaxZ = 1.0f;
		viewPort.MinZ = 0.0f;
		m_p3DDevice->SetViewport(&viewPort);
	}
	return true;
}

bool CAnimationSpooler::IsInitialized()
{
	return m_bIsInitialized;
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
