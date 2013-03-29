#include "post.h"
#include <fstream>
#include "tools.h"
int main()
{
	WSADATA wsaData = {0};
	if(WSAStartup(MAKEWORD(2,2), &wsaData))
	{
		cout << "套接字初始化失败!" << endl;
		return false;
	}
	vector<CATEGORY_DATA*> vec;
	CPost post("www.likebeta.net/xmlrpc.php","admin","www.fuck1.com",1);
	post.Run();
	cout << "任务已经完成" << endl;
	WSACleanup();
	system("pause");
	return 0;
}