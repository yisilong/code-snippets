// EditableListBox.cpp : implementation file
//

#include "stdafx.h"
#include "EditableListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditableListBox

CEditableListBox::CEditableListBox()
{
	m_iItemBeingEdited = -1;
}

CEditableListBox::~CEditableListBox()
{
	// Is there an edit window lurking about? If so, get rid of it
	if( m_ceEdit.GetSafeHwnd() != NULL )
		m_ceEdit.DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
//	PROTECTED
/////////////////////////////////////////////////////////////////////////////
void CEditableListBox::EditStarts()
{
	int iSel = GetCurSel();

	// Anything selected?
	if( iSel != LB_ERR )
	{
		CRect rItem;

		// Get the rectangle and position this item occupies in the listbox
		GetItemRect( iSel, rItem );

		// Make the rectangle a bit larger top-to-bottom. Not necessary but looks better to me
		rItem.InflateRect( 0, 2, 0, 2 );

		// Create the edit control
		m_ceEdit.Create( WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL, rItem, this, 1 );
		m_ceEdit.ModifyStyleEx( 0, WS_EX_CLIENTEDGE );	// brings up the XP-style
		
		// Give it the same font as the listbox
		m_ceEdit.SetFont( this->GetFont() );
		
		// Now add the item's text to it and selected for convenience
		CString csItem;

		GetText( iSel, csItem );

		m_ceEdit.SetWindowText( csItem );
		m_ceEdit.SetSel( 0, -1, TRUE );
		
		// Set the focus on it
		m_ceEdit.SetFocus();

		// Record the item position
		m_iItemBeingEdited = iSel;
	}
}

void CEditableListBox::EditEnds( BOOL bCommitText /* = TRUE */ )
{
	// Is there an edit window lurking about? If so, we are in business
	if( m_ceEdit.GetSafeHwnd() != NULL )
	{
		// Do we want the text entered by the user to replace the selected lb item's text?
		if( bCommitText && m_iItemBeingEdited != -1 )
		{
			CString csOldItem, csNewItem;

			GetText( m_iItemBeingEdited, csOldItem );
			m_ceEdit.GetWindowText( csNewItem );

			// If the new text is the same as the old, why bother
			if( csOldItem.Compare( csNewItem ) != 0 )
			{
				// Get rid of the lb item that we are replacing
				DeleteString( m_iItemBeingEdited );

				// If the listbox has the "sort" attribute on, we add the new text and let it sort it out
				// Otherwise, we insert the new text precisely where the old one was
				if( GetStyle() & LBS_SORT )
					m_iItemBeingEdited = AddString( csNewItem );
				else
					m_iItemBeingEdited = InsertString( m_iItemBeingEdited, csNewItem );

				// Select the lb item
				SetCurSel( m_iItemBeingEdited );

				// Let the parent know if LBS_NOTIFY is flagged
				if( GetStyle() & LBS_NOTIFY )
					GetParent()->SendMessage( WM_APP_LB_ITEM_EDITED, (WPARAM)m_iItemBeingEdited, (LPARAM)(LPCTSTR)csOldItem );
			}
		}

		// The editing is done, nothing is marked for editing
		m_iItemBeingEdited = -1;

		// Get rid of the editing window
		m_ceEdit.DestroyWindow();
	}
}


BEGIN_MESSAGE_MAP(CEditableListBox, CListBox)
	//{{AFX_MSG_MAP(CEditableListBox)
	ON_WM_RBUTTONUP()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_APP_ED_EDIT_FINISHED, OnEditFinished)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditableListBox message handlers

void CEditableListBox::OnRButtonUp(UINT nFlags, CPoint point) 
{
	int iItemCurrentlySelected = GetCurSel();
	
	// Anything selected?
	if( iItemCurrentlySelected != LB_ERR )
	{
		CRect rItemCurrentlySelected;

		GetItemRect( iItemCurrentlySelected, rItemCurrentlySelected );

		// If the user clicked on a previously selected item, start the editing
		if( rItemCurrentlySelected.PtInRect( point ) )
			EditStarts();
	}
	
	CListBox::OnRButtonUp(nFlags, point);
}

void CEditableListBox::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( nChar == VK_INSERT )
		EditStarts();
	
	CListBox::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CEditableListBox::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	EditEnds();
	
	CListBox::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CEditableListBox::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	EditEnds();
	
	CListBox::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CEditableListBox::OnSize(UINT nType, int cx, int cy) 
{
	CListBox::OnSize(nType, cx, cy);
	
	EditEnds();
}

LRESULT CEditableListBox::OnEditFinished(WPARAM wParam, LPARAM /*lParam*/)
{
	EditEnds( (BOOL)wParam );
	
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CEditHelper
CEditHelper::CEditHelper()
{
}

CEditHelper::~CEditHelper()
{
}


BEGIN_MESSAGE_MAP(CEditHelper, CEdit)
//{{AFX_MSG_MAP(CEditHelper)
ON_WM_KEYUP()
ON_WM_KILLFOCUS()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditHelper message handlers

void CEditHelper::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch( nChar )
	{
	case VK_RETURN:
		GetParent()->SendMessage( WM_APP_ED_EDIT_FINISHED, (WPARAM)TRUE );	// Commit changes
		break;
		
	case VK_ESCAPE:
		GetParent()->SendMessage( WM_APP_ED_EDIT_FINISHED, (WPARAM)FALSE );	// Disregard changes
		break;
	}
	
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CEditHelper::OnKillFocus(CWnd* pNewWnd) 
{
	GetParent()->SendMessage( WM_APP_ED_EDIT_FINISHED, (WPARAM)TRUE );	// Commit changes
	
	CEdit::OnKillFocus(pNewWnd);
}
//////////////////////////////////////////////////////////////////////////////////////////////
