#include "stdafx.h"
#include "MyHeaderCtrl.h"
//download by www.codefans.net
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMyHeaderCtrl::CMyHeaderCtrl()
	: m_iSortColumn( -1 )
	, m_bSortAscending( TRUE )
{
	m_clrBack=RGB(161,192,245);
	m_blnDefault=FALSE;
	m_clrLeft=RGB(97,244,78);
	m_clrRight=RGB(52,148,39);
}
CMyHeaderCtrl::~CMyHeaderCtrl()
{
}
BEGIN_MESSAGE_MAP(CMyHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CMyHeaderCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CMyHeaderCtrl message handlers
void CMyHeaderCtrl::SetSortArrow( const int iSortColumn, const BOOL bSortAscending )
{
	m_iSortColumn = iSortColumn;
	m_bSortAscending = bSortAscending;
	m_blnDefault=FALSE;
	HD_ITEM hditem;
	hditem.mask = HDI_FORMAT;
	VERIFY( GetItem( iSortColumn, &hditem ) );
	hditem.fmt |= HDF_OWNERDRAW;
	VERIFY( SetItem( iSortColumn, &hditem ) );
	Invalidate();
}
void CMyHeaderCtrl::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	CDC dc;
	VERIFY( dc.Attach( lpDrawItemStruct->hDC ) );
	const int iSavedDC = dc.SaveDC();
	CRect rc( lpDrawItemStruct->rcItem );
	CRgn rgn;
	VERIFY( rgn.CreateRectRgnIndirect( &rc ) );
	(void)dc.SelectObject( &rgn );
	VERIFY( rgn.DeleteObject() );
	CBrush brush( GetSysColor( COLOR_3DFACE ) );
	dc.SetBkMode(TRANSPARENT);
		int r1=GetRValue(m_clrBack);
		int g1=GetGValue(m_clrBack);
		int b1=GetBValue(m_clrBack);
		for(int i=rc.Height()/2;i>0;i--)
		{
			r1=(r1+5)>255?255:(r1+5);
			g1=(g1+5)>255?255:(g1+5);
			b1=(b1+5)>255?255:(b1+5);
			CPen pen(PS_SOLID, 1, RGB(r1, g1, b1));
			CPen *old = dc.SelectObject(&pen);
			dc.MoveTo(rc.left,rc.top+i);
			dc.LineTo(rc.right,rc.top+i);
			dc.MoveTo(rc.left,rc.bottom-i);
			dc.LineTo(rc.right,rc.bottom-i);
			dc.SelectObject(old);
		}	
	TCHAR szText[ 256 ];
	HD_ITEM hditem;
	hditem.mask = HDI_TEXT | HDI_FORMAT;
	hditem.pszText = szText;
	hditem.cchTextMax = 255;
	VERIFY( GetItem( lpDrawItemStruct->itemID, &hditem ) );
	UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER | DT_END_ELLIPSIS ;
	if( hditem.fmt & HDF_CENTER)
		uFormat |= DT_CENTER;
	else if( hditem.fmt & HDF_RIGHT)
		uFormat |= DT_RIGHT;
	else
		uFormat |= DT_LEFT;
	if( lpDrawItemStruct->itemState == ODS_SELECTED )
	{
		rc.left++;
		rc.top += 2;
		rc.right++;
	}
	CRect rcIcon( lpDrawItemStruct->rcItem );
	const int iOffset = ( rcIcon.bottom - rcIcon.top ) / 4;
	if( lpDrawItemStruct->itemID == (UINT)m_iSortColumn )
		rc.right -= 3 * iOffset;

	rc.left += iOffset;
	rc.right -= iOffset;
	if( rc.left < rc.right )
		(void)dc.DrawText( szText, -1, rc, uFormat );
	if( lpDrawItemStruct->itemID == (UINT)m_iSortColumn )
	{
		CPen penLight( PS_SOLID, 1,m_clrLeft);
		CPen penShadow( PS_SOLID, 1,m_clrRight);
		CPen* pOldPen = dc.SelectObject( &penLight );
		if( m_bSortAscending )
		{
			dc.MoveTo( rcIcon.right - 2 * iOffset, iOffset);
			dc.LineTo( rcIcon.right - iOffset, rcIcon.bottom - iOffset - 1 );
			dc.LineTo( rcIcon.right - 3 * iOffset - 2, rcIcon.bottom - iOffset - 1 );
			(void)dc.SelectObject( &penShadow );
			dc.MoveTo( rcIcon.right - 3 * iOffset - 1, rcIcon.bottom - iOffset - 1 );
			dc.LineTo( rcIcon.right - 2 * iOffset, iOffset - 1);		
		}
		else
		{
			dc.MoveTo( rcIcon.right - iOffset - 1, iOffset );
			dc.LineTo( rcIcon.right - 2 * iOffset - 1, rcIcon.bottom - iOffset );
			(void)dc.SelectObject( &penShadow );
			dc.MoveTo( rcIcon.right - 2 * iOffset - 2, rcIcon.bottom - iOffset );
			dc.LineTo( rcIcon.right - 3 * iOffset - 1, iOffset );
			dc.LineTo( rcIcon.right - iOffset - 1, iOffset );		
		}
		(void)dc.SelectObject( pOldPen );
	}
	VERIFY( dc.RestoreDC( iSavedDC ) );
	(void)dc.Detach();
}
void CMyHeaderCtrl::Serialize( CArchive& ar )
{
	if( ar.IsStoring() )
	{
		const int iItemCount = GetItemCount();
		if( iItemCount != -1 )
		{
			ar << iItemCount;

			HD_ITEM hdItem = { 0 };
			hdItem.mask = HDI_WIDTH;

			for( int i = 0; i < iItemCount; i++ )
			{
				VERIFY( GetItem( i, &hdItem ) );
				ar << hdItem.cxy;
			}
		}
	}
	else
	{
		int iItemCount;
		ar >> iItemCount;
		
		if( GetItemCount() != iItemCount )
			TRACE0( _T("Different number of columns in registry.") );
		else
		{
			HD_ITEM hdItem = { 0 };
			hdItem.mask = HDI_WIDTH;

			for( int i = 0; i < iItemCount; i++ )
			{
				ar >> hdItem.cxy;
				VERIFY( SetItem( i, &hdItem ) );
			}
		}
	}
}
void CMyHeaderCtrl::SetDefault()
{
	for(int i=0;i<GetItemCount();i++)
	{	
		HD_ITEM hditem;
		hditem.mask = HDI_FORMAT;
		VERIFY( GetItem( i, &hditem ) );
		hditem.fmt |= HDF_OWNERDRAW;
		VERIFY( SetItem( i, &hditem ) );
		Invalidate();
	}
}
