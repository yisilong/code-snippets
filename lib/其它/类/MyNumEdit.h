//
//时间和日期型的掩码型的输入
//-------------------------------------------------------设计人：牛文平
#ifndef _MYNUMDEFINE_
#define _MYNUMDEFINE_
//定义三种形式的数据类型
class CMyNumEdit : public CEdit
{
	DECLARE_DYNAMIC(CMyNumEdit)
public:
	CMyNumEdit();
public:
	CString m_strText;
	int m_intLeft;
	int m_intRight;
	int m_Point;
	BOOL blnAuto;
	//掩码
public:
	//设置类型
	//intRight为小数点位数
	void SetNumType(int intLeft,int intRight);
	//返回字符串
	CString GetText();
	//设置字符型数据
	void SetText(CString strText);
	//析构函数
	virtual ~CMyNumEdit();
	//检测日期型的字符
	BOOL CheckInt(char nchar,int StartPos,int StartEnd);
	//检测时间性的字符
	BOOL CheckFloat(char nchar,int StartPos,int StratEnd);
protected:
	//{{AFX_MSG(CMaskEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LONG OnCut(UINT, LONG);
	afx_msg LONG OnClear(UINT wParam, LONG lParam);
	afx_msg LONG OnPaste(UINT, LONG);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif
