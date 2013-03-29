/////////////////////////////////////////////////////////////////////////////
// CEditHelper.cpp : implementation file
//////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MyListBox.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef unsigned long  DWORD_PTR, *PDWORD_PTR;
/////////////////////////////////////////////////////////////////////////////////////////////
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


/////////////////////////////////////////////////////////////////////////////
// CMyListBox.cpp : implementation file
#define TID_SCROLLDOWN  100
#define TID_SCROLLUP    101
#define	WM_APP_LB_ITEM_EDITED ( WM_APP + 04100 )
IMPLEMENT_DYNAMIC(CMyListBox, CListBox)

CMyListBox::CMyListBox()
{
	m_clrDlgBackGround=RGB(254,250,247);//::GetSysColor(COLOR_3DFACE);
	m_clrItemBackGround=RGB(254,250,247);
	m_clrItemSelBackGround = RGB(252,236,172);//::GetSysColor(COLOR_HIGHLIGHT);
	m_clrText = RGB(128,128,128);
	m_clrSelText = RGB(0,0,0);//::GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_clrMouseMove=RGB(193,225,248);

	m_bEnableDragSort=FALSE;
	m_bEnableDropFiles=FALSE;

	m_clrPreItem=-1;
	m_nPreIndex=-1;

	m_nMovingIndex=-1;
	m_nMoveToIndex=-1;
	m_bCaptured=FALSE;
    m_dwInterval=0;

	m_nItemBeingEdited = -1;
	m_bIsEditing = FALSE;
	m_bIsMsgBoxSend = FALSE;

	m_dlgBrush.CreateSolidBrush(m_clrDlgBackGround);
}

CMyListBox::~CMyListBox()
{
	if( m_Edit.GetSafeHwnd() != NULL )
		m_Edit.DestroyWindow();
	m_dlgBrush.DeleteObject();
}
BEGIN_MESSAGE_MAP(CMyListBox, CListBox)
    //{{AFX_MSG_MAP(CMyListBox)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
    ON_WM_TIMER()
	ON_WM_DROPFILES()
	ON_WM_KEYUP()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR_REFLECT()
    ON_WM_MEASUREITEM_REFLECT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_MESSAGE(WM_APP_ED_EDIT_FINISHED, OnEditFinished)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyListBox message handlers
void CMyListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if ((int)lpDrawItemStruct->itemID < 0)
		return;
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);	
	ItemData *pItemData=(ItemData*)GetItemDataPtr(lpDrawItemStruct->itemID);
//	COLORREF clrItemData = (COLORREF)lpDrawItemStruct->itemData; // RGB in item data
	CString sLabel;
	GetText(lpDrawItemStruct->itemID, sLabel);

	CRect rcItem=lpDrawItemStruct->rcItem;
	// item selected
	if ((lpDrawItemStruct->itemState & ODS_SELECTED) &&   
		(lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{	
		// draw label background
	
		pDC->FillSolidRect(&rcItem,m_clrItemSelBackGround);
		pDC->DrawIcon(rcItem.left,rcItem.top,pItemData->hIcon);	
		rcItem.left+=32;
		pDC->SetBkMode(TRANSPARENT);
		COLORREF colorTextSave = pDC->SetTextColor(m_clrSelText);
		pDC->DrawText( sLabel, -1, &rcItem, DT_WORDBREAK  | DT_SINGLELINE | DT_VCENTER );		
		pDC->SetTextColor(colorTextSave);
		return;
	}
	
	// item brought into box
	if (lpDrawItemStruct->itemAction & ODA_DRAWENTIRE) 
	{
		pDC->FillSolidRect(&rcItem,pItemData->clr);
		pDC->DrawIcon(rcItem.left,rcItem.top,pItemData->hIcon);
		rcItem.left+=32;
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText( sLabel, -1, &rcItem, DT_WORDBREAK | DT_SINGLELINE | DT_VCENTER );
		return;
	}
	
	// item deselected
	if (!(lpDrawItemStruct->itemState & ODS_SELECTED) && 
		(lpDrawItemStruct->itemAction & ODA_SELECT))
	{
		pDC->FillSolidRect(&rcItem,pItemData->clr);
		pDC->DrawIcon(rcItem.left,rcItem.top,pItemData->hIcon);
		rcItem.left+=32;
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText( sLabel, -1, &rcItem, DT_WORDBREAK | DT_SINGLELINE | DT_VCENTER );
		return;
	}
}

void CMyListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// all items are of fixed size
	// must use LBS_OWNERDRAWVARIABLE for this to work
    ASSERT(lpMeasureItemStruct->CtlType == ODT_LISTBOX);	
	int nItem = lpMeasureItemStruct->itemID;
	CPaintDC dc(this);
	CString sLabel;
	CListBox::GetText(nItem, sLabel);
	CRect	csRect(0, 0, lpMeasureItemStruct->itemWidth, lpMeasureItemStruct->itemHeight);
	int itemHeight = dc.DrawText( sLabel, -1, csRect, DT_CALCRECT | DT_WORDBREAK);
    lpMeasureItemStruct->itemHeight =32; //itemHeight+6*GetSystemMetrics(SM_CXBORDER);
}

