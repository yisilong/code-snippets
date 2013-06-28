///////////////////////////////////////////////////////////////////////////////
//
// ListBoxXP.cpp : implementation file
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TreeCtrlXP.h"
#include "Draw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////
CTreeCtrlXP::CTreeCtrlXP ()
{
}

////////////////////////////////////////////////////////////////////////////////
CTreeCtrlXP::~CTreeCtrlXP ()
{
}

////////////////////////////////////////////////////////////////////////////////
void CTreeCtrlXP::PreSubclassWindow ()
{
    ModifyStyleEx (WS_EX_CLIENTEDGE, 0);
    ModifyStyle (0, WS_BORDER, SWP_FRAMECHANGED);
    m_MouseMgr.Init (m_hWnd, MMS_NCPAINT);
}

////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTreeCtrlXP, CTreeCtrl)
    //{{AFX_MSG_MAP(CTreeCtrlXP)
    ON_WM_MOUSEMOVE()
    ON_WM_NCPAINT()
    //}}AFX_MSG_MAP
    ON_WM_MOUSEOUT()
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
void CTreeCtrlXP::OnMouseMove (UINT, CPoint)
{
    Default();
    m_MouseMgr.OnMouseMove();
}

///////////////////////////////////////////////////////////////////////////////
void CTreeCtrlXP::OnMouseOut ()
{
    m_MouseMgr.OnMouseOut();
}

///////////////////////////////////////////////////////////////////////////////
void CTreeCtrlXP::OnNcPaint ()
{
    Default();

    CWindowDC cDC (this);
    CWindowRect rc (this);
    CPenDC pen (cDC, ::GetSysColor (m_MouseMgr.MouseOver() ? COLOR_HIGHLIGHT : COLOR_3DSHADOW));

    cDC.SelectObject (::GetStockObject (NULL_BRUSH));
    cDC.Rectangle (0, 0, rc.Width(), rc.Height());
}