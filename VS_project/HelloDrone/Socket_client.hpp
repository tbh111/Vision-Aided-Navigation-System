#ifndef SOCKET_CLIENT_HPP
#define SOCKET_CLIENT_HPP
#include <cstdio>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;
#define BuffSize 1024
class Socket_client
{
public:
	Socket_client();
	~Socket_client();
	int connect_qt();
	int close_qt();
	void send_data(float x, float y, float z, string cmd);
	void send_data(int x);
	char read_data();
private:
	char* ip;
	int port;
	SOCKET s_server;
	//服务端地址客户端地址
	SOCKADDR_IN server_addr;
	int slen = sizeof(server_addr);					//接收端地址长度
	int iSend = 0;								//发送函数的状态
	int iRecv = 0;
};

#endif