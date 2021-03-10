#ifndef SOCKET_CLIENT_HPP
#define SOCKET_CLIENT_HPP
#include <cstdio>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)
#include <iostream>
#include <string>
#include <opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
using namespace std;
using namespace cv;
#define BuffSize 1024
class Socket_client {
public:
	Socket_client();
	~Socket_client();
	int connect(string path);
	int close();
	
private:
	SOCKET client;
	char* ip;
	int port;
};

#endif