#include "StdAfx.h"
#include "ADOConn.h"
//功能:构造函数
//strData:数据库的名字
//download by www.codefans.net
CString CADOConn::GetAppPath()
{
	char lpFileName[MAX_PATH];
	GetModuleFileName(AfxGetInstanceHandle(),lpFileName,MAX_PATH);

	CString strFileName = lpFileName;
	int nIndex = strFileName.ReverseFind ('\\');
	
	CString strPath;

	if (nIndex > 0)
		strPath = strFileName.Left (nIndex);
	else
		strPath = "";
	return strPath;
}
CADOConn::CADOConn(CString strData)
{
	CString strValue;
	strData.TrimLeft();
	strData.TrimRight();
	m_strData=_bstr_t(strData);
	strValue=strData.Right(3);
	if(strValue=="mdb") m_DataType=1;
	if(strValue=="xls") m_DataType=2;
}
CADOConn::CADOConn()
{
	m_DataType=3;
}
//功能:析构函数
CADOConn::~CADOConn()
{

}
//功能:初始化数据库连接
void  CADOConn::OnInitCADOConn()
{
	// 初始化OLE/COM库环境
	::CoInitialize(NULL);
 	try
	{
		//初始化指针
		m_pConnection=NULL;
		//初始化指针
		m_pRecordset=NULL;
		// 创建Connection对象
		m_pConnection.CreateInstance("ADODB.Connection");
		// 设置连接字符串，必须是BSTR型或者_bstr_t类型	
		_bstr_t strConnect;
		switch(m_DataType)
		{
		case 1://ACCESS
			strConnect=_bstr_t("Provider=Microsoft.Jet.OLEDB.4.0;");
			strConnect=strConnect+_bstr_t("Data Source=");
			strConnect=strConnect+_bstr_t(GetAppPath())+_bstr_t("\\");
			strConnect=strConnect+m_strData;
			break;
		case 2://EXCEL
			strConnect=_bstr_t("Provider=Microsoft.Jet.OLEDB.4.0;");
			strConnect=strConnect+_bstr_t("Data Source=");
			strConnect=strConnect+_bstr_t(GetAppPath())+_bstr_t("\\");
			strConnect=strConnect+m_strData;
			strConnect=strConnect+";Extended Properties=Excel 8.0";		
			break;			
		case 3://SQLSERVER
			strConnect=_bstr_t("Provider=Microsoft.Jet.OLEDB.4.0;");
			strConnect=strConnect+_bstr_t("Data Source=D:\\FECG\\");
			strConnect=strConnect+";Extended Properties=Excel 8.0";					
			break;
		}
		m_pConnection->Open(strConnect,_bstr_t(""),_bstr_t(""),adModeUnknown);
	}
	// 捕捉异常
	catch(_com_error e)
	{	
		// 显示错误信息
		AfxMessageBox(e.Description());
	}
	ASSERT(m_pConnection != NULL);
}
//功能:执行查询
//返回值:字段集的指针
_RecordsetPtr&  CADOConn::GetRecordSet(CString strSQL)
{

	try
	{
		// 连接数据库，如果Connection对象为空，则重新连接数据库
		if(m_pConnection==NULL)OnInitCADOConn();	
		strSQL.TrimLeft();
		strSQL.TrimRight();
		// 创建记录集对象
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		// 取得表中的记录
		m_pRecordset->Open(_bstr_t(strSQL),m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
	}
	// 捕捉异常
	catch(_com_error e)
	{
		// 显示错误信息
		AfxMessageBox(e.Description());
	}
	ASSERT(m_pRecordset!= NULL);
	// 返回记录集
	return m_pRecordset;
}
//功能:打开字符集
//返回值:TRUE:成功 FALSE:失败
BOOL CADOConn::Open(CString strSQL)
{
	try
	{
		// 连接数据库，如果Connection对象为空，则重新连接数据库
		if(m_pConnection==NULL) OnInitCADOConn();		
		strSQL.TrimLeft();
		strSQL.TrimRight();
		// 创建记录集对象
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		// 取得表中的记录
		m_pRecordset->Open(_bstr_t(strSQL),m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
	}
	// 捕捉异常
	catch(_com_error e)
	{
		// 显示错误信息
		AfxMessageBox(e.Description());
		return FALSE;
	}
	// 返回记录集
	ASSERT(m_pRecordset!= NULL);
	return TRUE;	
}
//功能:记录集移向开头
//返回值:TRUE:成功 FALSE:失败
BOOL CADOConn::MoveFirst()
{
	if(m_pRecordset==NULL) 
		return FALSE;
	else
	{
		m_pRecordset->MoveFirst();
		return TRUE;
	}

}
//功能:记录集向下移动
//返回值:TRUE:成功 FALSE:失败
BOOL CADOConn::MoveNext()
{
	if(m_pRecordset==NULL) 
		return FALSE;
	else
	{
		if (!m_pRecordset->adoEOF)
		{
			m_pRecordset->MoveNext();
			return TRUE;
		}
		else
			return FALSE;		
	}

}
//功能:取得字段中的字符串
//index:字段集中的索引
//strSum:返回的字符的长度(<=实际:代表全部返回,>实际:左补空格)
CString CADOConn::GetValueString(int index,int strSum)
{
	_variant_t vValue;//var型返回值
	_variant_t vIndex;//索引
	CString strValue,strType,str;//strValue:初始返回值 strType:格式化字符串 str:最终返回值
	vIndex.vt=VT_I2;
	vIndex.iVal=index;
	vValue=m_pRecordset->Fields->GetItem(vIndex)->Value;
	switch(vValue.vt)
	{
	case VT_NULL:
		str="";
		break;
	case VT_ERROR:
		str="";
		break;	
	case VT_EMPTY:
		str="";
		break;
	default:
		str=(LPCSTR)_bstr_t(vValue);
	}
	strType.Format("%d",strSum);
	strType="%"+strType+"s";
	strValue.Format(strType,str);
	return strValue;
	
}
//功能:取得字段中的字节型整数
//index:字段集中的索引
byte CADOConn::GetValueByte(int index)
{
	_variant_t vValue;//var型返回值
	_variant_t vIndex;//索引
	byte bValue;//数值返回值
	vIndex.vt=VT_I2;
	vIndex.iVal=index;
	vValue=m_pRecordset->Fields->GetItem(vIndex)->Value;
	switch(vValue.vt)
	{
	case VT_NULL:
		bValue=0;
		break;
	case VT_ERROR:
		bValue=0;
		break;
	case VT_EMPTY:
		bValue=0;
		break;
	default:
		bValue=vValue.bVal;
	}
	return bValue;
}
//功能:取得字段中的字节型整数的字符串形式
//index:字段集中的索引
//strSum:返回的字符的长度(<=实际:代表全部返回,>实际:左补空格)
CString CADOConn::GetValueByteStr (int index,int strSum)
{
	_variant_t vValue;//var型返回值
	_variant_t vIndex;//索引
	byte bValue;//数值返回值
	CString strValue,strType;//strValue:字符串返回值 strType:格式化字符串
	vIndex.vt=VT_I2;
	vIndex.iVal=index;
	vValue=m_pRecordset->Fields->GetItem(vIndex)->Value;
	switch(vValue.vt)
	{
	case VT_NULL:
		bValue=0;
		break;
	case VT_ERROR:
		bValue=0;
		break;
	case VT_EMPTY:
		bValue=0;
		break;
	default:
		bValue=vValue.bVal;
	}
	strType.Format("%d",strSum);
	strType="%"+strType+"d";
	strValue.Format(strType,bValue);
	return strValue;
}
//功能:取得字段中的短整型整数
//index:字段集中的索引
int CADOConn::GetValueInt(int index)
{
	_variant_t vValue;//var型返回值
	_variant_t vIndex;//索引
	int iValue;//数值返回值
	vIndex.vt=VT_I2;
	vIndex.iVal=index;
	vValue=m_pRecordset->Fields->GetItem(vIndex)->Value;
	switch(vValue.vt)
	{
	case VT_NULL:
		iValue=0;
		break;
	case VT_ERROR:
		iValue=0;
		break;
	case VT_EMPTY:
		iValue=0;
		break;
	default:
		iValue=vValue.iVal;
	}
	return iValue;
}
//功能:取得字段中的短整型整数的字符串形式
//index:字段集中的索引
//strSum:返回的字符的长度(<=实际:代表全部返回,>实际:左补空格)
CString CADOConn::GetValueIntStr(int index,int strSum)
{
	_variant_t vValue;//var型返回值
	_variant_t vIndex;//索引
	int iValue;//数值返回值
	CString strValue,strType;//strValue:字符串返回值 strType:格式化字符串
	vIndex.vt=VT_I2;
	vIndex.iVal=index;
	vValue=m_pRecordset->Fields->GetItem(vIndex)->Value;
	switch(vValue.vt)
	{
	case VT_NULL:
		iValue=0;
		break;
	case VT_ERROR:
		iValue=0;
		break;
	case VT_EMPTY:
		iValue=0;
		break;
	default:
		iValue=vValue.iVal;
	}
	strType.Format("%d",strSum);
	strType="%"+strType+"d";
	strValue.Format(strType,iValue);
	return strValue;	
}
//功能:取得字段中的双精度浮点数
//index:字段集中的索引
double CADOConn::GetValueDouble(int index)
{
	_variant_t vValue;//var型返回值
	_variant_t vIndex;//索引
	double dValue;//数值返回值
	vIndex.vt=VT_I2;
	vIndex.iVal=index;
	vValue=m_pRecordset->Fields->GetItem(vIndex)->Value;
	switch(vValue.vt)
	{
	case VT_NULL:
		dValue=0;
		break;
	case VT_ERROR:
		dValue=0;
		break;
	case VT_EMPTY:
		dValue=0;
		break;
	default:
		dValue=vValue.dblVal;
	}
	return dValue;
}
//功能:取得字段中的双精度浮点数的字符串形式
//index:字段集中的索引
//strLSum(整数部分):返回的整数部分的长度(<=实际:代表全部返回,>实际:左补空格)
//strRSum(小数部分):返回的小数部分的长度(<=实际:代表全部返回,>实际:右补空格)
CString CADOConn::GetValueDoubleStr(int index,int strLSum,int strRSum)
{
	_variant_t vValue;//var型返回值
	_variant_t vIndex;//索引
	double dValue;//数值返回值
	CString strValue,strType;//strValue:字符串返回值 strType:格式化字符串
	vIndex.vt=VT_I2;
	vIndex.iVal=index;
	vValue=m_pRecordset->Fields->GetItem(vIndex)->Value;
	switch(vValue.vt)
	{
	case VT_NULL:
		dValue=0;
		break;
	case VT_ERROR:
		dValue=0;
		break;
	case VT_EMPTY:
		dValue=0;
		break;
	default:
		dValue=vValue.dblVal;
	}
	strType.Format("%d.%d",strLSum,strRSum);
	strType="%"+strType+"f";
	strValue.Format(strType,dValue);
	return strValue;
}
//功能:取得字段中的单精度浮点数
//index:字段集中的索引
float CADOConn::GetValueFloat(int index)
{
	_variant_t vValue;//var型返回值
	_variant_t vIndex;//索引
	float fValue;//数值返回值
	vIndex.vt=VT_I2;
	vIndex.iVal=index;
	vValue=m_pRecordset->Fields->GetItem(vIndex)->Value;
	switch(vValue.vt)
	{
	case VT_NULL:
		fValue=0;
		break;
	case VT_ERROR:
		fValue=0;
		break;
	case VT_EMPTY:
		fValue=0;
		break;
	default:
		fValue=vValue.fltVal;
	}
	return fValue;
}
//功能:取得字段中的单精度浮点数的字符串形式
//index:字段集中的索引
//strLSum(整数部分):返回的整数部分的长度(<=实际:代表全部返回,>实际:左补空格)
//strRSum(小数部分):返回的小数部分的长度(<=实际:代表全部返回,>实际:右补空格)
CString CADOConn::GetValueFloatStr(int index,int strLSum,int strRSum)
{
	_variant_t vValue;//var型返回值
	_variant_t vIndex;//索引
	float fValue;//数值返回值
	CString strValue,strType;//strValue:字符串返回值 strType:格式化字符串
	vIndex.vt=VT_I2;
	vIndex.iVal=index;
	vValue=m_pRecordset->Fields->GetItem(vIndex)->Value;
	switch(vValue.vt)
	{
	case VT_NULL:
		fValue=0;
		break;
	case VT_ERROR:
		fValue=0;
		break;
	case VT_EMPTY:
		fValue=0;
		break;
	default:
		fValue=vValue.fltVal;
	}
	strType.Format("%d.%d",strLSum,strRSum);
	strType="%"+strType+"f";
	strValue.Format(strType,fValue);
	return strValue;
}
//功能:取得字段中的长整型整数的字符串
//index:字段集中的索引
long CADOConn::GetValueLong(int index)
{
	_variant_t vValue;//var型返回值
	_variant_t vIndex;//索引
	long lValue;//数值返回值
	vIndex.vt=VT_I2;
	vIndex.iVal=index;
	vValue=m_pRecordset->Fields->GetItem(vIndex)->Value;
	switch(vValue.vt)
	{
	case VT_NULL:
		lValue=0;
		break;
	case VT_ERROR:
		lValue=0;
		break;
	case VT_EMPTY:
		lValue=0;
		break;
	default:
		lValue=vValue.lVal;
	}
	return lValue;
}
//功能:取得字段中的长整型整数的字符串形式
//index:字段集中的索引
//strSum:返回的字符的长度(<=实际:代表全部返回,>实际:左补空格)
CString CADOConn::GetValueLongStr(int index,int strSum)
{
	_variant_t vValue;//var型返回值
	_variant_t vIndex;//索引
	long lValue;//数值返回值
	CString strValue,strType;//strValue:字符串返回值 strType:格式化字符串
	vIndex.vt=VT_I2;
	vIndex.iVal=index;
	vValue=m_pRecordset->Fields->GetItem(vIndex)->Value;
	switch(vValue.vt)
	{
	case VT_NULL:
		lValue=0;
		break;
	case VT_ERROR:
		lValue=0;
		break;
	case VT_EMPTY:
		lValue=0;
		break;
	default:
		lValue=vValue.lVal;
	}
	strType.Format("%d",strSum);
	strType="%"+strType+"d";
	strValue.Format(strType,lValue);
	return strValue;	
}
//返回时间型值
//数据库中存的格式为字符串(yyyy-mm-dd HH-MM-SS)
CTime CADOConn::GetValueDate(int index)
{
	_variant_t vValue;//var型返回值
	_variant_t vIndex;//索引
	CString strValue;//数值返回值
	vIndex.vt=VT_I2;
	vIndex.iVal=index;
	vValue=m_pRecordset->Fields->GetItem(vIndex)->Value;
	switch(vValue.vt)
	{
	case VT_NULL:
		strValue="1980-08-08";
		break;
	case VT_ERROR:
		strValue="1980-08-08";
		break;
	case VT_EMPTY:
		strValue="1980-08-08";
		break;
	default:
		strValue=(LPCSTR)_bstr_t(vValue);
		break;
	}
	CString strYear,strMonth,strDay,strHour,strMin,strSec;
	strYear=strValue.Mid(0,4);
	strMonth=strValue.Mid(5,2);
	strDay=strValue.Mid(8,2);
	if(strValue.GetLength()>10)
	{
		strHour=strValue.Mid(11,2);
		strMin=strValue.Mid(14,2);
		strSec=strValue.Mid(17,2);
	}
	else
	{
		strHour="0";
		strMin="0";
		strSec="0";
	}
	CTime TValue(atoi(strYear),atoi(strMonth),atoi(strDay),atoi(strHour),atoi(strMin),atoi(strSec));
	return TValue;
}
//返回时间型值的字符串
//数据库中存的格式为字符串(yyyy-mm-dd HH-MM-SS)
CString CADOConn::GetValueDateStr(int index,CString strType)
{
	_variant_t vValue;//var型返回值
	_variant_t vIndex;//索引
	CString strValue="";//数值返回值
	vIndex.vt=VT_I2;
	vIndex.iVal=index;
	vValue=m_pRecordset->Fields->GetItem(vIndex)->Value;
	switch(vValue.vt)
	{
	case VT_NULL:
		strValue="1980-08-08";
		break;
	case VT_ERROR:
		strValue="1980-08-08";
		break;
	case VT_EMPTY:
		strValue="1980-08-08";
		break;
	default:
		strValue=(LPCSTR)_bstr_t(vValue);
		break;
	}
	CString strYear,strMonth,strDay,strHour,strMin,strSec;
	strYear=strValue.Mid(0,4);
	strMonth=strValue.Mid(5,2);
	strDay=strValue.Mid(8,2);
	if(strValue.GetLength()>10)
	{
		strHour=strValue.Mid(11,2);
		strMin=strValue.Mid(14,2);
		strSec=strValue.Mid(17,2);
	}
	else
	{
		strHour="0";
		strMin="0";
		strSec="0";
	}
	CTime TValue(atoi(strYear),atoi(strMonth),atoi(strDay),atoi(strHour),atoi(strMin),atoi(strSec));
	CString str=TValue.Format(strType);
	return str;	
}
//功能:执行SQL语句(Insert Update delete)
//strSQL:SQL语句
//返回值:TRUE:成功 FALSE:失败
BOOL CADOConn::ExecuteSQL(CString strSQL)
{
	try
	{
		// 是否已经连接数据库
		if(m_pConnection == NULL) OnInitCADOConn();
		strSQL.TrimLeft();
		strSQL.TrimRight();
		m_pConnection->Execute(_bstr_t(strSQL),NULL,adCmdText);
		return TRUE;
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		return FALSE;
	}
}
//功能:退出连接
void CADOConn::ExitConnect()
{
	try
	{
	if (m_pRecordset !=NULL) m_pRecordset->Close();
	if (m_pConnection !=NULL) m_pConnection->Close();
	// 释放环境
	::CoUninitialize();
	}
	catch (_com_error e) 
	{
		AfxMessageBox(e.Description());
	}
}
//功能:向一表中添加一行数据
//strTable:表名
//strSum:表中字段集的数目
//pszText...:strSum个数据字符串
//返回值:TRUE:成功 FALSE:错误
BOOL CADOConn::AddItem(CString strTable,int strSum,LPCTSTR pszText, ... )
{
	strTable.TrimLeft();
	strTable.TrimRight();
	CString strSQL="select * from "+strTable;
	CString strFirst=pszText;
	LPTSTR* arrpsz = new LPTSTR[strSum];//初始化一列表存放数据
	arrpsz[ 0 ] = new TCHAR[ lstrlen( pszText ) + 1 ];
	(void)lstrcpy( arrpsz[ 0 ], pszText );
 	va_list list;
	va_start( list, pszText );//向列表填充数据
	Open(strSQL);//字符集类型的查找
	strSQL="insert into "+strTable+" values(";
	int iType;
	iType=GetValueType(0);
	if (iType==ado_Field_Str||iType==ado_Field_Text||iType==ado_Field_Date)
		strSQL=strSQL+"'"+strFirst+"',";//字符型
	else
	{
		if(strFirst=="") strFirst="0";
		strSQL=strSQL+strFirst+",";//数字型
	}
	for( int iColumn = 1; iColumn <strSum; iColumn++ )
	{
		pszText = va_arg( list, LPCTSTR );
		ASSERT_VALID_STRING( pszText );
		iType=GetValueType(iColumn);
		if (iType==ado_Field_Str||iType==ado_Field_Text||iType==ado_Field_Date)
			strSQL=strSQL+"'"+pszText+"',";
		else
		{
			CString strValue=pszText;
			if(strValue=="") strValue="0";
			strSQL=strSQL+strValue+",";
		}
		arrpsz[ iColumn ] = new TCHAR[ lstrlen( pszText ) + 1 ];
		(void)lstrcpy( arrpsz[ iColumn ], pszText );
	}

	va_end( list );
	//开始执行
	strSQL=strSQL.Left(strSQL.GetLength()-1)+")";
	try
	{
		//执行插入操作
		m_pConnection->Execute(_bstr_t(strSQL),NULL,adCmdText);
		return TRUE;
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		return FALSE;
	}
}
//功能:返回索引为Index时的数据的类型
//返回值:数据类型
int CADOConn::GetValueType(int index)
{
	_variant_t vIndex;
	int Tpye;
	vIndex.vt=VT_I2;
	vIndex.iVal=index;
	Tpye=m_pRecordset->Fields->GetItem(vIndex)->GetType();
	return Tpye;	
}
//功能:判断字段集是否结束
//返回值:TRUE:结束 FALSE:未结束
BOOL CADOConn::adoEOF()
{
	if(m_pRecordset->adoEOF)
		return TRUE;
	else
		return FALSE;
}
//功能:填充列表
//listMain:列表指针 ColOpenEnd:代表展开多少列
//返回值:TRUE:成功　FALSE:失败
BOOL CADOConn::FillList(CListCtrl *listMain,int ColOpenEnd)
{
	int i,iType,iRow=0,listWidth=0;//iType:字段集的数据类型 listWidth:列表中列的宽度
	long lMax=0;
	_variant_t vIndex;
	lMax=m_pRecordset->Fields->Count;
	vIndex.vt=VT_I2;
	listMain->SetExtendedStyle( LVS_EX_FULLROWSELECT);
	listMain->DeleteAllItems();
	for(i=0;i<100;i++)
		listMain->DeleteColumn(i);
	for(i=0;i<lMax;i++)
	{	
		CString strTitle="";
		vIndex.iVal=i;
		strTitle=(LPCTSTR)m_pRecordset->Fields->GetItem(vIndex)->GetName();
		listMain->InsertColumn(i,strTitle,LVCFMT_CENTER,100,0);
	}
	if(!m_pRecordset->adoEOF)
	{	
		MoveFirst();
		while (!m_pRecordset->adoEOF)
		{
			for (i=0;i<lMax;i++)
			{
				CString strValue="";
				vIndex.iVal=i;
				iType=m_pRecordset->Fields->GetItem(vIndex)->GetType();
				switch(iType)
				{
				case ado_Field_Str:
				case ado_Field_Text:
					strValue=GetValueString(i,0);
					break;
				case ado_Field_Long:
					strValue=GetValueLongStr(i,0);
					break;
				case ado_Field_Int:
					strValue=GetValueIntStr(i,0);
					break;
				case ado_Field_Float:
					strValue=GetValueFloatStr(i,0,2);
					break;
				case ado_Field_Double:
					strValue=GetValueDoubleStr(i,0,2);
					break;
				case ado_Field_Byte:
					strValue=GetValueByteStr(i,0);
					break;
				case ado_Field_Date:
					strValue=GetValueString(i,0);
					break;
				default:
					strValue="";
					break;
				}
				if(m_DataType==2)
					strValue=GetValueString(i,0);
				if(i==0)
					listMain->InsertItem(iRow,strValue,0);
				else
					listMain->SetItemText(iRow,i,strValue);
			}
			m_pRecordset->MoveNext();
			iRow=iRow+1;
		}
		//移向开头
		MoveFirst();
	}
	if(listMain->GetItemCount()>0)
	{			
		if(ColOpenEnd>0)
		{
			for(int i=0;i<ColOpenEnd;i++)
			{
				listMain->SetColumnWidth(i,LVSCW_AUTOSIZE);
				listWidth=listMain->GetColumnWidth(i);
				listMain->SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);
				if(listWidth<listMain->GetColumnWidth(i))
					listMain->SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);
			}	
		}
		else
		{
			listMain->SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);
		}

	}
	else
	{
		for(i=0;i<lMax;i++)
		{
			listMain->SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);
			listWidth=listWidth+listMain->GetColumnWidth(i);
		}
		RECT rectList;
		listMain->GetWindowRect(&rectList);
		if(listWidth<(rectList.right-rectList.left))
		{
			listWidth=(rectList.right-rectList.left-listWidth)/11;
			listMain->SetColumnWidth(i,listMain->GetColumnWidth(i)+listWidth);
		}
	}
	return TRUE;
}
//功能:初始化自动排列列表
//CListCtrl:列表指针;colSum:列表中已经存在列数
BOOL CADOConn::InitList(CListCtrl *listMain,int colSum)
{
	int intWidth=0;
	int i;
	for(i=0;i<colSum;i++)
	{
		listMain->SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);
		intWidth=intWidth+listMain->GetColumnWidth(i);
	}
	RECT rectList;
	listMain->GetWindowRect(&rectList);
	if(intWidth<(rectList.right-rectList.left))
	{
		intWidth=(rectList.right-rectList.left-intWidth)/colSum;
		listMain->SetColumnWidth(i,listMain->GetColumnWidth(i)+intWidth);
	}
	return TRUE;
}