int CMyListBox::AddString(LPCTSTR lpszItem, COLORREF clr /* = -1 */,HICON hIcon/* =NULL */)
{
    int nItem=((CListBox*)this)->AddString(lpszItem);
	if (nItem >= 0)
	{
		ItemData *pItemData=new ItemData;
		pItemData->clr=(clr==-1)? m_clrItemBackGround:clr;
		pItemData->hIcon=hIcon;
		SetItemDataPtr(nItem,(void*)pItemData);
	}
	return nItem;
}
int CMyListBox::InsertString(int nIndex, LPCTSTR lpszItem, COLORREF clr /* = -1 */,HICON hIcon/* =NULL */)
{
	int nItem = ((CListBox*)this)->InsertString(nIndex,lpszItem);
	if (nItem >= 0)
	{
		ItemData *pItemData=new ItemData;
		pItemData->clr=(clr==-1)? m_clrItemBackGround:clr;
		pItemData->hIcon=hIcon;
		SetItemDataPtr(nItem,(void*)pItemData);
	}
	return nItem;
}

void CMyListBox::SetItemColor(int nIndex, COLORREF clr)
{
	if (nIndex<=GetCount())
	{
		ItemData *pItemData=(ItemData*)GetItemDataPtr(nIndex-1);
		pItemData->clr=clr;	
		RedrawWindow();
	}
}

void CMyListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
   CListBox::OnLButtonDown(nFlags, point);

   if (m_bEnableDragSort)
   { 
	   //clear all the flags
	   m_nMovingIndex = LB_ERR;
	   m_nMoveToIndex = LB_ERR;
	   m_bCaptured = FALSE;
	   m_dwInterval = 0;
	   BOOL Outside;
	   //find the item that they want to drag
	   //and keep track of it. Later in the mouse move
	   //we will capture the mouse if this value is set
	   int Index = ItemFromPoint(point,Outside);
	   if (Index != LB_ERR && !Outside)
	   {
		   m_nMovingIndex = Index;
		   SetCurSel(Index);
	   }
   }
}

void CMyListBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bEnableDragSort)
	{ 
		if (m_nMovingIndex != LB_ERR && m_bCaptured)
		{
			KillTimer(TID_SCROLLDOWN);
			KillTimer(TID_SCROLLUP);
			m_dwInterval = 0;
			m_bCaptured = FALSE;
			ReleaseCapture();
			CRect Rect;
			GetClientRect(&Rect);      
			if (Rect.PtInRect(point))//if they are still within the listbox window
			{
				InsertDraggedItem();
			}
			else
			{
				RedrawWindow();
			}
			m_nMovingIndex = LB_ERR;
			m_nMoveToIndex = LB_ERR;
			RedrawWindow();
		}
	}  
   CListBox::OnLButtonUp(nFlags, point);
}


