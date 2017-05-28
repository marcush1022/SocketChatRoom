//  服务器端代码
// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
using namespace std;
#pragma pack(4)
class recieve {
	char robot;
	char order;
	double paras[8];
};
//#pragma pack(pop)
class send_data {
	char robot=0x01;
	char order=0x53;
	char okflag=0x00;
	double paras[8] = {746.0,0.0,336.0,0.0,0.0,0.0};
};
#pragma pack()
int _tmain(int argc, char* argv[])
{
	long long  size = sizeof(send_data);
	send_data m ;
	recieve n;
	char bufbuf[100];
	memset(bufbuf, 0, sizeof(bufbuf));
	memcpy(bufbuf, &m, sizeof(m));
	memcpy(&n, bufbuf, sizeof(n));
	
	
	const int BUF_SIZE = 100;

	WSADATA wsd; //WSADATA变量
	SOCKET sServer; //服务器套接字
	SOCKET sClient; //客户端套接字
	SOCKADDR_IN addrServ;; //服务器地址
	char buf[BUF_SIZE]; //接收数据缓冲区
	char sendBuf[BUF_SIZE];//返回给客户端得数据
	int retVal; //返回值

	//初始化套结字动态库
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		cout << "WSAStartup failed!" << endl;
		return 1;
	}
	//创建套接字
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sServer)
	{
		cout << "socket failed!" << endl;
		WSACleanup();//释放套接字资源;
		return -1;
	}
	//服务器套接字地址 
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(9999);
	addrServ.sin_addr.s_addr = INADDR_ANY;
	//绑定套接字
	retVal = bind(sServer, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));

	if (SOCKET_ERROR == retVal)
	{
		cout << "bind failed!" << endl;
		closesocket(sServer); //关闭套接字
		WSACleanup(); //释放套接字资源;
		return -1;
	}
	//开始监听
	cout << "listen" << endl;

	retVal = listen(sServer, 1);
	if (SOCKET_ERROR == retVal)
	{
		cout << "listen failed!" << endl;
		closesocket(sServer); //关闭套接字
		WSACleanup(); //释放套接字资源;
		return -1;
	}
	//接受客户端请求
	sockaddr_in addrClient;
	int addrClientlen = sizeof(addrClient);
	sClient = accept(sServer, (sockaddr FAR*)&addrClient, &addrClientlen);
	if (INVALID_SOCKET == sClient)
	{
		cout << "accept failed!" << endl;
		closesocket(sServer); //关闭套接字
		WSACleanup(); //释放套接字资源;
		return -1;
	}
	cout << "accept" << endl;

	while (true) {
	//接收客户端数据
		ZeroMemory(buf, BUF_SIZE);
		recieve rev_packet;
		retVal = recv(sClient, buf, BUF_SIZE, 0);
		memset(&rev_packet, 0, sizeof(rev_packet));
		memcpy(&rev_packet, buf, sizeof(rev_packet));
		if (SOCKET_ERROR == retVal)
		{
			cout << "recv failed!" << endl;
			closesocket(sServer); //关闭套接字
			closesocket(sClient); //关闭套接字
			WSACleanup(); //释放套接字资源;
			return -1;
		}
		if (buf[0] == '0')
			break;
		cout << "客户端发送的数据: " << buf << endl;
		cout << "向客户端发送数据: ";
		//cin >> sendBuf;
		send_data sendpacket;
		memcpy(sendBuf, &sendpacket, sizeof(sendpacket));
		send(sClient, sendBuf, sizeof(sendpacket), 0);
	}
	//退出
	closesocket(sServer); //关闭套接字
	closesocket(sClient); //关闭套接字
	WSACleanup(); //释放套接字资源;

	return 0;
}
