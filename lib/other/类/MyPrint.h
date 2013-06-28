//功能:实现简单的打印
//---------------------------------------------------设计人:牛文平
class CMyPrint
{
public:
	//构造函数
	CMyPrint();
	//返回指针
	CDC* GetMyCdc();
	//人为的选择打印机
	//可以在注册表HKEY_CURRENT_USER\Printers\中找到相关的数据
	CDC* GetManCdc(CString strDriver,CString strDevice, CString strOutput, CONST DEVMODE* lpInitData);
	//初始化打印机 	
	BOOL GetPrinterDeviceDefaults(void);
	//设置横向，竖向
	//TRUE--横向  FALSE--竖向
	BOOL SetPrintOrientation(BOOL bVal);
	//开始打印
	void DoPrint();
	//设备名
	HGLOBAL m_hDevNames;
	//设备设置字
	HGLOBAL	m_hDevMode;
	//打印页的边框空白(像素)
	CRect m_rcPrintMargin;
	//打印机返回的可写范围(像素)
	CRect rcContent;
	//设置打印文件的名字
	CString strDocName;
	//设置横向，竖向
	//TRUE--横向  FALSE--竖向
	BOOL blnVal;
	//CDC指针
	CDC *m_CDC;
	//宽(英寸)
	int nXPixelsOfInch;
	//高度(光栅线)
	int nYPixelsOfInch;	
	//得到像素向英寸转换的比例
	//像素:英寸=?:1
	double GetPrintProportionX();
	//由像素转换成所需的英寸
	int IntToRealIntX(int i);
	//得到像素向英寸转换的比例
	//像素:英寸=?:1
	double GetPrintProportionY();
	//由像素转换成所需的英寸
	int IntToRealIntY(int i);
	//释放所有内存
	void FreeAllMemory();
	//析构函数
	~CMyPrint ();
};