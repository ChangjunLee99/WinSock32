#include "h.h"
#define PORT_NUM 10200
#define BLOG_SIZE 5
#define MAX_MSG_LEN 256
SOCKET SetTCPServer(short pnum, int blog);
void AcceptLoop(SOCKET sock);
int main() {
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	SOCKET sock = SetTCPServer(PORT_NUM, BLOG_SIZE);
	AcceptLoop(sock); // accept
	closesocket(sock); // socket release
	WSACleanup(); // winsock release
	return 0;
}

SOCKET SetTCPServer(short pnum, int blog) {
	SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // generate socket
	SOCKADDR_IN servaddr = { 0 }; // server address
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr = GetDefaultMyIP();
	servaddr.sin_port = htons(pnum);
	int re = 0;
	re = bind(sock, (SOCKADDR*)&servaddr, sizeof(servaddr));
	if (re == -1) { return -1; }// -1 => 0xFFFFFFFFFF => SOCKET_ERROR
	re = listen(sock, blog);
	if (re == -1) { return -1; }
	printf("%s:%d Setup\n", inet_ntoa(servaddr.sin_addr), pnum);
	return sock;
	// connect net interface to socket
}
void DoIt(void* param);
void AcceptLoop(SOCKET sock) {
	SOCKET dosock;
	SOCKADDR_IN cliaddr = { 0 };
	int len = sizeof(cliaddr);
	while (true) {
		dosock = accept(sock, (SOCKADDR*)&cliaddr, &len); // accept connection
		if (dosock == -1) {
			perror("accept 실패");
			break;
		}
		printf("%s:%d의 연결 요청 수락\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
		//DoIt(dosock);

		_beginthread(DoIt, 0, (void*)dosock);
	}
}
void DoIt(void* param) {
	SOCKET dosock = (SOCKET)param;
	SOCKADDR_IN cliaddr = { 0 };
	int len = sizeof(cliaddr);
	getpeername(dosock, (SOCKADDR*)&cliaddr, &len);
	char msg[MAX_MSG_LEN] = "";
	while (recv(dosock, msg, sizeof(msg), 0) > 0) { // return 0 for no connection
		printf("%s:%d로 부터 recv:%s\n",
			inet_ntoa(cliaddr.sin_addr),
			ntohs(cliaddr.sin_port),
			msg);
		send(dosock, msg, sizeof(msg), 0);
	}
	printf("%s:%d와 통신 종료\n",
		inet_ntoa(cliaddr.sin_addr),
		ntohs(cliaddr.sin_port));
	closesocket(dosock);
}