void CMyListBox::OnMouseMove(UINT nFlags, CPoint point)
{
	TRACKMOUSEEVENT   tme;
	tme.cbSize=sizeof(TRACKMOUSEEVENT);
	tme.dwFlags=TME_LEAVE;         
	tme.dwHoverTime=HOVER_DEFAULT;       
	tme.hwndTrack=m_hWnd;     
     _TrackMouseEvent(&tme);  
	BOOL bOutside;
	int nIdexNow=ItemFromPoint(point,bOutside);
	if (nIdexNow!=LB_ERR)
	{
		if (m_nPreIndex!=nIdexNow)
		{
			SetRedraw(FALSE);
			CRect rect;
			if (m_nPreIndex!=LB_ERR)
			{
				GetItemRect(m_nPreIndex,rect);
				ItemData *pItemData=(ItemData*)GetItemDataPtr(m_nPreIndex);
				pItemData->clr=m_clrPreItem;
				InvalidateRect(rect);
			}
			m_nPreIndex=nIdexNow;
			ItemData *pItemData=(ItemData*)GetItemDataPtr(m_nPreIndex);
			m_clrPreItem=pItemData->clr;
		    GetItemRect(nIdexNow,rect);			
			pItemData->clr=m_clrMouseMove;
			InvalidateRect(rect);
			SetRedraw(TRUE);
		}
	}

	
	if (m_bEnableDragSort)
	{  
		if (nFlags & MK_LBUTTON)
		{
			if (m_nMovingIndex != LB_ERR && !m_bCaptured)
			{
				SetCapture();
				m_bCaptured = TRUE;
			}
			BOOL bOutside;
			int Index = ItemFromPoint(point,bOutside);
			//if they are not on a particular item
			if (bOutside)
			{
				CRect ClientRect;
				GetClientRect(&ClientRect);
				DoTheScrolling(point,ClientRect);
			}
			else
			{
				KillTimer(TID_SCROLLDOWN);
				KillTimer(TID_SCROLLUP);
				m_dwInterval = 0;
			}
			
			if (Index != m_nMoveToIndex)
			{
				DrawTheLines(Index);
			}
		}
	} 
	CListBox::OnMouseMove(nFlags, point);
}

void CMyListBox::DrawTheLines(int Index)
{
   CRect ClientRect;
   GetClientRect(&ClientRect);
   CDC *pDC = GetDC();
   CRect Rect;
   int Width = 2;
   if (m_nMoveToIndex != m_nMovingIndex)
   {
	  ItemData *pItemData=(ItemData*)GetItemDataPtr(m_nMoveToIndex);
	  COLORREF clr=pItemData->clr;
      CPen Pen(PS_SOLID,Width,clr);
      CPen *pOldPen = pDC->SelectObject(&Pen);
      if (m_nMoveToIndex != LB_ERR)
      {
		  GetItemRect(m_nMoveToIndex,&Rect);
		  if ((m_nMoveToIndex == GetCount()-1) || (m_nMoveToIndex == m_nMovingIndex+1))
		  {
			  Rect.DeflateRect(1,1,1,1);
			  if (ClientRect.PtInRect(Rect.BottomRight()))
			  {
				  pDC->MoveTo(Rect.left,Rect.bottom);
				  pDC->LineTo(Rect.right,Rect.bottom);
			  }			 
		  }  
		  else if (ClientRect.PtInRect(Rect.TopLeft()))
		  {
			  pDC->MoveTo(Rect.left,Rect.top+1);
			  pDC->LineTo(Rect.right-(Width/2),Rect.top+1);
		  }
      }
	  pDC->SelectObject(pOldPen);
   }
   m_nMoveToIndex = Index;
   if (m_nMoveToIndex != m_nMovingIndex)
   {
	   CPen Pen(PS_SOLID,Width,RGB(0,0,0));
	   CPen *pOldPen = pDC->SelectObject(&Pen);
	   if (m_nMoveToIndex != LB_ERR)
	   {
		   GetItemRect(m_nMoveToIndex,&Rect);
		   if ((m_nMoveToIndex == GetCount()-1) || (m_nMoveToIndex == m_nMovingIndex+1))
		   {
			   Rect.DeflateRect(1,1,1,1);
			   if (ClientRect.PtInRect(Rect.BottomRight()))
			   {
				   pDC->MoveTo(Rect.left,Rect.bottom);
				   pDC->LineTo(Rect.right,Rect.bottom);
			   }	 
		   }  
		   else if (ClientRect.PtInRect(Rect.TopLeft()))
		   {
			   pDC->MoveTo(Rect.left,Rect.top+1);
			   pDC->LineTo(Rect.right-(Width/2),Rect.top+1);
		   }
	   }
      pDC->SelectObject(pOldPen);
   }
   ReleaseDC(pDC);
}

