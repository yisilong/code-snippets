// H нд╪Ч
#pragma once

class CScrollWnd  
{
public:
	CScrollWnd();
	virtual ~CScrollWnd();

public:
	void Attach(HWND hWnd);
	void Detach();
	void SetSize(int nWidth, int nHeight);
	const SIZE& GetSize() const;
	const SIZE& GetScrollPos() const;
	const SIZE& GetPageSize() const;

	void ScrollToOrigin(BOOL bLeft, BOOL bTop);

	void OnHScroll(WPARAM wParam, LPARAM lParam);
	void OnVScroll(WPARAM wParam, LPARAM lParam);
	BOOL OnMouseWheel(WPARAM wParam, LPARAM lParam);
	void OnSize(WPARAM wParam, LPARAM lParam);

private:
	void SetScrollClientRect(HWND hWnd, RECT& rect);
	int Get32BitScrollPos(int bar, BOOL bTrackPos, HWND hWndScrollBar);
	void UpdateScrollInfo();
	void UpdateScrollBar(int bar, int windowSize, int displaySize, LONG& pageSize, LONG& scrollPos, LONG& deltaPos);

	HWND m_hWndAttach;
	SIZE m_pageSize;
	SIZE m_size;
	SIZE m_scrollPos;
};