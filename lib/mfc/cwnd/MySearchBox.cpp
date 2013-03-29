// MySearchBox.cpp : implementation file
//

#include "stdafx.h"
#include "MySearchBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_EDIT 1
#define IDC_BTN	2

/////////////////////////////////////////////////////////////////////////////
// CMySearchBox

CMySearchBox::CMySearchBox()
{
}

CMySearchBox::~CMySearchBox()
{
}


BEGIN_MESSAGE_MAP(CMySearchBox, CWnd)
	//{{AFX_MSG_MAP(CMySearchBox)
	ON_WM_CREATE()
    ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN, OnButtonClick)
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySearchBox message handlers

int CMySearchBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_pEdit.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, CRect(0, 0, 0, 0),this, IDC_EDIT);
	m_pBtn.Create(_T("S"),WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0),this,	IDC_BTN);
	

	//设置字体
	HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	CFont* pFont = CFont::FromHandle(hFont);
	m_pEdit.SetFont(pFont);
	m_pBtn.SetFont(pFont);

	LOGFONT lf;
	pFont->GetLogFont(&lf);
	return 0;
}

void CMySearchBox::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	//调整两控件大小
	int nBtnWidth = 30;
	m_pEdit.MoveWindow(5,(cy-12)/2,cx-nBtnWidth-10,12);
	m_pBtn.MoveWindow(cx - nBtnWidth, 1, nBtnWidth, cy - 2);
}

void CMySearchBox::OnNcPaint()
{
	CPaintDC dc(this);
	CRect rcWnd;
	GetWindowRect(&rcWnd);
	rcWnd.SetRect(0, 0, rcWnd.Width(), rcWnd.Height());

	dc.SelectStockObject(BLACK_PEN);
	dc.SelectStockObject(WHITE_BRUSH);
	dc.Rectangle(&rcWnd);
}

//////////////////////////////////////////////////////////////////////////
//处理搜索按钮
void CMySearchBox::OnButtonClick() 
{
	// TODO: Add your message handler code here and/or call default
	::SendMessage(::GetParent(m_hWnd), UM_BEGINSEARCH, 0, 0);
}

//////////////////////////////////////////////////////////////////////////
//外部调用
//////////////////////////////////////////////////////////////////////////
BOOL CMySearchBox::Create(DWORD dwStyle, const RECT &rcLayout,
						   CWnd *pParentWnd, UINT nID)
{
	int nRight = rcLayout.right;
	if(nRight - rcLayout.left < 24)
		nRight = rcLayout.left + 28;

	CRect rect(rcLayout.left,rcLayout.top,nRight,rcLayout.bottom);

	WNDCLASS wndcls;	
    memset(&wndcls, 0, sizeof(WNDCLASS));   // start with NULL
    // defaults	
    wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_NOCLOSE;	
    //you can specify your own window procedure
    wndcls.lpfnWndProc = ::DefWindowProc; 
    wndcls.hInstance = AfxGetInstanceHandle();
    wndcls.hIcon =  NULL; // or load a different icon
    wndcls.hCursor =NULL;
    wndcls.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);//(HBRUSH) (COLOR_WINDOW + 1);
    wndcls.lpszMenuName = NULL;
	
    // Specify your own class name for using FindWindow later
    wndcls.lpszClassName = _T("likebeta_SearchBox");
	
	LPCTSTR lpszClassName = _T("likebeta_SearchBox");
    // Register the new class and exit if it fails
    if(!AfxRegisterClass(&wndcls))
    {
        lpszClassName= AfxRegisterWndClass(0);
    }	
	return CWnd::Create(lpszClassName, 	_T("likebeta_SearchBox"),dwStyle, rect,	pParentWnd,	nID, NULL);
}