void CMyListBox::InsertDraggedItem()
{
   CString Text;
   GetText(m_nMovingIndex,Text);
   ItemData *pItemData=(ItemData*)GetItemDataPtr(m_nMovingIndex);
   if (m_nMovingIndex < m_nMoveToIndex)
   {
      SetRedraw(FALSE);
      int TopIndex = GetTopIndex();
      DeleteString(m_nMovingIndex);  
	  InsertString(m_nMoveToIndex,Text,pItemData->clr,pItemData->hIcon);
	  delete pItemData;
	  SetTopIndex(TopIndex);
      //we have to redraw the window since we are deleting a string
      //after we set the current selection. DeleteString causes a 
      //focus rect to show up under the current selection      
      SetRedraw(TRUE);     
      RedrawWindow();
   }
   else if (m_nMovingIndex > m_nMoveToIndex)
   {
      SetRedraw(FALSE);
      int TopIndex = GetTopIndex();
      DeleteString(m_nMovingIndex);
      int NewIndex = InsertString(m_nMoveToIndex,Text);
      InsertString(m_nMoveToIndex,Text,pItemData->clr,pItemData->hIcon);
	  delete pItemData;
      SetTopIndex(TopIndex);
	  //we have to redraw the window since we are deleting a string
      //after we set the current selection. DeleteString causes a 
      //focus rect to show up under the current selection
      SetRedraw(TRUE);      
      RedrawWindow();
   }
   GetParent()->SendMessage(WM_COMMAND,MAKEWPARAM(GetDlgCtrlID(),LBN_SELCHANGE),(LPARAM)m_hWnd);
}

void CMyListBox::DoTheScrolling(CPoint Point,CRect ClientRect)
{
   if (Point.y > ClientRect.Height())
   {
      DWORD Interval = 250 / (1+ ((Point.y-ClientRect.Height())/GetItemHeight(0)));
      if (Interval != m_dwInterval)
      {
         m_dwInterval = Interval;
         SetTimer(TID_SCROLLDOWN,Interval,NULL);
         OnTimer(TID_SCROLLDOWN);
      }
   }
   else if (Point.y < 0)
   {
      DWORD Interval = 250 / (1+(abs(Point.y)/GetItemHeight(1)));
      if (Interval != m_dwInterval)
      {
         m_dwInterval = Interval;
         SetTimer(TID_SCROLLUP,Interval,NULL);
         OnTimer(TID_SCROLLUP);
      }
   }
   else
   {
      KillTimer(TID_SCROLLDOWN);
      KillTimer(TID_SCROLLUP);
      m_dwInterval = 0;
   }
}
void CMyListBox::OnTimer(UINT nIDEvent)
{
   if (nIDEvent == TID_SCROLLDOWN)
   {
      DrawTheLines(m_nMoveToIndex+1);
	  SetTopIndex(GetTopIndex()+1);
   }
   else if (nIDEvent == TID_SCROLLUP)
   { 
      DrawTheLines(m_nMoveToIndex-1);
	  SetTopIndex(GetTopIndex()-1);
   }
   CListBox::OnTimer(nIDEvent);
}

void CMyListBox::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	char *lpszfilename=new char[512];
	int nfilecount;
	int i;
	nfilecount=::DragQueryFile(hDropInfo,0xffffffff,NULL,512);   
	for (i=0; i < nfilecount; i++)
	{ 
		UINT nchars=::DragQueryFile(hDropInfo,i,&lpszfilename[0],512); 
		CString str(&lpszfilename[0],nchars); 
		AddString(str);		
	} 
	::DragFinish(hDropInfo); //释放内存 
    delete []lpszfilename;
	CListBox::OnDropFiles(hDropInfo);
}

void CMyListBox::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CListBox::PreSubclassWindow();
}

void CMyListBox::EnableDropFiles(BOOL bEnable)
{
	if (bEnable)
	{
	   ModifyStyleEx(0,WS_EX_ACCEPTFILES,0);
	}
}

LRESULT CMyListBox::OnMouseLeave(WPARAM wParam  ,LPARAM  lParam )
{
	if (m_nPreIndex!=LB_ERR)
	{
		ItemData *pItemData=(ItemData*)GetItemDataPtr(m_nPreIndex);
		pItemData->clr=m_clrPreItem;
		CRect rc;
		GetItemRect(m_nPreIndex,rc);
		InvalidateRect(rc);	
		m_nPreIndex=LB_ERR;
	}
	return 0;
}

LRESULT CMyListBox::OnEditFinished(WPARAM wParam, LPARAM lParam)
{
	EndEdit( (BOOL)wParam );	
	return 0;
}
void CMyListBox::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nChar)
	{
	case VK_F2:
		StartEdit();
		break;
	case VK_DELETE:
		{
			if (!m_bIsEditing)
			{
				int nIndex=GetCurSel();
				if (nIndex!=LB_ERR)
				{
					m_bIsMsgBoxSend = TRUE;
					if (IDOK==MessageBox("确定要删除该项纪录?","警告",MB_OKCANCEL | MB_ICONWARNING))
					{
						DeleteString(nIndex);
						int nSizeItem=GetCount();
						if (nSizeItem!=LB_ERR)
						{
							if (nIndex==nSizeItem)
							{
								SetCurSel(nIndex-1);
							}
							else
							{
								SetCurSel(nIndex);
							}
						}
					}
				}
			}
		}break;			
	case VK_RETURN:
		{
			if (!m_bIsEditing && !m_bIsMsgBoxSend)
			{
				StartEdit();
			}
			m_bIsMsgBoxSend =FALSE;
		}break;
	case VK_INSERT:
		{
			if (!m_bIsEditing)
			{
				int nItem=GetCurSel();
				InsertString(nItem," 请输入");
				StartEdit();
			}
		}break;
	}
	CListBox::OnKeyUp(nChar, nRepCnt, nFlags);
}

BOOL CMyListBox::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{	
		if (pMsg->wParam == VK_TAB)
		{
			if (m_bIsEditing)
			{
				if ((GetStyle() & LBS_EXTENDEDSEL) || (GetStyle() & LBS_MULTIPLESEL)) //多选
				{
// 					EndEdit(TRUE);
// 					int nTotalItem=GetCount();
// 					int nNowItem=nTotalItem;
// 					while (nNowItem!=0)
// 					{
// 						nNowItem--;
// 						int nResult=GetSel(nNowItem);
// 						if (nResult!=0 && nResult!=LB_ERR)
// 						{
// 							SetSel(nNowItem,FALSE);
// 							if (nNowItem!=nTotalItem-1)
// 								SetSel(nNowItem+1,TRUE);
// 							else SetSel(0,TRUE);
// 							StartEdit();
// 							return TRUE;
// 						}						
// 					}
					EndEdit(TRUE);
					int nNowItem=GetCurSel();
					SetSel(nNowItem,FALSE);
					if (nNowItem!=GetCount()-1)
						SetSel(nNowItem+1);
					else SetSel(0);
					StartEdit();
					return TRUE;
				}
				else
				{
					EndEdit(TRUE);
					int nNowItem=GetCurSel();
					if (nNowItem!=GetCount()-1)
						SetCurSel(nNowItem+1);
					else SetCurSel(0);
					StartEdit();
					return TRUE;
				}				
			}
		}
	}
	return CListBox::PreTranslateMessage(pMsg);
}

void CMyListBox::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	EndEdit();	
	CListBox::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CMyListBox::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	EndEdit();
	CListBox::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMyListBox::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int nItemCurSel = GetCurSel();	
	// Anything selected?
	if( nItemCurSel != LB_ERR )
	{
		CRect rcItemCurSel;		
		GetItemRect( nItemCurSel, rcItemCurSel );		
		// If the user clicked on a previously selected item, start the editing
		if( rcItemCurSel.PtInRect( point ) )
			StartEdit();
	}
	CListBox::OnRButtonDblClk(nFlags, point);
}

void CMyListBox::OnSize(UINT nType, int cx, int cy) 
{
	CListBox::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	EndEdit();
}

void CMyListBox::EndEdit( BOOL bCommitText /* = TRUE */ )
{
	// Is there an edit window lurking about? If so, we are in business
	if( m_Edit.GetSafeHwnd() != NULL )
	{
		// Do we want the text entered by the user to replace the selected lb item's text?
		if( bCommitText && m_nItemBeingEdited != -1 )
		{
			CString szOldItem, szNewItem;
			
			GetText( m_nItemBeingEdited, szOldItem );
			m_Edit.GetWindowText( szNewItem );
			
			// If the new text is the same as the old, why bother
			if( szOldItem.Compare( szNewItem ) != 0 )
			{
				// Get rid of the lb item that we are replacing
				DeleteString( m_nItemBeingEdited );
				
				// If the listbox has the "sort" attribute on, we add the new text and let it sort it out
				// Otherwise, we insert the new text precisely where the old one was
				if( GetStyle() & LBS_SORT )
					m_nItemBeingEdited = AddString( szNewItem );
				else
					m_nItemBeingEdited = InsertString( m_nItemBeingEdited, szNewItem );
				
				// Select the lb item
				SetCurSel( m_nItemBeingEdited );
				
				// Let the parent know if LBS_NOTIFY is flagged
				if( GetStyle() & LBS_NOTIFY )
					GetParent()->SendMessage( WM_APP_LB_ITEM_EDITED, (WPARAM)m_nItemBeingEdited, (LPARAM)(LPCTSTR)szOldItem );
			}
		}
		
		// The editing is done, nothing is marked for editing
		m_nItemBeingEdited = -1;
		m_bIsEditing = FALSE;
		// Get rid of the editing window
		m_Edit.DestroyWindow();
		m_Edit.m_hWnd=NULL;
	}
}

void CMyListBox::StartEdit()
{
	int nSelIndex = GetCurSel();
	// Anything selected?
	if( nSelIndex != LB_ERR )
	{
		CRect rcItem;
		// Get the rectangle and position this item occupies in the listbox
		GetItemRect( nSelIndex, rcItem );
		
		// Make the rectangle a bit larger top-to-bottom. Not necessary but looks better to me
		rcItem.InflateRect( 0, 2, 0, 2 );
		
		// Create the edit control
		m_Edit.Create( WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL, rcItem, this, 1 );
		m_Edit.ModifyStyleEx( 0, WS_EX_CLIENTEDGE );	// brings up the XP-style
		
		// Give it the same font as the listbox
		m_Edit.SetFont( this->GetFont() );
		
		// Now add the item's text to it and selected for convenience
		CString csItem;
		
		GetText( nSelIndex, csItem );
		
		m_Edit.SetWindowText( csItem );
		m_Edit.SetSel( 0, -1, TRUE );
		
		// Set the focus on it
		m_Edit.SetFocus();
		
		// Record the item position
		m_nItemBeingEdited = nSelIndex;
		m_bIsEditing = TRUE;
	}
}

COLORREF CMyListBox::SetDlgBackGround(COLORREF clr/*=-1*/)
{
	if (clr==-1 || clr==m_clrDlgBackGround)
	{
		return m_clrDlgBackGround;
	}	
	COLORREF clrTemp=m_clrDlgBackGround;
	m_clrDlgBackGround=clr;
	m_dlgBrush.DeleteObject();
	m_dlgBrush.CreateSolidBrush(m_clrDlgBackGround);
	RedrawWindow();
	return clrTemp;
}
COLORREF CMyListBox::SetItemBackGroundColor(COLORREF clr/*=-1*/)
{
	if (clr==-1 || clr==m_clrItemBackGround)
	{
		return m_clrItemBackGround;
	}	
	COLORREF clrTemp=m_clrItemBackGround;
	m_clrItemBackGround=clr;
	RedrawWindow();
	return clrTemp;
}
COLORREF CMyListBox::SetItemSelBackGroundColor(COLORREF clr/*=-1*/)
{
	if (clr==-1 || clr==m_clrItemSelBackGround)
	{
		return m_clrItemSelBackGround;
	}	
	COLORREF clrTemp=m_clrItemSelBackGround;
	m_clrItemSelBackGround=clr;
	RedrawWindow();
	return clrTemp;
}
COLORREF CMyListBox::SetTextColor(COLORREF clr/*=-1*/)
{
	if (clr==-1 || clr==m_clrText)
	{
		return m_clrText;
	}	
	COLORREF clrTemp=m_clrText;
	m_clrText=clr;
	RedrawWindow();
	return clrTemp;
}
COLORREF CMyListBox::SetSelTextColor(COLORREF clr/*=-1*/)
{
	if (clr==-1 || clr==m_clrSelText)
	{
		return m_clrSelText;
	}	
	COLORREF clrTemp=m_clrSelText;
	m_clrSelText=clr;
	RedrawWindow();
	return clrTemp;
}
HICON CMyListBox::SetItemIcon(int nIndex,HICON hIcon/*=NULL*/)
{
	if (nIndex<=0) return NULL;
	ItemData *pItemData=(ItemData*)GetItemDataPtr(nIndex-1);
	if (pItemData->hIcon==hIcon || hIcon==NULL)
	{
		return pItemData->hIcon;
	}
	HICON hIconTemp=pItemData->hIcon;
	pItemData->hIcon=hIcon;
	CRect rc;
	GetItemRect(nIndex-1,rc);
	InvalidateRect(rc);
	return hIconTemp;
}
void CMyListBox::OnDestroy() 
{
	CListBox::OnDestroy();
	
	// TODO: Add your message handler code here
	int ItemCount;
	ItemCount = GetCount();
	ItemData *pItemData=NULL;
	for (int i = 0; i < ItemCount; i++)
	{
		pItemData = (ItemData*) GetItemDataPtr(i);
		delete pItemData;
	}
}// CMyListBox
///////////////////////////////////////////////////////////////////////////////////////

HBRUSH CMyListBox::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	return m_dlgBrush;
}

