#include "Socket_client.hpp"
Socket_client::Socket_client() {
	WORD version = MAKEWORD(2, 2);
	WSADATA wsadata;
	if (WSAStartup(version, &wsadata))
	{
		cout << "WSAStartup failed " << endl;
		exit(0);
	}
	//判断版本
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2)
	{
		cout << "wVersion not 2.2" << endl;
		exit(0);
	}

	if (SOCKET_ERROR == s_server)
	{
		cout << "socket failed" << endl;
		exit(0);
	}
	ip = "127.0.0.1";
	port = 8888;

}

Socket_client::~Socket_client() {

}

int Socket_client::connect_qt() {


	//设置服务器地址
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.S_un.S_addr = inet_addr(ip);
	s_server = socket(AF_INET, SOCK_STREAM, 0);

	if (connect(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		cout << "服务器连接失败！" << endl;
		WSACleanup();
	}
	else {
		cout << "服务器连接成功！" << endl;
	}

	char data[100] = { 0 };						//接受一些短字节的数据缓冲区
	char begin[] = "BEGIN";					//发送图片前的确认信息

	//发送前先和服务器打个招呼，欲准备状态，判断信息发送是否成功，若不成功，则服务器处于关闭状态
	//iSend = sendto(s_server, begin, strlen(begin), 0, (SOCKADDR*)&s_server, slen);
	cout << "Client: " << begin << endl;
	return 0;
}

void Socket_client::send_data(float x, float y, float z, string cmd) {
	char end[] = "send one package\n";					//完成发送的通知信息

	char buf1[20] = "dx";
	char buf2[20] = "dy";
	char buf3[20] = "dz";

	if (cmd == "gps")
	{
		buf1[0] = 'l';
		buf2[0] = 'l';
		buf3[0] = 'l';
	}

	gcvt(x, 5, buf1+2);
	gcvt(y, 5, buf2+2);
	gcvt(z, 5, buf3+2);
	*(buf1 + 8) = '\n';
	*(buf2 + 8) = '\n';
	*(buf3 + 8) = '\n';

	iSend = sendto(s_server, buf1, strlen(buf1), 0, (SOCKADDR*)&s_server, slen);

	iSend = sendto(s_server, buf2, strlen(buf2), 0, (SOCKADDR*)&s_server, slen);

	iSend = sendto(s_server, buf3, strlen(buf3), 0, (SOCKADDR*)&s_server, slen);

	iSend = sendto(s_server, end, strlen(end), 0, (SOCKADDR*)&s_server, slen);
	cout << "end" << endl;
}

void Socket_client::send_data(int x) {

}

char Socket_client::read_data() {
	char buf[2] = {'x','\0'};
	iRecv = recv(s_server, buf, 1, 0);

	if (iRecv > 0) {
		cout << "received data" << buf << endl;
	}
	else
	{
		cout << "waiting" << endl;
	}
	return buf[0];
}

int Socket_client::close_qt() {
	closesocket(s_server);
	WSACleanup();
	cout << "SEND FINISH" << endl;
	return 0;
}

