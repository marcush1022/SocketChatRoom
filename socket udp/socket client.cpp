//  客户端代码
// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "winsock2.h"
#include <iostream>
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
	BOOL fBroadcast = TRUE;

	//初始化套结字动态库
	if (WSAStartup(MAKEWORD(1, 1), &wsd) != 0)
	{
		cout << "WSAStartup failed!" << endl;
		return -1;
	}

	//创建套接字
	sHost = socket(PF_INET, SOCK_DGRAM, 0);
	if (INVALID_SOCKET == sHost)
	{
		cout << "socket failed!" << endl;
		WSACleanup();//释放套接字资源
		return -1;
	}
	//setsockopt(sHost, SOL_SOCKET, SO_BROADCAST, (CHAR *)&fBroadcast, sizeof(BOOL));
	//设置服务器地址
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr("192.168.1.102");
	servAddr.sin_port = htons((short)8888);
	int nServAddlen = sizeof(servAddr);


	while (true) {
		//向服务器发送数据
		ZeroMemory(buf, BUF_SIZE);
		cout << " 向服务器发送数据:  ";
		cin >> buf;
		retVal = sendto(sHost, buf, strlen(buf), 0, (SOCKADDR *)&servAddr, nServAddlen);
		if (SOCKET_ERROR == retVal)
		{
			cout << "send failed!" << endl;
			closesocket(sHost); //关闭套接字
			WSACleanup(); //释放套接字资源
			return -1;
		}
		//RecvLine(sHost, bufRecv);
		//recv(sHost, bufRecv, 5, 0); // 接收服务器端的数据， 只接收5个字符
		int nbSize=recvfrom(sHost, bufRecv, strlen(bufRecv), 0,(SOCKADDR *)&servAddr, &nServAddlen);
		bufRecv[nbSize] = '\0'; //字符串终止
		cout << endl << "从服务器接收数据：";
		printf("%s\n", bufRecv); //显示所接收到的字符串

	}
	//退出
	closesocket(sHost); //关闭套接字
	WSACleanup(); //释放套接字资源
	return 0;
}


