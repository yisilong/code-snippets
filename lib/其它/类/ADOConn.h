//使用说明:
//1.构造函数:必须初始化数据库名字,数据库的默认路径为此运行程序的所在路径
//2.结束:必须使用ExitConnect退出
//3.插入、删除、更新:
//4.查询:使用open打开字符集方可使用Get...的函数
//5.填充列表:使用open打开字符集,然后填充
//---------------------------------------------------------设计人:牛文平
#import "c:\program files\common files\system\ado\msado15.dll" \
	no_namespace \
	rename ("EOF", "adoEOF")
//返回类型的宏定义
#define ado_Field_Str 202
#define ado_Field_Byte 17
#define ado_Field_Int 2
#define ado_Field_Long 3
#define ado_Field_Float 4 
#define ado_Field_Double 5  
#define ado_Field_Text 203 
#define ado_Field_Date 7
#ifdef _DEBUG
	#define ASSERT_VALID_STRING( str ) ASSERT( !IsBadStringPtr( str, 0xfffff ) )
#else	//	_DEBUG
	#define ASSERT_VALID_STRING( str ) ( (void)0 )
#endif	//	_DEBUG
class CADOConn  
{
// 定义变量
public:
	//添加一个指向Connection对象的指针:
	_ConnectionPtr m_pConnection;
	//添加一个指向Recordset对象的指针:
	_RecordsetPtr m_pRecordset;
	_bstr_t m_strData;
	int m_DataType;
	CADOConn(CString strData);
	CADOConn();
	virtual ~CADOConn();
	//初始化—连接数据库
	void  OnInitCADOConn();
	//执行查询
	_RecordsetPtr& GetRecordSet(CString strSQL);
	//执行SQL语句，Insert Update delete
	BOOL ExecuteSQL(CString strSQL);
	//退出连接
	void ExitConnect();
	//字段集移向开头
	BOOL MoveFirst();
	//字段集向下移
	BOOL MoveNext();
	//打开字段集
	BOOL Open(CString strSQL);
	//字符串类型(所有类型均可通过此返回此类型的字符串)
	CString GetValueString(int index,int strSum);//字符
	//字节型整数BYTE
	byte GetValueByte(int index);
	//短整型数INT
	int GetValueInt(int index);
	//双精度数DOUBLE
	double GetValueDouble(int index);
	//单精度数FLOAT
	float GetValueFloat(int index);
	//长整型数LONG
	long GetValueLong(int index);
	//时间型CTime
	CTime GetValueDate(int index);
	//字节型整数BYTE数值字符
	CString GetValueByteStr(int index,int strSum);
	//短整型数INT数值字符
	CString GetValueIntStr(int index,int strSum);
	//双精度数DOUBLE数值字符
	CString GetValueDoubleStr(int index,int strLSum,int strRSum);
	//单精度数FLOAT数值字符
	CString GetValueFloatStr(int index,int strLSum,int strRSum);
	//长整型数LONG数值字符
	CString GetValueLongStr(int index,int strSum);
	//时间型CTime数值字符
	CString GetValueDateStr(int index,CString strType);
	//添加单项数据
	BOOL AddItem(CString strTable,int strSum,LPCTSTR pszText, ... );
	//得到字段中数据的类型
	int GetValueType(int index);
	//字段集的结束判断
	BOOL adoEOF();
	//填充列表(ColOpenEnd代表展开多少列)
	BOOL FillList(CListCtrl *listMain,int ColOpenEnd);
	//初始化列表
	BOOL InitList(CListCtrl *listMain,int colSum);
	//得到应用程序所在的文件夹
	CString GetAppPath();
};
