//
//时间和日期型的掩码型的输入
//-------------------------------------------------------设计人：牛文平
#ifndef _MYMASKDEFINE_
#define _MYMASKDEFINE_
//定义三种形式的数据类型
#define edit_date 1
#define edit_time 2
#define edit_datetime 3
#include<comdef.h>
class CMyDateEdit : public CEdit
{
	DECLARE_DYNAMIC(CMyDateEdit)
public:
	CMyDateEdit();
public:
	//是否使用掩码
	BOOL m_bUseMask;
	//是否是日期格式
	BOOL m_isDate;
	//是否是时间格式
	BOOL m_isTime;
	//是否是日期时间格式
	BOOL m_isDateTime; 
	//存储用户输入的字符串
	CString m_str;
	//掩码
	CString m_strMask;
	//表示某位是否可输入，“_”表示可输入
	CString m_strLiteral;
	//存储预设的字符串，当用户输入退格键的时候使用
	CString m_strMaskLiteral;
	BOOL m_bMaskKeyInProgress;
public:
	//缺省
	void SetDefault();
	//设置成当前时间
	void SetNow();
	//重新设置
	void Reset();
	//设置类型
	void SetClassType(int intStyle);
	//返回字符串
	CString GetText();
	//设置字符型数据
	void SetText(CString strText);
	//设置检测正确的时间
	void SetDetectText(CString strText);
	//得到检测正确的时间
	CString GetDetectText();
	//设置时间值，将自动进行转换成所需的字符串
	void SetTimeValue(COleDateTime tm);
	//模拟发送一字符进行操作
	void SendChar(UINT nChar);
	//检查字符合法性
	BOOL CheckChar(UINT nChar);
	//析构函数
	virtual ~CMyDateEdit();
	//粘贴屏蔽
    LONG OnPaste(UINT, LONG);
	//剪切屏蔽
    LONG OnCut(UINT, LONG);
	//清空屏蔽
    LONG OnClear(UINT, LONG);
	//检测日期型的字符
	BOOL CheckDate(char nchar,int StartPos,int StartEnd);
	//检测时间性的字符
	BOOL CheckTime(char nchar,int StartPos,int StratEnd);
protected:
	//{{AFX_MSG(CMaskEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif
