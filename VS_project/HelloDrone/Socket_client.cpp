#include "Socket_client.hpp"

Socket_client::Socket_client() {
	WORD version = MAKEWORD(2, 2);
	WSADATA wsadata;
	if (WSAStartup(version, &wsadata))
	{
		cout << "WSAStartup failed " << endl;
		exit(0);
	}
	//�жϰ汾
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
	char buffer[4096];  //���ڷ������ݻ�����
}picture;
Socket_client::~Socket_client() {

}

void openPicture(string data, FILE*& p) {
	while (1) {
		//data = "C:\\Users\\13087\\PycharmProjects\\pythonProject\\1615126946055343.jpg";
		// �Զ� / д��ʽ��һ���������ļ���ֻ����� / д���ݡ���ͼƬ�޷��򿪣���·�������⣬�ر�����
		if (!(p = fopen(data.c_str(), "rb+"))) {
			//memset(data, 0, sizeof(data));
			cout << "ͼƬ·������,�����³��ԣ�" << endl;
		}
		else {
			break;
		}
	}
}

int Socket_client::connect(string path) {
	sockaddr_in saddr;
	//���ն˵�ַ����
	int slen = sizeof(saddr);

	//���÷�������ַ
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.S_un.S_addr = inet_addr(ip);
	char data[100] = { 0 };                    //����һЩ���ֽڵ����ݻ�����
	char begin[] = "I BEGIN";       //����ͼƬǰ��ȷ����Ϣ
	char end[] = "I FINISHED";         //���ͼƬ���͵�֪ͨ��Ϣ

	int iSend = 0;                             //���ͺ�����״̬
	FILE* p;                                   //����һ���ļ�ָ��



	//����ͼƬǰ�Ⱥͷ���������к�����׼��״̬���ж���Ϣ�����Ƿ�ɹ��������ɹ�������������ڹر�״̬
	iSend = sendto(client, begin, strlen(begin), 0, (SOCKADDR*)&saddr, slen);

	if (iSend == SOCKET_ERROR) {
		cout << "���������ڹر�״̬�����Ժ����ԣ�" << endl;
		cout << "20s���˳�����̨��" << endl;
		closesocket(client);
		WSACleanup();
		Sleep(20000);
		return -4;
	}
	cout << "Client: " << begin << endl;

	memset(data, 0, sizeof(data));

	openPicture(path, p);
	fseek(p, 0, SEEK_END);
	int length = ftell(p);  //��ȡͼƬ�ܳ���
	fseek(p, 0, SEEK_SET);  //ָ�뻹ԭ����ʼλ��
	cout << "Img length: " << length << endl;

	iSend = sendto(client, (char*)&length, sizeof(int), 0, (SOCKADDR*)&saddr, slen); //���ȷ���ͼƬ��С(��λbyte)�����ն�
	if (iSend == SOCKET_ERROR) {
		cout << "�ļ�������Ϣ����ʧ�ܣ�" << endl;
		cout << "10s���˳�����̨��" << endl;
		closesocket(client);
		WSACleanup();
		Sleep(10000);
		return -4;
	}

	cout << endl;
	cout << "��������BEGIN SEND PICTURE��������" << endl;
	int i = 1;

	while (length > 0) {
		cout << i << endl;

		memset(picture.buffer, 0, sizeof(picture.buffer));     //��ʼ�����ܻ�����
		fread(picture.buffer, sizeof(picture.buffer), 1, p);     //��ȡͼƬ��������

		int len = sizeof(picture.buffer);                      //��ȡ��ȡ�ĳ���

		/*����ȡ�ĳ��ȴ��ڵ�ǰͼƬʣ���ܳ��ȣ����ṹ���ͼƬ���ȸ�ֵΪͼƬʣ�೤�ȣ�
		�����ͼƬ��ȡ����������ͼƬ����Ϊ��ȡ���������ȣ�ͼƬ���״̬Ϊδ��� */

		//����ͼƬ��һ���֣����ͳɹ�����ͼƬ�ܳ��ȼ�ȥ��ǰ���͵�ͼƬ����
		iSend = sendto(client, (char*)&picture, sizeof(struct Picture), 0, (SOCKADDR*)&saddr, slen);

		if (iSend == SOCKET_ERROR) {
			cout << "����ͼƬ����" << endl;
			cout << "2s���˳�����̨��" << endl;
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