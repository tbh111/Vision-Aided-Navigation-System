#ifndef SOCKET_PYTHON_HPP
#define SOCKET_PYTHON_HPP
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
class Socket_python {
public:
	Socket_python();
	~Socket_python();
	int connect(string path);
	int close();
	
private:
	SOCKET client;
	char* ip;
	int port;
};

#endif