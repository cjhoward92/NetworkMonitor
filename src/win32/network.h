#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <windows.h>
#include <stdio.h>

// https://www.binarytides.com/winsock-socket-programming-tutorial/
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_SERVER_PORT "36000"

typedef addrinfo addrinfo;

#endif