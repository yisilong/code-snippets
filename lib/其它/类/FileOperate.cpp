#include "StdAfx.h"
#include "FileOperate.h"
//功能:构造函数制造主目录
//download by www.codefans.net
CString strTmpPath;
int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	TCHAR szDir[MAX_PATH];
	switch(uMsg)
	{
	case BFFM_INITIALIZED:
		if (lpData)
		{
			strcpy(szDir, strTmpPath.GetBuffer(strTmpPath.GetLength()));
			SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)szDir);
		}
		break;
	case BFFM_SELCHANGED: 
		{
			if (SHGetPathFromIDList((LPITEMIDLIST) lParam ,szDir))
			{
				SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)szDir);
			}
			break;
		}
	default:
		break;
	}
	
	return 0;
}
CFileOperate::CFileOperate()
{
	CString strPath=GetAppPath()+"\\data";
	if (!IsFileExist(strPath,TRUE))
	{
		CreateDirectory(strPath,NULL);
	}
}
//功能:取得当前程序运行的路径
CString CFileOperate::GetAppPath()
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
//功能:判断是否存在strFn文件夹
BOOL CFileOperate::IsFileExist(CString strFn, BOOL bDir)
{
    HANDLE h;
	LPWIN32_FIND_DATA pFD=new WIN32_FIND_DATA;
	BOOL bFound=FALSE;
	if(pFD)
	{
		h=FindFirstFile(strFn,pFD);
		bFound=(h!=INVALID_HANDLE_VALUE);
		if(bFound)
		{
			if(bDir)
				bFound= (pFD->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)!=NULL;
			FindClose(h);
		}
		delete pFD;
	}
	return bFound;
}
//功能:制作strFloderName文件夹返回文件夹名
CString CFileOperate::MakeDirectory(CString strFloderName)
{
	strFloderName.TrimLeft();
	strFloderName.TrimRight();
	CString strFilePath;
	COleDateTime time=COleDateTime::GetCurrentTime();
	strFloderName=time.Format("%Y%m%d%H%M%S")+"_"+strFloderName;
	strFilePath=GetAppPath()+"\\data\\"+strFloderName;
	if(!IsFileExist(strFilePath,TRUE))
	{
		CreateDirectory(strFilePath,NULL);
	}
	return strFloderName;
}
//功能:自动制作文件夹返回文件夹名
CString CFileOperate::MakeDirectory()
{
	CString strFilePath,strFloderName;
	COleDateTime time=COleDateTime::GetCurrentTime();
	strFloderName=time.Format("%Y%m%d%H%M%S");
	strFilePath=GetAppPath()+"\\data\\"+strFloderName;
	if(!IsFileExist(strFilePath,TRUE))
	{
		CreateDirectory(strFilePath,NULL);
	}
	return strFloderName;
}
//功能:得到文件夹名为strFloderName的路径
CString CFileOperate::GetDirectoryPath(CString strFloderName)
{
	strFloderName.TrimLeft();
	strFloderName.TrimRight();
	CString strFilePath;
	strFilePath=GetAppPath()+"\\data\\"+strFloderName;
	if(!IsFileExist(strFilePath,TRUE))
	{
		CreateDirectory(strFilePath,NULL);
	}
	return strFilePath;
}
//功能:删除strFloderdName文件夹
void CFileOperate::delDirectory(CString strFloderName)
{
	strFloderName.TrimLeft();
	strFloderName.TrimRight();
	CFileFind finder;
	CString strPath;
	strPath=GetAppPath()+"\\data\\"+strFloderName+"\\*.*";
	BOOL bWork=finder.FindFile(strPath);
	while(bWork)
	{
		bWork=finder.FindNextFile();
		if (finder.IsArchived())
		{
			CString str1=finder.GetFilePath();
			DeleteFile(str1);
		}
		else
		{
			CString str2=finder.GetFilePath();
			RemoveDirectory(str2);
		}
	}
	finder.Close();
	strPath=GetAppPath()+"\\data\\"+strFloderName;
	RemoveDirectory(strPath);
}
//功能:制作主文件夹
void CFileOperate::MakeMainDirectory()
{
	CString strFilePath;
	strFilePath=GetAppPath()+"\\data\\";
	if(!IsFileExist(strFilePath,TRUE))
	{
		CreateDirectory(strFilePath,NULL);
	}
}
//功能:删除主文件夹
void CFileOperate::delMainDirectory()
{
	CString strFilePath;
	strFilePath=GetAppPath()+"\\data\\";
	if(IsFileExist(strFilePath,TRUE))
	{
		RemoveDirectory(strFilePath);
	}
}
//功能:判断strIntDigit是否为整数，是否小于intBig
BOOL CFileOperate::CheckIntDigit(CString strIntDigit,int intBig)
{	
	strIntDigit.TrimLeft();
	strIntDigit.TrimRight();
	BOOL bln=TRUE;
	int i=0;
	if (strIntDigit=="")
	{
		return TRUE;
	}
	else
	{
		while (i<strIntDigit.GetLength() && bln==TRUE)
		{
			if (strIntDigit.GetAt(i)>='0' && strIntDigit.GetAt(i)<='9')
			{
				bln=TRUE;
			}
			else
			{
				bln=FALSE;
			}
			i=i+1;
		}
		if (bln==TRUE)
		{
			if (atoi(strIntDigit)<=intBig)
			{
				return TRUE;
			}
			else 
				return FALSE;
		}
			else
				return FALSE;
	}
}
//功能:判断strFileName是否可以做文件夹或文件的名字
BOOL CFileOperate::CheckFileName(CString strFileName)
{
	strFileName.TrimLeft();
	strFileName.TrimRight();
	char ch[10]={'\\','/',':','*','?','<','>','|','"','.'};
	for (int i=0;i<strFileName.GetLength();i++)
	{
		for (int j=0;j<10;j++)
		{
			if (strFileName.GetAt(i)==ch[j])
			{
				return FALSE;
			}
		}

	}
	if (i==strFileName.GetLength())
	{
		return TRUE;
	}
	else
		return FALSE;
}
//功能:判断strText是否为空
BOOL CFileOperate::CheckEmpty(CString strText)
{
	strText.TrimLeft();
	strText.TrimRight();
	if(strText=="")
		return TRUE;
	else
		return FALSE;
}
//功能:删除strFloderName文件夹下名为strFileName的文件
void CFileOperate::delFile(CString strFloderName,CString strFileName)
{
	strFloderName.TrimLeft();
	strFloderName.TrimRight();
	strFileName.TrimLeft();
	strFileName.TrimRight();
	CFileFind finder;
	CString strPath;
	strPath=GetAppPath()+"\\data\\";
	strPath=strPath+strFloderName;
	strPath=strPath+"\\"+strFileName;
	DeleteFile(strPath);
}
//功能:得到strFloderName文件夹下名为strFileName的文件路径
CString CFileOperate::GetFileName(CString strFolderName,CString strFileName)
{
	strFolderName.TrimLeft();
	strFolderName.TrimRight();
	strFileName.TrimLeft();
	strFileName.TrimRight();
	CString strPath;
	strPath=GetAppPath()+"\\data\\"+strFolderName+"\\"+strFileName;
	return strPath;
}
//功能:制作strFloderName文件夹下名为strFileName的文件
CString CFileOperate::MakeFile(CString strFloderName,BOOL blnMake)
{
	strFloderName.TrimLeft();
	strFloderName.TrimRight();
	CString strFilePath;
	CString strFileName;
	COleDateTime time=COleDateTime::GetCurrentTime();
	if (blnMake)//实时
	{
		strFileName="0_"+time.Format("%Y%m%d%H%M%S")+".tl";
	}
	else//回放
	{
		strFileName="1_"+time.Format("%Y%m%d%H%M%S")+".tl";
	}
	strFilePath=GetDirectoryPath(strFloderName)+"\\"+strFileName;
	return strFilePath;
}
BOOL CFileOperate::GetFolder(CString* strSelectedFolder,const char* lpszTitle,const HWND hwndOwner,const char* strRootFolder,const char* strStartFolder)
{
	char pszDisplayName[MAX_PATH];
	LPITEMIDLIST lpID;
	BROWSEINFOA bi;
	bi.hwndOwner = hwndOwner;
	if (strRootFolder == NULL)
	{
		bi.pidlRoot = NULL;
	}
	else
	{
		LPITEMIDLIST  pIdl = NULL;
		IShellFolder* pDesktopFolder;
		char          szPath[MAX_PATH];
		OLECHAR       olePath[MAX_PATH];
		ULONG         chEaten;
		ULONG         dwAttributes;
		
		strcpy(szPath, (LPCTSTR)strRootFolder);
		if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
		{
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szPath, -1, olePath, MAX_PATH);
			pDesktopFolder->ParseDisplayName(NULL, NULL, olePath, &chEaten, &pIdl, &dwAttributes);
			pDesktopFolder->Release();
		}
		bi.pidlRoot = pIdl;
	}
	bi.pszDisplayName = pszDisplayName;
	bi.lpszTitle = lpszTitle;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpfn = BrowseCallbackProc;
	if (strStartFolder == NULL)
	{
		bi.lParam = FALSE;
	}
	else
	{
		strTmpPath.Format("%s", strStartFolder);
		bi.lParam = TRUE;
	}
	bi.iImage = NULL;
	lpID = SHBrowseForFolderA(&bi);
	if (lpID != NULL)
	{
		BOOL b = SHGetPathFromIDList(lpID, pszDisplayName);
		if (b == TRUE)
		{
			strSelectedFolder->Format("%s",pszDisplayName);
			return TRUE;
		}
	}
	else
	{
		strSelectedFolder->Empty();
	}
	return FALSE;
}