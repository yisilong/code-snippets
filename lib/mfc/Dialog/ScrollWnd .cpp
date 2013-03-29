// CPP нд╪Ч
#include "StdAfx.h"
#include "ScrollWnd.h"

CScrollWnd::CScrollWnd() : m_hWndAttach(NULL)
{
	m_pageSize.cx = 0;
	m_pageSize.cy = 0;

	m_size.cx = 0;
	m_size.cy = 0;

	m_scrollPos.cx = 0;
	m_scrollPos.cy = 0;
}

CScrollWnd::~CScrollWnd()
{
	Detach();
}

void CScrollWnd::Attach(HWND hWnd)
{
	m_hWndAttach = hWnd;
}

void CScrollWnd::Detach()
{
	m_hWndAttach = NULL;
}

void CScrollWnd::SetSize(int nWidth, int nHeight)
{
	m_size.cx = nWidth;
	m_size.cy = nHeight;

	if(NULL != m_hWndAttach && ::IsWindow(m_hWndAttach))
	{
		UpdateScrollInfo();
	}
}

const SIZE& CScrollWnd::GetSize() const
{
	return m_size;
}

const SIZE& CScrollWnd::GetScrollPos() const
{
	return m_scrollPos;
}

const SIZE& CScrollWnd::GetPageSize() const
{
	return m_pageSize;
}

void CScrollWnd::ScrollToOrigin(BOOL bLeft, BOOL bTop)
{
	if(NULL == m_hWndAttach)
	{
		return ;
	}

	if(bLeft)
	{
		if(m_size.cx > 0 && m_pageSize.cx > 0 && m_scrollPos.cx > 0)
		{
			int deltaPos = -m_scrollPos.cx;
			m_scrollPos.cx += deltaPos;
			::SetScrollPos(m_hWndAttach, SB_HORZ, m_scrollPos.cx, TRUE);
			::ScrollWindowEx(m_hWndAttach, -deltaPos, 0, NULL, NULL, NULL, NULL, SW_ERASE | SW_INVALIDATE | SW_SCROLLCHILDREN);
			::UpdateWindow(m_hWndAttach);
		}
	}

	if(bTop)
	{
		if(m_size.cy > 0 && m_pageSize.cy > 0 && m_scrollPos.cy > 0)
		{
			int deltaPos = -m_scrollPos.cy;
			m_scrollPos.cy += deltaPos;
			::SetScrollPos(m_hWndAttach, SB_VERT, m_scrollPos.cy, TRUE);
			::ScrollWindowEx(m_hWndAttach, 0, -deltaPos, NULL, NULL, NULL, NULL, SW_ERASE | SW_INVALIDATE | SW_SCROLLCHILDREN);
		}
	}
}

void CScrollWnd::OnHScroll(WPARAM wParam, LPARAM lParam)
{
	if(NULL == m_hWndAttach)
	{
		return ;
	}
	const int lineOffset = 60;

	int deltaPos = 0;
	UINT nSBCode = LOWORD(wParam);
	UINT nPos = HIWORD(wParam);

	switch( nSBCode )
	{
	case SB_LINELEFT:
		deltaPos = -lineOffset;
		break;

	case SB_LINERIGHT:
		deltaPos = lineOffset;
		break;

	case SB_PAGELEFT:
		deltaPos = -m_pageSize.cx;
		break;

	case SB_PAGERIGHT:
		deltaPos = m_pageSize.cx;
		break;

	case SB_THUMBTRACK:
		deltaPos = Get32BitScrollPos(SB_HORZ, TRUE, (HWND)lParam) - m_scrollPos.cx;
		break;

	case SB_THUMBPOSITION:
		deltaPos = Get32BitScrollPos(SB_HORZ, FALSE, (HWND)lParam) - m_scrollPos.cx;
		break;

	default:
		return;
	}

	int newScrollPos = m_scrollPos.cx + deltaPos;

	if(newScrollPos < 0)
	{
		deltaPos = -m_scrollPos.cx;
	}

	int maxScrollPos = m_size.cx - m_pageSize.cx;
	if(newScrollPos > maxScrollPos)
	{
		deltaPos = maxScrollPos - m_scrollPos.cx;
	}

	if(deltaPos != 0)
	{
		m_scrollPos.cx += deltaPos;
		::SetScrollPos(m_hWndAttach, SB_HORZ, m_scrollPos.cx, TRUE);
		::ScrollWindowEx(m_hWndAttach, -deltaPos, 0, NULL, NULL, NULL, NULL, SW_ERASE | SW_INVALIDATE | SW_SCROLLCHILDREN);
		::UpdateWindow(m_hWndAttach);
	}
}

void CScrollWnd::OnVScroll(WPARAM wParam, LPARAM lParam)
{
	if(NULL == m_hWndAttach)
	{
		return ;
	}
	const int lineOffset = 60;
	UINT nSBCode = LOWORD(wParam);
	UINT nPos = HIWORD(wParam);    
	int deltaPos = 0;

	SCROLLINFO si = {sizeof(si), SIF_ALL};
	GetScrollInfo(m_hWndAttach, SB_VERT, &si);

	switch( nSBCode )
	{
	case SB_LINEUP:
		deltaPos = -lineOffset;
		break;

	case SB_LINEDOWN:
		deltaPos = lineOffset;
		break;

	case SB_PAGEUP:
		deltaPos = -m_pageSize.cy;
		break;

	case SB_PAGEDOWN:
		deltaPos = m_pageSize.cy;
		break;

	case SB_THUMBTRACK:
		deltaPos = Get32BitScrollPos(SB_VERT, TRUE, (HWND)lParam) - m_scrollPos.cy;
		break;

	case SB_THUMBPOSITION:
		deltaPos = Get32BitScrollPos(SB_VERT, FALSE, (HWND)lParam) - m_scrollPos.cy;
		break;

	default:
		return;
	}

	int newScrollPos = m_scrollPos.cy + deltaPos;	
	if(newScrollPos < 0)
	{
		deltaPos = -m_scrollPos.cy;
	}

	int maxScrollPos = m_size.cy - m_pageSize.cy;
	if(newScrollPos > maxScrollPos)
	{
		deltaPos = maxScrollPos - m_scrollPos.cy;
	}

	if(deltaPos != 0)
	{
		m_scrollPos.cy += deltaPos;
		::SetScrollPos(m_hWndAttach, SB_VERT, m_scrollPos.cy, TRUE);
		::ScrollWindowEx(m_hWndAttach, 0, -deltaPos, NULL, NULL, NULL, NULL, SW_ERASE | SW_INVALIDATE | SW_SCROLLCHILDREN);
		::UpdateWindow(m_hWndAttach);
	}
}

BOOL CScrollWnd::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
	if(NULL == m_hWndAttach)
	{
		return FALSE;
	}

	int scrollMin = 0;
	int scrollMax = 0;

	SCROLLINFO si = {sizeof(si), SIF_ALL};
	GetScrollInfo(m_hWndAttach, SB_VERT, &si);
	scrollMin = si.nMin;
	scrollMax = si.nMax;

	if(scrollMin == scrollMax)
	{
		return FALSE;
	}

	UINT nFlags = LOWORD(wParam);
	short zDelta = HIWORD(wParam);
	POINT pt = {LOWORD(lParam), HIWORD(lParam)};

	int numScrollIncrements = abs(zDelta) / WHEEL_DELTA;

	int numScrollLinesPerIncrement = 0;
	::SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &numScrollLinesPerIncrement, 0);

	if(numScrollLinesPerIncrement == WHEEL_PAGESCROLL)
	{
		OnVScroll(MAKEWPARAM(zDelta > 0 ? SB_PAGEUP : SB_PAGEDOWN, 0), NULL);
		return TRUE;
	}

	int numScrollLines = numScrollIncrements * numScrollLinesPerIncrement;

	numScrollLines = max(numScrollLines/3, 1);

	for(int i = 0; i < numScrollLines; ++i)
	{
		OnVScroll(MAKEWPARAM(zDelta > 0 ? SB_LINEUP : SB_LINEDOWN, 0), NULL);
	}

	return TRUE;
}

