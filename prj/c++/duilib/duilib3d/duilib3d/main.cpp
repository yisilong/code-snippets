#include "Duilib3dFrame.h"
int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE, __in_opt LPSTR lpCmdLine, __in int nShowCmd )
{
	//CWndShadow::Initialize(hInstance);//阴影窗口
	CPaintManagerUI::SetInstance(hInstance);


	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));
	//CPaintManagerUI::SetResourceZip(_T("360SafeRes.zip"));
	//GdiplusStartupInput   gdiplusStartupInput;
// 	ULONG_PTR               gdiplusToken;
// 	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;

// 	if( ::LoadLibrary(_T("d3d9.dll")) == NULL ) 
// 		::MessageBox(NULL, _T("Denne applikation vises bedst med DirectX 9 installeret!"), _T("Test"), MB_ICONINFORMATION);

	CDuilib3dFrame* pFrame = new CDuilib3dFrame();
	if( pFrame == NULL ) return 0;
	pFrame->Create(NULL, _T("360安全卫士"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
	pFrame->CenterWindow();
	::ShowWindow(*pFrame, SW_SHOW);
	CPaintManagerUI::MessageLoop();

	::CoUninitialize();
//	GdiplusShutdown(gdiplusToken);
	return 0;
}