//实现汉字向拼音的转化
//-----------------------------------------------------设计人:牛文平
class CMyChiToLetter
{
public:
	CMyChiToLetter();
	~CMyChiToLetter();
	//TRUE:得到首字母大写
	//FALSE:得到首字母小写
	BOOL m_blnFirstBig;
	//TRUE:得到全部大写
	//FALSE:得到除去首拼音的所有小写
	BOOL m_blnAllBiG;
	//True:得到全部拼音
	//FALSE:得到首拼音
	BOOL m_blnSimaple;
	CString GetLetter(CString strText);
private:
	CString FindLetter(int nCode);
};