void CScrollWnd::OnSize(WPARAM wParam, LPARAM lParam)
{
	UpdateScrollInfo();
}

void CScrollWnd::SetScrollClientRect(HWND hWnd, RECT& rect)
{
	ASSERT(NULL != hWnd);

	RECT rc;
	::GetWindowRect(hWnd, &rc);

	POINT ptLeftTop = {rc.left, rc.top};
	POINT ptRightBottom = {rc.right, rc.bottom};
	::ScreenToClient(hWnd, &ptLeftTop);
	::ScreenToClient(hWnd, &ptRightBottom);

	rc.left = ptLeftTop.x;
	rc.top = ptLeftTop.y;
	rc.right = ptRightBottom.x;
	rc.bottom = ptRightBottom.y;

	::GetClientRect(hWnd, &rect);

	int cx = ::GetSystemMetrics(SM_CXVSCROLL);
	int cy = ::GetSystemMetrics(SM_CYHSCROLL);

	if(rc.right >= (rect.right + cx))
	{
		rect.right += cx;
	}
	if(rc.bottom >= (rect.bottom + cy))
	{
		rect.bottom += cy;
	}
}

int CScrollWnd::Get32BitScrollPos(int bar, BOOL bTrackPos, HWND hWndScrollBar)
{
	if(NULL == m_hWndAttach)
	{
		return 0;
	}

	HWND hWndScroll;
	if (NULL == hWndScrollBar)
	{
		hWndScroll = m_hWndAttach;
	}
	else
	{
		hWndScroll = hWndScrollBar;
	}

	SCROLLINFO si = {0};
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = bTrackPos ? SIF_TRACKPOS : SIF_POS;
	::GetScrollInfo(hWndScroll, bar, &si);

	return bTrackPos ? si.nTrackPos : si.nPos;
}

void CScrollWnd::UpdateScrollInfo()
{
	if(NULL == m_hWndAttach)
	{
		return ;
	}

	RECT rect;
	SetScrollClientRect(m_hWndAttach, rect);

	SIZE windowSize = {rect.right - rect.left, rect.bottom - rect.top};
	SIZE deltaPos = {0, 0};

	UpdateScrollBar(SB_HORZ, windowSize.cx, m_size.cx, m_pageSize.cx, m_scrollPos.cx, deltaPos.cx);

	UpdateScrollBar(SB_VERT, windowSize.cy, m_size.cy, m_pageSize.cy, m_scrollPos.cy, deltaPos.cy);

	if(deltaPos.cx != 0 || deltaPos.cy != 0)
	{
		::ScrollWindowEx(m_hWndAttach, deltaPos.cx, deltaPos.cy, NULL, NULL, NULL, NULL, SW_ERASE | SW_INVALIDATE | SW_SCROLLCHILDREN);
		::UpdateWindow(m_hWndAttach);
	}
}

void CScrollWnd::UpdateScrollBar(int bar, int windowSize, int displaySize, LONG& pageSize, LONG& scrollPos, LONG& deltaPos)
{
	int scrollMax = 0;
	deltaPos = 0;

	SCROLLINFO si = {sizeof(si), SIF_ALL};
	::GetScrollInfo(m_hWndAttach, bar, &si);
	int nPos = si.nPos;

	if(windowSize < displaySize )
	{
		scrollMax = displaySize - 1;
		if(pageSize > 0 && scrollPos > 0)
		{
			scrollPos = (LONG)(1.0 * scrollPos * windowSize / pageSize);
		}
		pageSize = windowSize;
		scrollPos = min(scrollPos, displaySize - pageSize - 1);
		deltaPos = nPos - scrollPos;
	}
	else
	{
		pageSize = 0;
		scrollPos = 0;
		deltaPos = nPos;
	}

	memset(&si, 0, sizeof(SCROLLINFO));
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	si.nMin = 0;
	si.nMax = scrollMax;
	si.nPage = pageSize;
	si.nPos = scrollPos;
	::SetScrollInfo(m_hWndAttach, bar, &si, TRUE);
}
