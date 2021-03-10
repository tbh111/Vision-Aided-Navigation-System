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
	client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (SOCKET_ERROR == client)
	{
		cout << "socket failed" << endl;
		exit(0);
	}
	ip = "127.0.0.1";
	port = 9999;
}
struct Picture {
	char buffer[4096];  //用于发送数据缓冲区
}picture;
Socket_client::~Socket_client() {

}

void openPicture(string data, FILE*& p) {
	while (1) {
		//data = "C:\\Users\\13087\\PycharmProjects\\pythonProject\\1615126946055343.jpg";
		// 以读 / 写方式打开一个二进制文件，只允许读 / 写数据。若图片无法打开，则路径有问题，关闭连接
		if (!(p = fopen(data.c_str(), "rb+"))) {
			//memset(data, 0, sizeof(data));
			cout << "图片路径出错,请重新尝试！" << endl;
		}
		else {
			break;
		}
	}
}

int Socket_client::connect(string path) {
	sockaddr_in saddr;
	//接收端地址长度
	int slen = sizeof(saddr);

	//设置服务器地址
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.S_un.S_addr = inet_addr(ip);
	char data[100] = { 0 };                    //接受一些短字节的数据缓冲区
	char begin[] = "I BEGIN";       //发送图片前的确认信息
	char end[] = "I FINISHED";         //完成图片发送的通知信息

	int iSend = 0;                             //发送函数的状态
	FILE* p;                                   //创建一个文件指针



	//发送图片前先和服务器打个招呼，欲准备状态，判断信息发送是否成功，若不成功，则服务器处于关闭状态
	iSend = sendto(client, begin, strlen(begin), 0, (SOCKADDR*)&saddr, slen);

	if (iSend == SOCKET_ERROR) {
		cout << "服务器处于关闭状态，请稍后重试！" << endl;
		cout << "20s后退出控制台！" << endl;
		closesocket(client);
		WSACleanup();
		Sleep(20000);
		return -4;
	}
	cout << "Client: " << begin << endl;

	memset(data, 0, sizeof(data));

	openPicture(path, p);
	fseek(p, 0, SEEK_END);
	int length = ftell(p);  //获取图片总长度
	fseek(p, 0, SEEK_SET);  //指针还原到开始位置
	cout << "Img length: " << length << endl;

	iSend = sendto(client, (char*)&length, sizeof(int), 0, (SOCKADDR*)&saddr, slen); //首先发送图片大小(单位byte)给接收端
	if (iSend == SOCKET_ERROR) {
		cout << "文件长度信息发送失败！" << endl;
		cout << "10s后退出控制台！" << endl;
		closesocket(client);
		WSACleanup();
		Sleep(10000);
		return -4;
	}

	cout << endl;
	cout << "····BEGIN SEND PICTURE····" << endl;
	int i = 1;

	while (length > 0) {
		cout << i << endl;

		memset(picture.buffer, 0, sizeof(picture.buffer));     //初始化接受缓冲区
		fread(picture.buffer, sizeof(picture.buffer), 1, p);     //读取图片到缓冲区

		int len = sizeof(picture.buffer);                      //获取读取的长度

		/*若读取的长度大于当前图片剩余总长度，将结构体的图片长度赋值为图片剩余长度，
		并标记图片读取结束；否则图片长度为读取缓冲区长度，图片标记状态为未完成 */

		//发送图片的一部分，发送成功，则图片总长度减去当前发送的图片长度
		iSend = sendto(client, (char*)&picture, sizeof(struct Picture), 0, (SOCKADDR*)&saddr, slen);

		if (iSend == SOCKET_ERROR) {
			cout << "发送图片出错" << endl;
			cout << "2s后退出控制台！" << endl;
			closesocket(client);
			WSACleanup();
			Sleep(2000);
			return -8;
		}

		length -= len;
		i++;
	}

	fclose(p);
	p = NULL;


}
int Socket_client::close() {
	closesocket(client);
	WSACleanup();
	cout << "SEND FINISH" << endl;
	return 0;
}