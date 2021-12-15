#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <process.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <malloc.h>
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4496)

IN_ADDR GetDefaultMyIP() {
	char localhostname[MAX_PATH];
	IN_ADDR addr = { 0, };
	if (gethostname(localhostname, MAX_PATH) == SOCKET_ERROR) {
		return addr;
	}
	HOSTENT* ptr = gethostbyname(localhostname);
	while (ptr && ptr->h_name) {
		if (ptr->h_addrtype == PF_INET) {
			memcpy(&addr, ptr->h_addr_list[0], ptr->h_length);
			break;
		}
		ptr++;
	}
	return addr;
} IN_ADDR GetDefaultMyIP();