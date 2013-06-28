//功能:实现简单的时间格式的转换
//---------------------------------------------------------------设计人:牛文平
class CMyTime
{
public:
	//CTime类型的值
	CTime ValueTime;
	//返回字符串形式的日期时间
	//blnChinese:TRUE-"年月日时分秒"  FALSE-"-:"
	CString GetAllString(BOOL blnChinese);
	//返回字符串形式的日期
	//blnChinese:TRUE-"年月日"  FALSE-"-"
	CString GetDateString(BOOL blnChinese);
	//返回字符串形式的时间
	//blnChinese:TRUE-"时分秒"  FALSE-":"
	CString GetTimeString(BOOL blnChinese);
	//返回简单字符串形式的日期时间
	CString GetSimpleString();
	//返回星期"星期日"
	CString GetWeek();
	//设置字符串形式的值
	//blnSimple:TRUE-(%Y%m%d%H%M%S) FALSE-(年月日时分秒或-:)
	void SetAllString(CString Value,BOOL blnSimple);
	//设置成现在时间
	void SetNow();
	//构造函数
	CMyTime();
};
