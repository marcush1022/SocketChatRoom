// ConsoleApplication1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "winsock2.h"
#include <iostream>
//创建新的套接字之前需要调用一个引入Ws2_32.dll库的函数,否则服务器和客户端连接不上
#pragma comment(lib,"ws2_32.lib") 
using namespace std;

int main(int argc, char* argv[])
{
	WSADATA wsd; //指向WinSocket信息结构的指针
	SOCKET sHost;
	SOCKADDR_IN servAddr; //设置两个地址，sin用来绑定
								 //saClient用来从广播地址接收消息
	char cRecvBuff[64]; //定义接收缓冲区
	char buf[64]; //接收数据缓冲区

	int nSize, nbSize;
	int iAddrLen = sizeof(servAddr);
	if (WSAStartup(MAKEWORD(1, 1), &wsd) != 0) //进行WinSocket的初始化
	{
		printf("Can't initiates windows socket!Program stop.\n");//初始化失败返回-1
		return -1;
	}
	sHost = socket(AF_INET, SOCK_DGRAM, 0);
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(8888); //发送端使用的发送端口，可以根据需要更改
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sHost, (SOCKADDR*)&servAddr, sizeof(servAddr)) != 0)
	{
		printf("Can't bind socket to local port!Program stop.\n");//初始化失败返回-1
		return -1;
	}
	while (1)
	{
		nSize = sizeof(servAddr);
		cout << "mmmmm" << endl;

		if ((nbSize = recvfrom(sHost, cRecvBuff, strlen(cRecvBuff), 0,
			(SOCKADDR*) &servAddr, &nSize)) == SOCKET_ERROR) //若接收失败则提示错误
		{
			printf("Recive Error");
			break;
		}
		cRecvBuff[nbSize] = '\0'; //字符串终止
		cout << endl << "从客户端接收数据：" ;
		printf("%s\n", cRecvBuff); //显示所接收到的字符串
		ZeroMemory(buf, 64);
		std::cout << " 向客户端发送数据:  ";
		std::cin >> buf;

		sendto(sHost, buf, strlen(buf), 0, (SOCKADDR*)&servAddr, nSize);
	}
	return 0;
}

