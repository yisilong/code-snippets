//download by www.codefans.net
#include "stdafx.h"
#include "MyListCtrl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
struct ItemData
{
public:
  ItemData() : arrpsz( NULL ), dwData( NULL ) {}
  LPTSTR* arrpsz;
  DWORD dwData;  
private:
  ItemData( const ItemData& );
  ItemData& operator=( const ItemData& );
};
CMyListCtrl::CMyListCtrl()
: m_iNumColumns( 0 )
, m_iSortColumn( -1 )
, m_bSortAscending( TRUE )
,blnMenu(TRUE)
,itemX(-1)
,itemY(-1)
,blnAllEdit(TRUE)
,m_EditStyle(ctr_Edit_DblClick)
{
}
CMyListCtrl::~CMyListCtrl()
{
	CboArrayIndex.FreeExtra();
	strArrayEditStyle.FreeExtra();
	for(int i=0;i<CboArrayData.GetSize();i++)
	{
		CStringArray *strArray=(CStringArray *)CboArrayData.GetAt(i);
		strArray->FreeExtra();
	}
	CboArrayData.FreeExtra();
	NumArrayIndex.FreeExtra();
	NumArrayData.FreeExtra();
}
BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
//{{AFX_MSG_MAP(CMyListCtrl)
ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
ON_WM_DESTROY()
ON_WM_RBUTTONDOWN( )
ON_WM_LBUTTONDOWN( )
ON_WM_LBUTTONDBLCLK( )
ON_WM_PARENTNOTIFY()
ON_WM_HSCROLL()
ON_WM_VSCROLL()
ON_COMMAND(ID_MENU_list, OnMenuitemlist)
ON_COMMAND(ID_MENU_icon, OnMenuitemicon)
ON_COMMAND(ID_MENU_smallicon, OnMenuitemsmallicon)
ON_COMMAND(ID_MENU_report, OnMenuitemreport)
ON_COMMAND(ID_MENU_No,OnMenuNo)
ON_COMMAND(ID_MENU_Click,OnMenuClick)
ON_COMMAND(ID_MENU_DblClick,OnMenuDblClick)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
void CMyListCtrl::PreSubclassWindow()
{  
	ctl_EditText.Create(ES_AUTOHSCROLL|WS_CHILD|ES_LEFT|ES_WANTRETURN|WS_BORDER,CRect(0,0,0,0),this,ID_EditText);
	ctl_EditDateTime.Create(ES_AUTOHSCROLL|WS_CHILD|ES_CENTER|ES_WANTRETURN|WS_BORDER,CRect(0,0,0,0),this,ID_EditDateTime);
 	ctl_EditComboDown.Create(WS_CHILD|CBS_DROPDOWN |WS_VSCROLL | WS_TABSTOP,CRect(0,0,0,0),this,ID_EditCboDown);
 	ctl_EditComboDownList.Create(WS_CHILD|CBS_DROPDOWNLIST |WS_VSCROLL | WS_TABSTOP,CRect(0,0,0,0),this,ID_EditCboList);
	ctl_EditNum.Create(ES_AUTOHSCROLL|WS_CHILD|ES_LEFT|ES_WANTRETURN|WS_BORDER,CRect(0,0,0,0),this,ID_EditText);
	CFont *ft=GetFont();
	ctl_EditComboDown.SetFont(ft,TRUE);
	ctl_EditText.SetFont(ft,TRUE);
	ctl_EditDateTime.SetFont(ft,TRUE);
	ctl_EditNum.SetFont(ft,TRUE);
	ctl_EditComboDownList.SetFont(ft,TRUE);
	ctl_EditComboDownList.SetExtendedUI(TRUE);
	ctl_EditComboDown.SetExtendedUI(TRUE);
//	ModifyStyle(0,LVS_OWNERDRAWFIXED);
	CListCtrl::PreSubclassWindow();
}
//添加一项
int CMyListCtrl::AddItem(int image)
{
	LPTSTR* arrpsz = new LPTSTR[ m_iNumColumns ];
	for(int i=0;i<m_iNumColumns;i++)
	{
	  arrpsz[ i ] = new TCHAR[1];
	  (void)lstrcpy( arrpsz[ i ], "" );
	}
	int iIndex=InsertItem(GetItemCount(),"",image);
	VERIFY( SetTextArray( iIndex, arrpsz ) );
	return iIndex;
}
//设置排列方式
void CMyListCtrl::SetMyStyle(DWORD dwNewStyle)
{	
	if(itemX>=0&& itemY>=0)
	SetEditPos(FALSE,itemX,itemY,FALSE);
	DWORD oldStyle;
	oldStyle=GetWindowLong(GetSafeHwnd(),GWL_STYLE);
	if((oldStyle&LVS_TYPEMASK)!=dwNewStyle)
	{
		oldStyle&=~LVS_TYPEMASK;
		oldStyle|=dwNewStyle;
		SetWindowLong(GetSafeHwnd(),GWL_STYLE,oldStyle);
	}
}
//返回排列方式
DWORD CMyListCtrl::GetMyStyle()
{
	DWORD oldStyle,newStyle;
	oldStyle=GetWindowLong(GetSafeHwnd(),GWL_STYLE);
	newStyle=oldStyle&LVS_TYPEMASK;
	return newStyle;
}
void CMyListCtrl::OnMenuitemlist()
{
	SetMyStyle(LVS_LIST);
}
void CMyListCtrl::OnMenuitemicon()
{
	SetMyStyle(LVS_ICON);
}
void CMyListCtrl::OnMenuitemreport()
{
	SetMyStyle(LVS_REPORT);
}
void CMyListCtrl::OnMenuitemsmallicon()
{
	SetMyStyle(LVS_SMALLICON);
}
void CMyListCtrl::OnMenuClick ()
{
	m_EditStyle=ctr_Edit_Click;
	SetEditPos(FALSE,itemX,itemY,FALSE);
}
void CMyListCtrl::OnMenuDblClick()
{
	m_EditStyle=ctr_Edit_DblClick;
	SetEditPos(FALSE,itemX,itemY,FALSE);
}
void CMyListCtrl::OnMenuNo()
{
	if(m_EditStyle==ctr_Edit_No) return;
	m_EditStyle=ctr_Edit_No;
	if(!blnAllEdit) return;
	if(itemY<0) return;
	CString strText;
		switch(GetColumnStyle(itemY))
		{
		case ctr_Edit_CboDownList:
			ctl_EditComboDownList.ShowDropDown(FALSE);
			ctl_EditComboDownList.GetWindowText(strText);
			SetItemText(itemX,itemY,strText);
			ctl_EditComboDownList.ShowWindow(SW_HIDE);
			break;
		case ctr_Edit_CboDown:
			ctl_EditComboDown.ShowDropDown(FALSE);
			ctl_EditComboDown.GetWindowText(strText);
			SetItemText(itemX,itemY,strText);
			ctl_EditComboDown.ShowWindow(SW_HIDE);
			break;
		case ctr_Edit_Text:
			ctl_EditText.GetWindowText(strText);
			SetItemText(itemX,itemY,strText);
			ctl_EditText.ShowWindow(SW_HIDE);
			break;
		case ctr_Edit_Time:
			strText=ctl_EditDateTime.GetDetectText();
			SetItemText(itemX,itemY,strText);
			ctl_EditDateTime.ShowWindow(SW_HIDE);
			break;
		case ctr_Edit_Date:
			strText=ctl_EditDateTime.GetDetectText();
			SetItemText(itemX,itemY,strText);
			ctl_EditDateTime.ShowWindow(SW_HIDE);
		case ctr_Edit_DateTime:
			strText=ctl_EditDateTime.GetDetectText();
			SetItemText(itemX,itemY,strText);
			ctl_EditDateTime.ShowWindow(SW_HIDE);
			break;
		case ctr_Edit_Num:
			strText=ctl_EditNum.GetText();
			SetItemText(itemX,itemY,strText);
			ctl_EditNum.ShowWindow(SW_HIDE);
			break;
		default:
			ctl_EditText.ShowWindow(SW_HIDE);
			ctl_EditDateTime.ShowWindow(SW_HIDE);
			ctl_EditComboDown.ShowWindow(SW_HIDE);
			ctl_EditNum.ShowWindow(SW_HIDE);
			break;
		}
}
//右键单击显示菜单
void CMyListCtrl::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	if(itemY>=0) SetEditPos(FALSE,itemX,itemY,FALSE);
	if(m_EditStyle==ctr_Edit_DblClick)
	{
		int i=itemY;
		BOOL bln;
		if(itemY>=0) SetEditPos(FALSE,itemX,itemY,FALSE);
		GetItemPos(point);
		bln=(i==itemY);
		SetEditPos(TRUE,itemX,itemY,bln);
	}
	CListCtrl::OnLButtonDblClk(nFlags,point);
}
void CMyListCtrl::OnLButtonDown( UINT nFlags, CPoint point )
{
	if(itemY>=0) SetEditPos(FALSE,itemX,itemY,FALSE);
	if(m_EditStyle==ctr_Edit_Click)
	{
		int i=itemY;
		BOOL bln;
		if(itemY>=0) SetEditPos(FALSE,itemX,itemY,FALSE);
		GetItemPos(point);
		bln=(i==itemY);
		SetEditPos(TRUE,itemX,itemY,bln);	}
	CListCtrl::OnLButtonDown(nFlags,point);
}
void CMyListCtrl::GetItemPos(CPoint point)
{
	LVHITTESTINFO lvhti;
    lvhti.pt = point;
	int nItem = CListCtrl::SubItemHitTest(&lvhti);
	if(nItem == -1)
	   return;
	int nSubItem = lvhti.iSubItem;
	itemX=nItem;
	itemY=nSubItem;	
}
void CMyListCtrl::SetEditPos(BOOL blnShow,int itemX,int itemY,BOOL blnCbo)
{
	if(!blnAllEdit) return;
	if(itemX<0||itemY<0) return;
	if(m_EditStyle==ctr_Edit_No) return;
	CString strText;
	int i;
	if(GetMyStyle()!=LVS_REPORT) return;
	if(!blnShow)
	{
		switch(GetColumnStyle(itemY))
		{
		case ctr_Edit_CboDownList:
			ctl_EditComboDownList.ShowDropDown(FALSE);
			ctl_EditComboDownList.GetWindowText(strText);
			SetItemText(itemX,itemY,strText);
			ctl_EditComboDownList.ShowWindow(SW_HIDE);
			break;
		case ctr_Edit_CboDown:
			ctl_EditComboDown.ShowDropDown(FALSE);
			ctl_EditComboDown.GetWindowText(strText);
			SetItemText(itemX,itemY,strText);
			ctl_EditComboDown.ShowWindow(SW_HIDE);
			break;
		case ctr_Edit_Text:
			ctl_EditText.GetWindowText(strText);
			SetItemText(itemX,itemY,strText);
			ctl_EditText.ShowWindow(SW_HIDE);
			break;
		case ctr_Edit_Time:
			strText=ctl_EditDateTime.GetDetectText();
			SetItemText(itemX,itemY,strText);
			ctl_EditDateTime.ShowWindow(SW_HIDE);
			break;
		case ctr_Edit_Date:
			strText=ctl_EditDateTime.GetDetectText();
			SetItemText(itemX,itemY,strText);
			ctl_EditDateTime.ShowWindow(SW_HIDE);
		case ctr_Edit_DateTime:
			strText=ctl_EditDateTime.GetDetectText();
			SetItemText(itemX,itemY,strText);
			ctl_EditDateTime.ShowWindow(SW_HIDE);
			break;
		case ctr_Edit_Num:
			strText=ctl_EditNum.GetText();
			SetItemText(itemX,itemY,strText);
			ctl_EditNum.ShowWindow(SW_HIDE);
			break;
		default:
			ctl_EditText.ShowWindow(SW_HIDE);
			ctl_EditDateTime.ShowWindow(SW_HIDE);
			ctl_EditComboDown.ShowWindow(SW_HIDE);
			ctl_EditNum.ShowWindow(SW_HIDE);
			break;
		}
		//Invalidate(TRUE);
		return;
	}
	CRect rcCtrl;
	strText=GetItemText(itemX,itemY);
	CListCtrl::GetSubItemRect(itemX,itemY,LVIR_LABEL,rcCtrl);
	switch(GetColumnStyle(itemY))
	{
	case ctr_Edit_CboDownList:
		if(!blnCbo) FillCboData(itemY);
		rcCtrl.bottom=rcCtrl.top+150;
		i=ctl_EditComboDownList.FindString(0,strText);
		if(i>=0) 
			ctl_EditComboDownList.SetCurSel(i);
		else 
			ctl_EditComboDownList.SetCurSel(0);
		ctl_EditComboDownList.MoveWindow(rcCtrl,TRUE);
		ctl_EditComboDownList.ShowWindow(SW_SHOW);
		::SetFocus(ctl_EditComboDownList.m_hWnd);
		break;
	case ctr_Edit_CboDown:
		if(!blnCbo) FillCboData(itemY);
		rcCtrl.bottom=rcCtrl.top+150;
		i=ctl_EditComboDown.FindString(0,strText);
		if(i>=0) ctl_EditComboDown.SetCurSel(i);
		ctl_EditComboDown.SetWindowText(strText);
		ctl_EditComboDown.MoveWindow(rcCtrl,TRUE);
		ctl_EditComboDown.ShowWindow(SW_SHOW);
		::SetFocus(ctl_EditComboDown.m_hWnd);
		break;
	case ctr_Edit_Date:
		ctl_EditDateTime.SetClassType(edit_date);
		ctl_EditDateTime.SetDetectText(strText);
		ctl_EditDateTime.MoveWindow(rcCtrl,TRUE);
		ctl_EditDateTime.ShowWindow(SW_SHOW);
		::SetFocus(ctl_EditDateTime.m_hWnd);
		ctl_EditDateTime.SetSel(0,strText.GetLength(),TRUE);
		break;
	case ctr_Edit_DateTime:
		ctl_EditDateTime.SetClassType(edit_datetime);
		ctl_EditDateTime.SetDetectText(strText);
		ctl_EditDateTime.MoveWindow(rcCtrl,TRUE);
		ctl_EditDateTime.ShowWindow(SW_SHOW);
		::SetFocus(ctl_EditDateTime.m_hWnd);
		ctl_EditDateTime.SetSel(0,strText.GetLength(),TRUE);
		break;
	case ctr_Edit_Time:
		ctl_EditDateTime.SetClassType(edit_time);		
		ctl_EditDateTime.SetDetectText(strText);
		ctl_EditDateTime.MoveWindow(rcCtrl,TRUE);
		ctl_EditDateTime.ShowWindow(SW_SHOW);
		::SetFocus(ctl_EditDateTime.m_hWnd);
		ctl_EditDateTime.SetSel(0,strText.GetLength(),TRUE);
		break;
	case ctr_Edit_Text:
		ctl_EditText.SetWindowText(strText);
		ctl_EditText.MoveWindow(rcCtrl,TRUE);
		ctl_EditText.ShowWindow(SW_SHOW);
		::SetFocus(ctl_EditText.m_hWnd);
		ctl_EditText.SetSel(0,strText.GetLength(),TRUE);
		break;
	case ctr_Edit_Num:
		ctl_EditNum.blnAuto=GetNumEditAuto(itemY);
		ctl_EditNum.SetNumType(GetNumEditStyle(itemY)->x,GetNumEditStyle(itemY)->y);
		ctl_EditNum.SetText(strText);
		ctl_EditNum.MoveWindow(rcCtrl,TRUE);
		ctl_EditNum.ShowWindow(SW_SHOW);
		::SetFocus(ctl_EditNum.m_hWnd);
		ctl_EditNum.SetSel(0,strText.GetLength(),TRUE);
		break;
	default:
		break;
	}
}
void CMyListCtrl::SetItemFont(CFont ft)
{
	ctl_EditComboDown.SetFont(&ft,TRUE);
	ctl_EditText.SetFont(&ft,TRUE);
	ctl_EditDateTime.SetFont(&ft,TRUE);
	ctl_EditNum.SetFont(&ft,TRUE);
	ctl_EditComboDownList.SetFont(&ft,TRUE);	
}
void CMyListCtrl::OnRButtonDown( UINT nFlags, CPoint point )
{
	if(!blnMenu) return;
	CMenu m_MyRightMenu;
	m_MyRightMenu.CreatePopupMenu();
	m_MyRightMenu.AppendMenu(MF_STRING,ID_MENU_No,"无模式");
	m_MyRightMenu.AppendMenu(MF_STRING,ID_MENU_Click,"单击模式");
	m_MyRightMenu.AppendMenu(MF_STRING,ID_MENU_DblClick,"双击模式");
	m_MyRightMenu.AppendMenu(MF_SEPARATOR,ID_MENU_sep,"");
	m_MyRightMenu.AppendMenu(MF_STRING,ID_MENU_icon,"大图标");
	m_MyRightMenu.AppendMenu(MF_STRING,ID_MENU_smallicon,"小图标");
	m_MyRightMenu.AppendMenu(MF_STRING,ID_MENU_list,"列表");
	m_MyRightMenu.AppendMenu(MF_STRING,ID_MENU_report,"详细信息");
	DWORD newStyle=GetMyStyle();
	switch(newStyle)
	{
	case LVS_ICON:
		m_MyRightMenu.CheckMenuItem(ID_MENU_icon,MF_CHECKED);
		break;
	case LVS_SMALLICON:
		m_MyRightMenu.CheckMenuItem(ID_MENU_smallicon,MF_CHECKED);
		break;
	case LVS_LIST:
		m_MyRightMenu.CheckMenuItem(ID_MENU_list,MF_CHECKED);
		break;
	case LVS_REPORT:
		m_MyRightMenu.CheckMenuItem(ID_MENU_report,MF_CHECKED);
		break;
	default:
		break;
	}
	ClientToScreen(&point);
	m_MyRightMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this,NULL);
	m_MyRightMenu.DestroyMenu();
	CListCtrl::OnRButtonDown(nFlags,point);
}
//添加单列
int CMyListCtrl::InsertColumnX(CString strName,int nFormat,int iWidth,int intEdit)
{
	int i;
	i=InsertColumn( m_iNumColumns++, strName, nFormat, iWidth );
	if(i!=-1)
	{
		CString str;
		str.Format("%d",intEdit);
		strArrayEditStyle.InsertAt(i,str);
		if(intEdit==ctr_Edit_CboDown||intEdit==ctr_Edit_CboDownList)
		{
			CStringArray *strArray=new CStringArray;
			str.Format("%d",i);
			i=CboArrayIndex.GetSize();
			CboArrayIndex.InsertAt(i,str);
			CboArrayData.InsertAt(i,strArray);
		}
		if(intEdit==ctr_Edit_Num)
		{
			str.Format("%d",i);
			i=NumArrayIndex.GetSize();
			NumArrayIndex.InsertAt(i,str);
			CPoint *pt=new CPoint(10,0);
			NumArrayData.InsertAt(i,(CObject *)pt);
			str="0";
			NumArrayAuto.InsertAt(i,str);
		}
	}
	return i;
}
void CMyListCtrl::AddCboItem(int nItem,CString strText)
{
	CStringArray *strArray=GetCboArrayData(nItem);
	strArray->Add(strText);
}
void CMyListCtrl::RemoveCboItem(int nitem,CString strText)
{
	CStringArray *strArray=GetCboArrayData(nitem);
	for(int i=0;i<strArray->GetSize();i++)
	{	
		if(strArray->GetAt(i)==strText)
		{
			strArray->RemoveAt(i,strText.GetLength());
			return;
		}
	}
}
void CMyListCtrl::SetNumEditAuto(int nitem,BOOL blnAuto)
{
	if(GetColumnStyle(nitem)!=ctr_Edit_Num) return;
	CString str;
	str.Format("%d",nitem);
	int i=0;
	while(NumArrayIndex.GetAt(i)!=str && i<NumArrayIndex.GetSize())
	{
		i=i+1;
	}
	if(i==NumArrayIndex.GetSize() && NumArrayIndex.GetAt(i)!=str) return;
	if(blnAuto)
		str="1";
	else
		str="0";
	NumArrayAuto.SetAt(i,str);
}
BOOL CMyListCtrl::GetNumEditAuto(int nitem)
{
	if(GetColumnStyle(nitem)!=ctr_Edit_Num) return NULL;
	CString str;
	str.Format("%d",nitem);
	int i=0;
	while(NumArrayIndex.GetAt(i)!=str && i<NumArrayIndex.GetSize())
	{
		i=i+1;
	}
	if(i==NumArrayIndex.GetSize() && NumArrayIndex.GetAt(i)!=str) return NULL;
	str=NumArrayAuto.GetAt(i);
	BOOL bln;
	if(str=="1")
		bln=TRUE;
	else
		bln=FALSE;
	return bln;
}
void CMyListCtrl::SetNumEditStyle(int nitem,int intLeft,int intRight)
{
	CPoint *pt=GetNumEditStyle(nitem);
	pt->x=intLeft;
	pt->y=intRight;
}
CPoint *CMyListCtrl::GetNumEditStyle(int nitem)
{
	CString str;
	str.Format("%d",nitem);
	int i=0;
	while(NumArrayIndex.GetAt(i)!=str && i<NumArrayIndex.GetSize())
	{
		i=i+1;
	}
	if(i==NumArrayIndex.GetSize() && NumArrayIndex.GetAt(i)!=str) return NULL;
	CPoint *pt1=(CPoint *)NumArrayData.GetAt(i);
	return pt1;
}
void CMyListCtrl::FillCboData(int nitem)
{
	//ctl_EditComboDown.DeleteString()
	if(GetColumnStyle(nitem)==ctr_Edit_CboDown)
	{
		ctl_EditComboDown.ShowDropDown(FALSE);
		ctl_EditComboDown.ResetContent();
		CStringArray *strArry=GetCboArrayData(nitem);
		for(int i=0;i<strArry->GetSize();i++)
		{
			ctl_EditComboDown.AddString(strArry->GetAt(i));
		}
	}
	if(GetColumnStyle(nitem)==ctr_Edit_CboDownList)
	{
		ctl_EditComboDownList.ShowDropDown(FALSE);
		ctl_EditComboDownList.ResetContent();
		ctl_EditComboDownList.AddString("");
		CStringArray *strArry=GetCboArrayData(nitem);
		for(int i=0;i<strArry->GetSize();i++)
		{
			ctl_EditComboDownList.AddString(strArry->GetAt(i));
		}
	}

}
CStringArray *CMyListCtrl::GetCboArrayData(int nitem)
{
	CString str;
	str.Format("%d",nitem);
	int i=0;
	while(CboArrayIndex.GetAt(i)!=str && i<CboArrayIndex.GetSize())
	{
		i=i+1;
	}
	if(i==CboArrayIndex.GetSize() && CboArrayIndex.GetAt(i)!=str) return NULL;
	CStringArray *strArray=(CStringArray *)CboArrayData.GetAt(i);
	return strArray;
}
//设置单列
void CMyListCtrl::SetColumnStyle(int item,int intEdit)
{
	CString str;
	str.Format("%d",intEdit);
	strArrayEditStyle.SetAt(item,str);
}
//得到单列
int CMyListCtrl::GetColumnStyle(int item)
{
	CString str;
	str=strArrayEditStyle.GetAt(item);
	return(atoi(str));
}
//设置列头
BOOL CMyListCtrl::SetHeader()
{
	VERIFY( m_ctlHeader.SubclassWindow( GetHeaderCtrl()->GetSafeHwnd() ) );
	m_ctlHeader.SetDefault();
	return TRUE;
}
//释放内存
void CMyListCtrl::FreeItemMemory( const int iItem )
{
  ItemData* pid = reinterpret_cast<ItemData*>( CListCtrl::GetItemData( iItem ) ); 
  LPTSTR* arrpsz = pid->arrpsz;
  for( int i = 0; i < m_iNumColumns; i++ )
    delete[] arrpsz[ i ];
  delete[] arrpsz;
  delete pid;
  VERIFY( CListCtrl::SetItemData( iItem, NULL ) );
}
//删除单项
BOOL CMyListCtrl::DeleteItem( int iItem )
{
  FreeItemMemory( iItem );
  return CListCtrl::DeleteItem( iItem );
}
//删除单项
BOOL CMyListCtrl::DeleteAllItems()
{
  for( int iItem = 0; iItem < GetItemCount(); iItem ++ )
    FreeItemMemory( iItem );
  
  return CListCtrl::DeleteAllItems();
}
//数字型的判断
bool IsNumber( LPCTSTR pszText )
{
  ASSERT_VALID_STRING(pszText);
  int iDotCount=0;
  for(int i=0;i<lstrlen(pszText);i++){
    if(!_istdigit(pszText[i])){
      if(pszText[i]!='.')
        return false;
      iDotCount++;
      if(iDotCount>1)return false;
    }
  }
  return true;
}
//数字型的比较
int NumberCompare( LPCTSTR pszNumber1, LPCTSTR pszNumber2 )
{
  ASSERT_VALID_STRING(pszNumber1);
  ASSERT_VALID_STRING(pszNumber2);
  const double fNumber1=atof(pszNumber1);
  const double fNumber2=atof(pszNumber2);
  if(fNumber1<fNumber2)
    return -1;
  else if(fNumber1>fNumber2)
    return 1;
  return 0;
}
//时间性的判断
bool IsTime(LPCTSTR pszText)
{
  ASSERT_VALID_STRING( pszText );
  if( lstrlen( pszText ) != 8 )
    return false;
  
  return _istdigit(pszText[0])
    && _istdigit(pszText[1])
    && pszText[2]==_T(':')
    && _istdigit( pszText[ 3 ] )
    && _istdigit( pszText[ 4 ] )
    && pszText[5]==_T(':')
    && _istdigit( pszText[ 6 ] )
    && _istdigit( pszText[ 7 ] );
}
//日期型的判断
bool IsDate( LPCTSTR pszText )
{
  ASSERT_VALID_STRING( pszText );
  if( lstrlen( pszText ) != 10 )
    return false;
  
  return _istdigit(pszText[0])
    && _istdigit(pszText[1])
    && pszText[2]==_T('-')
    && _istdigit( pszText[ 3 ] )
    && _istdigit( pszText[ 4 ] )
    && pszText[5]==_T('-')
    && _istdigit( pszText[ 6 ] )
    && _istdigit( pszText[ 7 ] )
    && _istdigit( pszText[ 8 ] )
    && _istdigit( pszText[ 9 ] );
}
//日期时间型的判断
bool IsDateTime(LPCTSTR pszText)
{
  ASSERT_VALID_STRING( pszText );
  if( lstrlen( pszText ) != 18 )
    return false;
  return _istdigit(pszText[0])
    && _istdigit(pszText[1])
    && pszText[2]==_T(':')
    && _istdigit( pszText[ 3 ] )
    && _istdigit( pszText[ 4 ] )
    && pszText[5]==_T(':')
    && _istdigit( pszText[ 6 ] )
    && _istdigit( pszText[ 7 ] )
	&& _istdigit(pszText[ 8 ])
	&& _istdigit(pszText[9])
    && _istdigit(pszText[10])
    && pszText[11]==_T('-')
    && _istdigit( pszText[ 12 ] )
    && _istdigit( pszText[ 13] )
    && pszText[14]==_T('-')
    && _istdigit( pszText[ 15 ] )
    && _istdigit( pszText[ 16 ] )
    && _istdigit( pszText[ 17 ] )
    && _istdigit( pszText[ 18 ] );
}
//日期和时间性的比较
int DateTimeComPare(const CString &strTime1,const CString &strTime2)
{
  const int iHour1=atoi(strTime1.Mid(6, 2));
  const int iHour2=atoi(strTime2.Mid(6, 2));
  if(iHour1<iHour2)
    return -1;
  else if(iHour1>iHour2)
    return 1;
  const int iMin1=atoi(strTime1.Mid(3, 2));
  const int iMin2=atoi(strTime2.Mid(3, 2));
  if(iMin1<iMin2)
    return -1;  
  else if(iMin1>iMin2)
    return 1;
  const int iSec1=atoi(strTime1.Mid(0, 2));
  const int iSec2=atoi(strTime2.Mid(0, 2));
  if(iSec1<iSec2)
    return -1;
  else if(iSec1>iSec2)
    return 1;

  const int iYear1=atoi(strTime1.Mid(15, 4));
  const int iYear2=atoi(strTime2.Mid(15, 4));
  
  if(iYear1<iYear2)
    return -1;
  else if(iYear1>iYear2)
    return 1;
  
  const int iMonth1=atoi(strTime1.Mid(12, 2));
  const int iMonth2=atoi(strTime2.Mid(12, 2));
  
  if(iMonth1<iMonth2)
    return -1;  
  else if(iMonth1>iMonth2)
    return 1;
  
  const int iDay1=atoi(strTime1.Mid(9, 2));
  const int iDay2=atoi(strTime2.Mid(9, 2));
  
  if(iDay1<iDay2)
    return -1;
  else if(iDay1>iDay2)
    return 1;
  return 0;
}
//时间型的比较
int TimeCompare(const CString &strTime1,const CString &strTime2)
{
  const int iHour1=atoi(strTime1.Mid(6, 2));
  const int iHour2=atoi(strTime2.Mid(6, 2));
  
  if(iHour1<iHour2)
    return -1;
  else if(iHour1>iHour2)
    return 1;
  const int iMin1=atoi(strTime1.Mid(3, 2));
  const int iMin2=atoi(strTime2.Mid(3, 2));
  if(iMin1<iMin2)
    return -1;  
  else if(iMin1>iMin2)
    return 1;
  const int iSec1=atoi(strTime1.Mid(0, 2));
  const int iSec2=atoi(strTime2.Mid(0, 2));
  if(iSec1<iSec2)
    return -1;
  else if(iSec1>iSec2)
    return 1;

  return 0;
}
//日期型的比较
int DateCompare( const CString& strDate1, const CString& strDate2 )
{
  const int iYear1=atoi(strDate1.Mid(6, 4));
  const int iYear2=atoi(strDate2.Mid(6, 4));
  
  if(iYear1<iYear2)
    return -1;
  else if(iYear1>iYear2)
    return 1;
  
  const int iMonth1=atoi(strDate1.Mid(3, 2));
  const int iMonth2=atoi(strDate2.Mid(3, 2));
  
  if(iMonth1<iMonth2)
    return -1;  
  else if(iMonth1>iMonth2)
    return 1;
  
  const int iDay1=atoi(strDate1.Mid(0, 2));
  const int iDay2=atoi(strDate2.Mid(0, 2));
  
  if(iDay1<iDay2)
    return -1;
  else if(iDay1>iDay2)
    return 1;

  return 0;
}
int CALLBACK CMyListCtrl::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
  CMyListCtrl* pListCtrl = reinterpret_cast<CMyListCtrl*>( lParamData );
  ASSERT( pListCtrl->IsKindOf( RUNTIME_CLASS( CListCtrl ) ) );
  
  ItemData* pid1 = reinterpret_cast<ItemData*>( lParam1 );
  ItemData* pid2 = reinterpret_cast<ItemData*>( lParam2 );
  
  ASSERT( pid1 );
  ASSERT( pid2 );
  
  LPCTSTR pszText1 = pid1->arrpsz[ pListCtrl->m_iSortColumn ];
  LPCTSTR pszText2 = pid2->arrpsz[ pListCtrl->m_iSortColumn ];
  
  ASSERT_VALID_STRING( pszText1 );
  ASSERT_VALID_STRING( pszText2 );
  
	if(IsNumber(pszText1))
    return pListCtrl->m_bSortAscending ? NumberCompare( pszText1, pszText2 ) : NumberCompare( pszText2, pszText1 );
	else if(IsDate(pszText1))
    return pListCtrl->m_bSortAscending ? DateCompare( pszText1, pszText2 ) : DateCompare( pszText2, pszText1 );
	else if(IsTime(pszText1))
    return pListCtrl->m_bSortAscending ? TimeCompare( pszText1, pszText2 ) : TimeCompare( pszText2, pszText1 );
	else if(IsDateTime(pszText1))
    return pListCtrl->m_bSortAscending ? DateTimeComPare( pszText1, pszText2 ) : DateTimeComPare( pszText2, pszText1 );
    else
	return pListCtrl->m_bSortAscending ? lstrcmp( pszText1, pszText2 ) : lstrcmp( pszText2, pszText1 );
}
void CMyListCtrl::OnColumnClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	SetEditPos(FALSE,itemX,itemY,FALSE);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	const int iColumn = pNMListView->iSubItem;
	Sort( iColumn, iColumn == m_iSortColumn ? !m_bSortAscending : TRUE );
	*pResult = 0;
}
void CMyListCtrl::Sort( int iColumn, BOOL bAscending )
{
  m_iSortColumn = iColumn;
  m_bSortAscending = bAscending;
  m_ctlHeader.SetSortArrow( m_iSortColumn, m_bSortAscending );
  VERIFY( SortItems( CompareFunction, reinterpret_cast<DWORD>( this ) ) );
}
void CMyListCtrl::OnDestroy() 
{
  for( int iItem = 0; iItem < GetItemCount(); iItem ++ )
    FreeItemMemory( iItem );
  
  CListCtrl::OnDestroy();
}
BOOL CMyListCtrl::SetItemText( int nItem, int nSubItem, LPCTSTR lpszText )
{
  if( !CListCtrl::SetItemText( nItem, nSubItem, lpszText ) )
    return FALSE;
  
  LPTSTR* arrpsz = GetTextArray( nItem );
  LPTSTR pszText = arrpsz[ nSubItem ];
  delete[] pszText;
  pszText = new TCHAR[ lstrlen( lpszText ) + 1 ];
  (void)lstrcpy( pszText, lpszText );
  arrpsz[ nSubItem ] = pszText;
  
  return TRUE;
}
BOOL CMyListCtrl::SetItemData( int nItem, DWORD dwData )
{
  if( nItem >= GetItemCount() )
    return FALSE;
  
  ItemData* pid = reinterpret_cast<ItemData*>( CListCtrl::GetItemData( nItem ) );
  ASSERT( pid );
  pid->dwData = dwData;
  
  return TRUE;
}
DWORD CMyListCtrl::GetItemData( int nItem ) const
{
  ASSERT( nItem < GetItemCount() );
  
  ItemData* pid = reinterpret_cast<ItemData*>( CListCtrl::GetItemData( nItem ) );
  ASSERT( pid );
  return pid->dwData;
}
BOOL CMyListCtrl::SetTextArray( int iItem, LPTSTR* arrpsz )
{
  ASSERT( CListCtrl::GetItemData( iItem ) == NULL );
  ItemData* pid = new ItemData;
  pid->arrpsz = arrpsz;
  return CListCtrl::SetItemData( iItem, reinterpret_cast<DWORD>( pid ) );
}
LPTSTR* CMyListCtrl::GetTextArray( int iItem ) const
{
  ASSERT( iItem < GetItemCount() );
  
  ItemData* pid = reinterpret_cast<ItemData*>( CListCtrl::GetItemData( iItem ) );
  return pid->arrpsz;
}
void CMyListCtrl::SetShowMenu(BOOL blnShow)
{
	blnMenu=blnShow;
}
BOOL CMyListCtrl::PreTranslateMessage(MSG* pMsg)
{	
	int i=itemX,j=itemY;
	CRect rcCtrl;
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_TAB)
		{
			if((itemY+1)<=m_iNumColumns)
			{
				SetEditPos(FALSE,i,j,FALSE);
				if(FALSE == Key_Ctrl(i,j))
                    Key_Shift(i,j);
				CListCtrl::GetSubItemRect(i,j,LVIR_LABEL,rcCtrl); 
				if(m_EditStyle==ctr_Edit_Click)
				OnLButtonDown(0,CPoint(rcCtrl.left+1,rcCtrl.top+1));
				else
				OnLButtonDblClk(0,CPoint(rcCtrl.left+1,rcCtrl.top+1));
				POSITION pos = CListCtrl::GetFirstSelectedItemPosition();
				 if (pos == NULL)
				 {
				 }
				 else
				 {
					 while (pos)
					 {
						 int ntpItem = CListCtrl::GetNextSelectedItem(pos);
						 CListCtrl::SetItemState(ntpItem,0,LVIS_SELECTED);
					 }
				 }
				 CListCtrl::SetItemState(i,  LVIS_SELECTED,  LVIS_SELECTED);
				return TRUE;
			}
		}
		else if(pMsg->wParam == VK_RETURN)
		{
			short sRet = GetKeyState(VK_CONTROL);
			sRet = sRet >>15;
			if(sRet == 0) 
			{
				if(GetColumnStyle(j)!=ctr_Edit_No)
				{
					SetEditPos(FALSE,i,j,FALSE);
					Key_Shift(i,j);
					CListCtrl::GetSubItemRect(i,j,LVIR_LABEL,rcCtrl); 
					if(m_EditStyle==ctr_Edit_Click)
					OnLButtonDown(0,CPoint(rcCtrl.left+1,rcCtrl.top+1));
					else
					OnLButtonDblClk(0,CPoint(rcCtrl.left+1,rcCtrl.top+1));
				}
				return TRUE;	
			}
			else
			{
				if(GetColumnStyle(j)==ctr_Edit_CboDown)
					ctl_EditComboDown.ShowDropDown(TRUE);
				if(GetColumnStyle(j)==ctr_Edit_CboDownList)
					ctl_EditComboDownList.ShowDropDown(TRUE);
				return TRUE;
			}
		}
		if(pMsg->wParam == VK_LEFT)
		{
			short sRet = GetKeyState(VK_CONTROL);
			sRet = sRet >>15;
			if(sRet!= 0) 
			{
				switch(GetColumnStyle(j))
				{
				case ctr_Edit_No:
					break;
				case ctr_Edit_Text:
					ctl_EditText.SetSel(0,0);
					break;
				case ctr_Edit_Date:
					ctl_EditDateTime.SetSel(0,0);
					break;
				case ctr_Edit_DateTime:
					ctl_EditDateTime.SetSel(0,0);
					break;
				case ctr_Edit_Time:
					ctl_EditDateTime.SetSel(0,0);
					break;
				case ctr_Edit_CboDown:
					ctl_EditComboDown.SetEditSel(0,0);
					break;
//				case ctr_Edit_CboDownList:
//					ctl_EditComboDownList.SetEditSel(0,0);
// 					break;
				case ctr_Edit_Num:
					ctl_EditNum.SetSel(0,0);
					break;
				}
			return TRUE;
			}
		}
		if(pMsg->wParam == VK_RIGHT)
		{
			short sRet = GetKeyState(VK_CONTROL);
			sRet = sRet >>15;
			if(sRet!= 0) 
			{
				switch(GetColumnStyle(j))
				{
				case ctr_Edit_No:
					break;
				case ctr_Edit_Text:
					ctl_EditText.SetSel(-1);
					break;
				case ctr_Edit_Date:
					ctl_EditDateTime.SetSel(-1);
					break;
				case ctr_Edit_DateTime:
					ctl_EditDateTime.SetSel(-1);
					break;
				case ctr_Edit_Time:
					ctl_EditDateTime.SetSel(-1);
					break;
				case ctr_Edit_Num:
					ctl_EditNum.SetSel(-1);
					break;
				case ctr_Edit_CboDown:
					CString str;
					ctl_EditComboDown.GetWindowText(str);
					ctl_EditComboDown.SetEditSel(str.GetLength(),str.GetLength());
					break;
				}
			return TRUE;
			}
		}
		if(pMsg->wParam == VK_DOWN)
		{
			CString strText;
			short sRet = GetKeyState(VK_CONTROL);
			sRet = sRet >>15;
			if(sRet!= 0) 
			{
				switch(GetColumnStyle(j))
				{
				case ctr_Edit_No:
					break;
				case ctr_Edit_Text:
					ctl_EditText.GetWindowText(strText);
					ctl_EditText.SetSel(0,strText.GetLength());
					break;
				case ctr_Edit_Date:
					strText=ctl_EditDateTime.GetDetectText();
					ctl_EditDateTime.SetSel(0,strText.GetLength());
					break;
				case ctr_Edit_DateTime:
					strText=ctl_EditDateTime.GetDetectText();
					ctl_EditDateTime.SetSel(0,strText.GetLength());
					break;
				case ctr_Edit_Time:
					strText=ctl_EditDateTime.GetDetectText();
					ctl_EditDateTime.SetSel(0,strText.GetLength());
					break;
				case ctr_Edit_Num:
					strText=ctl_EditNum.GetText();
					ctl_EditNum.SetSel(0,strText.GetLength());
					break;
				case ctr_Edit_CboDown:
					if(ctl_EditComboDown.GetCount()>0)
					ctl_EditComboDown.SetCurSel(ctl_EditComboDown.GetCount()-1);
					break;
				case ctr_Edit_CboDownList:
					if(ctl_EditComboDownList.GetCount()>0)
					ctl_EditComboDownList.SetCurSel(ctl_EditComboDownList.GetCount()-1);
					break;
				default:
					break;
				}
			return TRUE;
			}
		}
		if(pMsg->wParam == VK_UP)
		{
			CString strText;
			short sRet = GetKeyState(VK_CONTROL);
			sRet = sRet >>15;
			if(sRet!= 0) 
			{
				switch(GetColumnStyle(j))
				{
				case ctr_Edit_No:
					break;
				case ctr_Edit_Text:
					ctl_EditText.GetWindowText(strText);
					ctl_EditText.SetSel(0,strText.GetLength());
					break;
				case ctr_Edit_Date:
					strText=ctl_EditDateTime.GetDetectText();
					ctl_EditDateTime.SetSel(0,strText.GetLength());
					break;
				case ctr_Edit_DateTime:
					strText=ctl_EditDateTime.GetDetectText();
					ctl_EditDateTime.SetSel(0,strText.GetLength());
					break;
				case ctr_Edit_Time:
					strText=ctl_EditDateTime.GetDetectText();
					ctl_EditDateTime.SetSel(0,strText.GetLength());
					break;
				case ctr_Edit_Num:
					strText=ctl_EditNum.GetText();
					ctl_EditNum.SetSel(0,strText.GetLength());
					break;
				case ctr_Edit_CboDown:
					if(ctl_EditComboDown.GetCount()>0)
						ctl_EditComboDown.SetCurSel(0);
					break;
				case ctr_Edit_CboDownList:
					if(ctl_EditComboDownList.GetCount()>0)
						ctl_EditComboDownList.SetCurSel(0);
					break;
				default:
					break;
				}
			return TRUE;
			}
		}
	}
	
	return CListCtrl::PreTranslateMessage(pMsg);
}
void CMyListCtrl::OnParentNotify(UINT message, LPARAM lParam) 
{
	CListCtrl::OnParentNotify(message, lParam);
	//////////////////////////////////////////////////////////////////////////
	CHeaderCtrl* pHeaderCtrl = CListCtrl::GetHeaderCtrl();
	if(pHeaderCtrl == NULL)
		return;
	CRect rcHeader;
	pHeaderCtrl->GetWindowRect(rcHeader);
	ScreenToClient(rcHeader);
	//The x coordinate is in the low-order word and the y coordinate is in the high-order word.
	CPoint pt;
	pt.x = (int)LOWORD(lParam);
	pt.y = (int)HIWORD(lParam);
	if(rcHeader.PtInRect(pt) && message == WM_LBUTTONDOWN)
	{
			ctl_EditComboDown.ShowWindow(SW_HIDE);
			ctl_EditDateTime.ShowWindow(SW_HIDE);
			ctl_EditText.ShowWindow(SW_HIDE);
			ctl_EditNum.ShowWindow(SW_HIDE);
	}	
}
BOOL CMyListCtrl::Key_Shift(int& nItem,int& nSub)
{
	int nItemCount = CListCtrl::GetItemCount();
	CHeaderCtrl* pHeader = CListCtrl::GetHeaderCtrl();
	if(pHeader == NULL)
		return FALSE;
	short sRet = GetKeyState(VK_SHIFT);
	int nSubcCount = pHeader->GetItemCount();
	sRet = sRet >>15;
	if(sRet == 0)
	{
		nSub += 1;
		if(nSub >= nSubcCount)
		{
			if(nItem == nItemCount-1)
			{
				nItem = 0;
				nSub  = 0;
			}
			else
			{
				nSub = 0;
				nItem += 1;
			}
		}
		if(nItem >= nItemCount)
			nItem = nItemCount-1;
		return FALSE;
	}
	else
	{
		nSub -= 1;
		if(nSub < 0)
		{
			
			nSub = nSubcCount -1;
			nItem --;
		}
		
		if(nItem < 0)
			nItem = nItemCount-1;
		return TRUE;
		
	}
	return FALSE;
}
BOOL CMyListCtrl::Key_Ctrl(int& nItem,int &nSub)
{
    short sRet = GetKeyState(VK_CONTROL);
	sRet = sRet >>15;
	int nItemCount = CListCtrl::GetItemCount();
	if(sRet == 0)
	{
		
	}
	else
	{
		nItem = nItem >=nItemCount-1? 0:nItem+=1;
		return TRUE;
	}
	
	return FALSE;
}
void CMyListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SetEditPos(FALSE,itemX,itemY,FALSE);
	CListCtrl::OnHScroll(nSBCode,nPos,pScrollBar);
}
void CMyListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SetEditPos(FALSE,itemX,itemY,FALSE);
	CListCtrl::OnVScroll(nSBCode,nPos,pScrollBar);
}
void CMyListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    if (!lpDrawItemStruct) return;
	CDC*		pDC			= CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect		rcItem;
	 	CFont ft;
	ft.CreateFont(10,10,0,0,
	FW_NORMAL,FALSE,FALSE,FALSE,
	ANSI_CHARSET,
	OUT_DEFAULT_PRECIS,
	CLIP_DEFAULT_PRECIS,
	DEFAULT_QUALITY,
	DEFAULT_PITCH|FF_MODERN,
	"MS Sans Serif");
	GetSubItemRect(lpDrawItemStruct->itemID,0,LVIR_LABEL,rcItem);
	CString str;
	str.Format("%d-----%d",lpDrawItemStruct->itemID,0);
	pDC->SelectObject(&ft);
	pDC->SetTextColor(RGB(255,0,0));
	pDC->TextOut(rcItem.left,rcItem.top,str);
}