/////////////////////////////////////////////////////////////////////////////
// LBHscrollDemoDlg.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LBHscrollDemo.h"
#include "LBHscrollDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
/////////////////////////////////////////////////////////////////////////////
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	if (!pDX->m_bSaveAndValidate)
		SetDlgItemText(IDC_EDIT_URL, _T("www.nebutech.com"));
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLBHscrollDemoDlg dialog
/////////////////////////////////////////////////////////////////////////////
CLBHscrollDemoDlg::CLBHscrollDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLBHscrollDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

/////////////////////////////////////////////////////////////////////////////
void CLBHscrollDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HSCROLL_LB, m_listBox);
}

/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CLBHscrollDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUT_ADD_STR, OnBnClickedButAddStr)
	ON_BN_CLICKED(IDC_BUT_DIR, OnBnClickedButDir)
	ON_BN_CLICKED(IDC_BUT_RESET, OnBnClickedButReset)
	ON_BN_CLICKED(IDC_BUT_TABBED, OnBnClickedButTabbed)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLBHscrollDemoDlg message handlers
/////////////////////////////////////////////////////////////////////////////
BOOL CLBHscrollDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

/////////////////////////////////////////////////////////////////////////////
void CLBHscrollDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

/////////////////////////////////////////////////////////////////////////////
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
/////////////////////////////////////////////////////////////////////////////
void CLBHscrollDemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

/////////////////////////////////////////////////////////////////////////////
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
/////////////////////////////////////////////////////////////////////////////
HCURSOR CLBHscrollDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/////////////////////////////////////////////////////////////////////////////
void CLBHscrollDemoDlg::OnBnClickedButAddStr()
{
	CString csLine;
	GetDlgItemText(IDC_EDIT_ADD_STR, csLine);
	if (!csLine.IsEmpty())
		m_listBox.AddString(csLine);
}

/////////////////////////////////////////////////////////////////////////////
void CLBHscrollDemoDlg::OnBnClickedButDir()
{
	m_listBox.ResetContent();
	m_listBox.Dir(DDL_READWRITE | DDL_DIRECTORY, _T("C:\\*.*"));
}

/////////////////////////////////////////////////////////////////////////////
void CLBHscrollDemoDlg::OnBnClickedButReset()
{
	m_listBox.ResetContent();
}

/////////////////////////////////////////////////////////////////////////////
void CLBHscrollDemoDlg::OnBnClickedButTabbed()
{
	m_listBox.AddString(_T("This is a long tabbed string\twith a couple of tabs\t to show how the box \tcan handle \thorizontal scroll"));
}
