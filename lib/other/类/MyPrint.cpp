#include "StdAfx.h"
#include "MyPrint.h" 
//构造函数
//download by www.codefans.net
CMyPrint::CMyPrint ()
{
	m_CDC=new CDC();
	strDocName="打印";
	blnVal=FALSE;
	m_rcPrintMargin.left=25;
	m_rcPrintMargin.right=25;
	m_rcPrintMargin.top=40;
	m_rcPrintMargin.bottom=30;
	nXPixelsOfInch=300;
	nYPixelsOfInch=300;
	rcContent.left=m_rcPrintMargin.left;
	rcContent.top=m_rcPrintMargin.top;
	rcContent.right=600;
	rcContent.bottom=800;
}
//析构函数
CMyPrint::~CMyPrint()
{
	if (m_hDevNames != NULL)
        GlobalFree(m_hDevNames);
	if (m_hDevMode != NULL)
        GlobalFree(m_hDevMode);
	if(m_CDC->m_hDC!=NULL)
	{
		m_CDC->ReleaseOutputDC();
		m_CDC->~CDC();
	}		
}
//释放内存
void CMyPrint::FreeAllMemory()
{
	if (m_hDevNames != NULL)
        GlobalFree(m_hDevNames);
	if (m_hDevMode != NULL)
        GlobalFree(m_hDevMode);
	if(m_CDC->m_hDC!=NULL)
	{
		m_CDC->ReleaseOutputDC();
	}
}
//返回CDC指针
CDC* CMyPrint::GetMyCdc()
{
	//	打印设置
	GetPrinterDeviceDefaults();
	//	设置默认打印方向为横向
	SetPrintOrientation(blnVal);
	PRINTDLG	pd;
	memset(&pd, 0, sizeof(PRINTDLG));
	pd.lStructSize = sizeof(PRINTDLG);
	pd.Flags = PD_RETURNDC | PD_NOSELECTION;
	pd.hDC = NULL;
	pd.hwndOwner = NULL;
	pd.hInstance = NULL;
	pd.nMaxPage = 1;
	pd.nMinPage = 1;
	pd.nFromPage = 1;
	pd.nToPage = 1;
	pd.nCopies = 1;
	pd.hDevMode = NULL;
	pd.hDevNames = NULL;
	pd.hDevMode = m_hDevMode;
	//显示打印对话框，由用户来设定纸张大小等。
	if (!PrintDlg(&pd))
		return FALSE;
	ASSERT(pd.hDC != NULL);
	int nHorRes = GetDeviceCaps(pd.hDC, HORZRES); 
	int nVerRes = GetDeviceCaps(pd.hDC, VERTRES);
	nXPixelsOfInch = GetDeviceCaps(pd.hDC, LOGPIXELSX);
	nYPixelsOfInch = GetDeviceCaps(pd.hDC, LOGPIXELSY);
	//	计算页面可写面积
	rcContent.left=m_rcPrintMargin.left;
	rcContent.top=m_rcPrintMargin.top;
	rcContent.right=(long)(nHorRes*GetPrintProportionX()-m_rcPrintMargin.right);
	rcContent.bottom=(long)(nVerRes*GetPrintProportionY()-m_rcPrintMargin.bottom);
	//	开始打印
	DOCINFO di;
	di.cbSize = sizeof(DOCINFO);
	di.lpszDocName =strDocName; 
	di.lpszOutput = (LPTSTR) NULL; 
	di.lpszDatatype = (LPTSTR) NULL; 
	di.fwType = 0; 
	StartDoc(pd.hDC, &di);
	if(m_CDC->m_hDC!=NULL)
	{
		m_CDC->ReleaseOutputDC();
	}
	m_CDC->m_hDC=pd.hDC;
	return m_CDC;
}
//人为的获得打印机的驱动
CDC* CMyPrint::GetManCdc(CString strDriver,CString strDevice, CString strOutput, CONST DEVMODE* lpInitData)
{
	if(m_CDC->m_hDC!=NULL)
	{
		m_CDC->ReleaseOutputDC();
	}
	m_CDC->m_hDC=CreateDC((LPCSTR)strDriver,(LPCSTR)strDevice,(LPCSTR)strOutput,lpInitData);
	DOCINFO df={sizeof(DOCINFO),"打印",NULL};
	int nHorRes = GetDeviceCaps(m_CDC->m_hDC, HORZRES); 
	int nVerRes = GetDeviceCaps(m_CDC->m_hDC, VERTRES);
	nXPixelsOfInch = GetDeviceCaps(m_CDC->m_hDC, LOGPIXELSX);
	nYPixelsOfInch = GetDeviceCaps(m_CDC->m_hDC, LOGPIXELSY);
	//	计算页面可写面积
	rcContent.left=m_rcPrintMargin.left;
	rcContent.top=m_rcPrintMargin.top;
	rcContent.right=(long)(nHorRes*GetPrintProportionX()-m_rcPrintMargin.right);
	rcContent.bottom=(long)(nVerRes*GetPrintProportionY()-m_rcPrintMargin.bottom);
	m_CDC->StartDoc(&df);
	return m_CDC;
}
//得到系统的打印设备属性
BOOL CMyPrint::GetPrinterDeviceDefaults(void)
{
	PRINTDLG printDlg;
	if(AfxGetApp()->GetPrinterDeviceDefaults(&printDlg))
	{
		LPDEVNAMES pDevNames =
			(LPDEVNAMES)::GlobalLock(printDlg.hDevNames);
		LPDEVMODE  pDevMode  = (LPDEVMODE)::GlobalLock(printDlg.hDevMode);		
		if (m_hDevNames != NULL)
			::GlobalFree(m_hDevNames);
		if (m_hDevMode != NULL)
			::GlobalFree(m_hDevMode);		
		m_hDevNames = ::GlobalAlloc(GPTR,
			::GlobalSize(printDlg.hDevNames));
		m_hDevMode = ::GlobalAlloc(GPTR, ::GlobalSize(printDlg.hDevMode));		
		LPDEVNAMES pNewDevNames = (LPDEVNAMES)::GlobalLock(m_hDevNames);
		LPDEVMODE  pNewDevMode  = (LPDEVMODE)::GlobalLock(m_hDevMode);		
		memcpy(pNewDevNames, pDevNames,
			(size_t)::GlobalSize(printDlg.hDevNames));
		memcpy(pNewDevMode, pDevMode,
			(size_t)::GlobalSize(printDlg.hDevMode));		
		::GlobalUnlock(printDlg.hDevNames);
		::GlobalUnlock(printDlg.hDevMode);		
		::GlobalUnlock(m_hDevNames);
		::GlobalUnlock(m_hDevMode);
		return TRUE;
	}
	else
	{
		return FALSE;
    }
}
//	设置打印方向
//	TRUE:横向
//	FALSE:纵向
BOOL CMyPrint::SetPrintOrientation(BOOL bVal)
{
	if (m_hDevMode == NULL)
		return FALSE;
	LPDEVMODE  pNewDevMode  = (LPDEVMODE)::GlobalLock(m_hDevMode);
	if (bVal)
		pNewDevMode->dmOrientation = DMORIENT_LANDSCAPE;	//	横向
	else
		pNewDevMode->dmOrientation = DMORIENT_PORTRAIT;		//	纵向
	::GlobalUnlock(m_hDevMode);
	return TRUE;
}
//开始打印
void CMyPrint::DoPrint()
{
	m_CDC->EndDoc();
	m_CDC->DeleteDC();
}
//得到像素向英寸转换的比例
//像素:英寸=?:1
double CMyPrint::GetPrintProportionX()
{
	double pProportion;
	//分辨率 每英寸的象素点 1250
	pProportion=nXPixelsOfInch/1250.0;
	return pProportion;
}
//由像素转换成所需的英寸
int CMyPrint::IntToRealIntX(int i)
{
	int iTemp;
	iTemp=int(i*1.0/GetPrintProportionX());
	return iTemp;
}
double CMyPrint::GetPrintProportionY()
{
	double pProportion;
	//分辨率 每英寸的象素点 1250
	pProportion=nYPixelsOfInch/1250.0;
	return pProportion;
}
//由像素转换成所需的英寸
int CMyPrint::IntToRealIntY(int i)
{
	int iTemp;
	iTemp=int(i*1.0/GetPrintProportionY());
	return iTemp;
}