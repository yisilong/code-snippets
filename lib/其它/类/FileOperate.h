//使用说明:
//1.在整个程序中如果使用了CFileDialog时将Flags设置为OFN_NOCHANGEDIR
//2.此类的书写并不完善，有待改进
//---------------------------------------------------------设计人:牛文平
extern int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
extern CString strTmpPath;
class CFileOperate
{
public:
	//构造函数
	CFileOperate();
	//取得当前程序运行的路径
	CString GetAppPath();
	//判断是否存在strFn文件夹
	BOOL IsFileExist(CString strFn, BOOL bDir);
	//制作strFloderName文件夹返回文件夹名
	CString MakeDirectory(CString strFloderName);
	//自动生成文件夹
	CString MakeDirectory();
	//得到文件夹名为strFloderName的路径
	CString GetDirectoryPath(CString strFloderName);
	//删除strFloderdName文件夹
	void delDirectory(CString strFloderdName);
	//删除主文件夹(data)
	void delMainDirectory();
	//制作主文件夹
	void MakeMainDirectory();
	//判断strIntDigit是否为整数，是否小于intBig
	BOOL CheckIntDigit(CString strIntDigit,int intBig);
	//判断strFileName是否可以做文件夹或文件的名字
	BOOL CheckFileName(CString strFileName);
	//判断strText是否为空
	BOOL CheckEmpty(CString strText);
	//删除strFloderName文件夹下名为strFileName的文件
	void delFile(CString strFloderName,CString strFileName);
	//得到strFloderName文件夹下名为strFileName的文件路径
	CString GetFileName(CString strFolderName,CString strFileName);
	//制作strFloderName文件夹下名为strFileName的文件
	CString MakeFile(CString strFloderName,BOOL blnMake);
	//显示文件夹对话框
	BOOL GetFolder(CString* strSelectedFolder,const char* lpszTitle,const HWND hwndOwner,const char* strRootFolder,const char* strStartFolder);

};