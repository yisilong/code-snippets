#include "stdafx.h"
#include "MyColorList.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//download by www.codefans.net
CMyColorList::CMyColorList()
{
	m_blnSelect=TRUE;
}
//***************************************************************
CMyColorList::~CMyColorList()
{
	m_ArrayCol.FreeExtra();
	m_ArrayHeaderAlign.FreeExtra();
}
//***************************************************************
BEGIN_MESSAGE_MAP(CMyColorList, CListCtrl)
	//{{AFX_MSG_MAP(CMyColorList)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
void CMyColorList::PreSubclassWindow()
{
	ModifyStyle(0,LVS_OWNERDRAWFIXED);
}
void CMyColorList::DeleteAllItems()
{
	m_ArrayCol.RemoveAll();
	CListCtrl::DeleteAllItems();
}
void CMyColorList::DeleteItem(int nItem)
{
	m_ArrayCol.RemoveAt(nItem);
	CListCtrl::DeleteItem(nItem);
}
void CMyColorList::OnDestroy() 
{	
	CListCtrl::OnDestroy();
}
void CMyColorList::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat , int nWidth , int nSubItem )
{
	CString strText;
	strText.Format("%d",nFormat);
	m_ArrayHeaderAlign.InsertAt(nCol,strText);
	CListCtrl::InsertColumn(nCol,lpszColumnHeading,nFormat,nWidth,nSubItem);
}
//ÉèÖÃÁÐÍ·
void CMyColorList::SetHeader()
{
	VERIFY( m_ctlHeader.SubclassWindow( GetHeaderCtrl()->GetSafeHwnd() ) );
	m_ctlHeader.SetDefault();
}
int CMyColorList::AddItem(int image,COLORREF colText)
{	
	int m=GetItemCount();	
	CMyColor *col=new CMyColor();
	col->colBack=GetBkColor();
	col->colText=colText;
	col->colSelect=RGB(52,148,39);
	m_ArrayCol.InsertAt(m,(CObject *)col);
	return CListCtrl::InsertItem(m,"",image);
}
void CMyColorList::SetItemColor(int nitem,COLORREF colText,COLORREF colBack,COLORREF colSelect)
{
	CMyColor *col=(CMyColor *)m_ArrayCol.GetAt(nitem);
	col->colBack=colBack;
	col->colText=colText;
	col->colSelect=colSelect;
}
void CMyColorList::SetItemColor(int nitem,COLORREF colText)
{
	CMyColor *col=(CMyColor *)m_ArrayCol.GetAt(nitem);
	col->colText=colText;
}
CMyColor CMyColorList::GetItemColor(int nitem)
{
	CMyColor *col=(CMyColor *)m_ArrayCol.GetAt(nitem);
	return (*col);
}
int CMyColorList::AddItem(int image,COLORREF colText,COLORREF colBack,COLORREF colSelect)
{	
	int m=GetItemCount();	
	CMyColor *col=new CMyColor();
	col->colBack=colBack;
	col->colText=colText;
	col->colSelect=colSelect;
	m_ArrayCol.InsertAt(m,(CObject *)col);
	return CListCtrl::InsertItem(m,"",image);
}
//***************************************************************
void CMyColorList::OnPaint() 
{
	CListCtrl::OnPaint();
}
//***************************************************************
void CMyColorList::OnKillFocus(CWnd* pNewWnd) 
{
    CListCtrl::OnKillFocus(pNewWnd);
}
//***************************************************************
void CMyColorList::OnSetFocus(CWnd* pOldWnd) 
{
	CListCtrl::OnSetFocus(pOldWnd);
}

//***************************************************************
void CMyColorList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    if (!lpDrawItemStruct) return;
	CDC*		pDC			= CDC::FromHandle(lpDrawItemStruct->hDC);
	int m=GetHeaderCtrl()->GetItemCount();
	for(int i=0;i<m;i++)
	{
		CString str;
		CRect rcItem;
		GetSubItemRect(lpDrawItemStruct->itemID,i,LVIR_LABEL,rcItem);
		str=GetItemText(lpDrawItemStruct->itemID,i);
		CMyColor *col=(CMyColor *)m_ArrayCol.GetAt(lpDrawItemStruct->itemID);
		pDC->SetBkMode(TRANSPARENT);
		CBrush brush;
		brush.CreateSolidBrush(col->colBack);
		pDC->FillRect(rcItem,&brush);
		pDC->SetTextColor(col->colText);
		LV_ITEM lvi;
		lvi.mask = LVIF_IMAGE | LVIF_STATE;
		lvi.iItem = lpDrawItemStruct->itemID;
		lvi.iSubItem = 0;
		lvi.stateMask = 0xFFFF;		// get all state flags
		GetItem(&lvi);
		if((lvi.state & LVIS_SELECTED)||(lvi.state & LVIS_FOCUSED))
		{
			if(m_blnSelect)
			{
				int r1=GetRValue(col->colSelect);
				int g1=GetGValue(col->colSelect);
				int b1=GetBValue(col->colSelect);
				for(int i=rcItem.Height()/2;i>0;i--)
				{
					r1=(r1+5)>255?255:(r1+5);
					g1=(g1+5)>255?255:(g1+5);
					b1=(b1+5)>255?255:(b1+5);
					CPen pen(PS_SOLID, 1, RGB(r1, g1, b1));
					CPen *old = pDC->SelectObject(&pen);
					pDC->MoveTo(rcItem.left,rcItem.top+i);
					pDC->LineTo(rcItem.right,rcItem.top+i);
					pDC->MoveTo(rcItem.left,rcItem.bottom-i);
					pDC->LineTo(rcItem.right,rcItem.bottom-i);
					pDC->SelectObject(old);
				}
			}
		}
		int nAlign=atoi(m_ArrayHeaderAlign.GetAt(i));
		if(i==0&&GetImageList(LVSIL_SMALL)!=NULL)
		{		
			CPoint ptImage;
			ptImage.x=rcItem.left;
			ptImage.y=rcItem.top;
			GetImageList(LVSIL_SMALL)->Draw(pDC,lvi.iImage,ptImage,ILD_TRANSPARENT);
			pDC->SetBkMode(TRANSPARENT);
			rcItem.left=rcItem.left+16;
			if(nAlign==LVCFMT_LEFT)
				rcItem.left=rcItem.left+8;
		}

		switch(nAlign)
		{
		case LVCFMT_LEFT:
			pDC->DrawText(str,rcItem,DT_LEFT);
			break;
		case LVCFMT_CENTER:
			pDC->DrawText(str,rcItem,DT_CENTER);
			break;
		case LVCFMT_RIGHT:
			pDC->DrawText(str,rcItem,DT_RIGHT);	
			break;
		default:
			pDC->DrawText(str,rcItem,DT_CENTER);
			break;
		}

	}
}
void CMyColorList::InsertItem(int nItem, LPCTSTR lpszItem, int nImage)
{
	int m=GetItemCount();	
	CMyColor *col=new CMyColor();
	col->colBack=RGB(255,255,255);
	col->colText=RGB(0,0,0);
	col->colSelect=RGB(52,148,39);
	m_ArrayCol.InsertAt(m,(CObject *)col);
	CListCtrl::InsertItem(nItem,lpszItem,nImage);
}
