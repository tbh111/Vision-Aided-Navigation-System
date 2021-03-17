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
	//����˵�ַ�ͻ��˵�ַ
	SOCKADDR_IN server_addr;
	int slen = sizeof(server_addr);					//���ն˵�ַ����
	int iSend = 0;								//���ͺ�����״̬
	int iRecv = 0;
};

#endif