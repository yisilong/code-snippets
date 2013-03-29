#include "StdAfx.h"
#include "KanboxFrame.h"
#include "LoginDlg.h"
int APIENTRY _tWinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPTSTR lpCmdLine, __in int nShowCmd )
{
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));
	//CPaintManagerUI::SetResourceZip(_T("default.zip"));
	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;
	if (true)
	{
		CLoginDlg* pLoginDlg = new CLoginDlg();
		if (pLoginDlg == NULL) return 0;
		pLoginDlg->Create(NULL, _T("µÇÂ½"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 0, 0);
		pLoginDlg->CenterWindow();
		UINT uRet = pLoginDlg->ShowModal();
		if (uRet == IDCANCEL) //È¡ÏûµÇÂ½
		{
		}
		else
		{
			CKanboxFrame* pFrame = new CKanboxFrame();
			if( pFrame == NULL ) return 0;
			pFrame->Create(NULL, _T("kanbox¿áÅÌ±ãÐ¯°æ"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 0, 0);
			pFrame->CenterWindow();
			::ShowWindow(*pFrame, nShowCmd);

			CPaintManagerUI::MessageLoop();
		}
	}
	::CoUninitialize();
	return 0;
}