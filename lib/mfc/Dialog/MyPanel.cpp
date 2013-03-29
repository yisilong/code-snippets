// MyPanel.cpp : implementation file
//

#include "stdafx.h"
#include "MyPanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyPanel dialog


CMyPanel::CMyPanel(UINT nIDTemplate,CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	//{{AFX_DATA_INIT(CMyPanel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
    // Create the scroll helper and attach it to this dialog.
    m_pScrollHelper = new CScrollHelper;
    m_pScrollHelper->AttachWnd(this);
    // Create the dialog.
}

CMyPanel::~CMyPanel()
{
}


void CMyPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPanel)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyPanel, CDialog)
	//{{AFX_MSG_MAP(CMyPanel)
	ON_WM_NCDESTROY()
	ON_WM_MOUSEWHEEL()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPanel message handlers

void CMyPanel::OnNcDestroy() 
{
	CDialog::OnNcDestroy();
	
	// TODO: Add your message handler code here
	m_pScrollHelper->DetachWnd();
	
    // Delete the C++ instance so the parent does not have
    // to worry about it.
    delete m_pScrollHelper;
	delete this;
}

BOOL CMyPanel::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL bResult=m_pScrollHelper->OnMouseWheel(nFlags,zDelta,pt);
	return bResult;//CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CMyPanel::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	m_pScrollHelper->OnHScroll(nSBCode,nPos,pScrollBar);
//	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMyPanel::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	m_pScrollHelper->OnVScroll(nSBCode,nPos,pScrollBar);
//	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CMyPanel::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
    m_pScrollHelper->OnSize(nType, cx, cy);
}

void   CMyPanel::SetDisplaySize(int displayWidth, int displayHeight)
{
	m_pScrollHelper->SetDisplaySize(displayWidth,displayHeight);
}
CSize CMyPanel::GetDisplaySize() const
{
    return m_pScrollHelper->GetDisplaySize();
}

BOOL CMyPanel::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rcMe;
	GetClientRect(rcMe);
	m_nDisplayWidth=rcMe.Width();
	m_nDisplayHeight=5;
	SetDisplaySize(m_nDisplayWidth,m_nDisplayHeight);
	ShowWindow(SW_SHOW);
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void   CMyPanel::OffsetDispSize(int nWidth,int nHeight)
{
	m_nDisplayWidth+=nWidth;
	m_nDisplayHeight+=nHeight;
	SetDisplaySize(m_nDisplayWidth,m_nDisplayHeight);
}
CSize CMyPanel::GetScrollPos() const
{
    return m_pScrollHelper->GetScrollPos();
}
