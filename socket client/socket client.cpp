//  客户端代码
// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "winsock2.h"
#include <iostream>
#include <fstream> 
#include <algorithm> 
#include <cmath> 
#include <deque> 
#include <vector> 
#include <queue> 
#include <string> 
#include <cstring> 
#include <map> 
#include <stack> 
#include <set> 
#pragma comment(lib, "ws2_32.lib")

using namespace std;
BOOL RecvLine(SOCKET s, char* buf); //读取一行数据

int main(int argc, char* argv[])
{
	


	const int BUF_SIZE = 64;

	WSADATA wsd; //WSADATA变量
	SOCKET sHost; //服务器套接字
	SOCKADDR_IN servAddr; //服务器地址
	char buf[BUF_SIZE]; //接收数据缓冲区
	char bufRecv[BUF_SIZE];
	int retVal; //返回值

	//cout << "start" << endl;

	//初始化套结字动态库
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		cout << "WSAStartup failed!" << endl;
		return -1;
	}

	//创建套接字
	sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sHost)
	{
		cout << "socket failed!" << endl;
		WSACleanup();//释放套接字资源
		return -1;
	}


	//设置服务器地址
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr("192.168.8.111");
	servAddr.sin_port = htons((short)4999);
	int nServAddlen = sizeof(servAddr);

	//连接服务器
	retVal = connect(sHost, (LPSOCKADDR)&servAddr, sizeof(servAddr));
	//cout << "start" << endl;
	if (SOCKET_ERROR == retVal)
	{
		cout << "connect failed!" << endl;
		closesocket(sHost); //关闭套接字
		WSACleanup(); //释放套接字资源
		return -1;
	}
	//cout << "start" << endl;
	while (true) {
		//cout << "start" << endl;
		//向服务器发送数据
		ZeroMemory(buf, BUF_SIZE);
		cout << " 向服务器发送数据:  ";
		cin >> buf;
		retVal = send(sHost, buf, strlen(buf), 0);
		if (SOCKET_ERROR == retVal)
		{
			cout << "send failed!" << endl;
			closesocket(sHost); //关闭套接字
			WSACleanup(); //释放套接字资源
			return -1;
		}
		//RecvLine(sHost, bufRecv);
		//recv(sHost, bufRecv, 5, 0); // 接收服务器端的数据， 只接收5个字符
		int ret;
		ret = recv(sHost, bufRecv, strlen(bufRecv), 0);
		/*while (ret !=75)
		{
			sleep(1);
			recv(sHost, bufRecv, strlen(bufRecv), 0);
			cout << endl << "从服务器接收数据：" << bufRecv;
			
		}
		*/
		cout << endl << "从服务器接收数据：" << bufRecv;
		ret = -1;

		char* path = ".//out.txt"; // 你要创建文件的路径
		ofstream fout(path);
		if (fout) { // 如果创建成功
			fout << bufRecv << endl; // 使用与cout同样的方式进行写入
			fout.close();  // 执行完操作后关闭文件句柄
		}

	}
	//退出
	closesocket(sHost); //关闭套接字
	WSACleanup(); //释放套接字资源
	return 0;
}


