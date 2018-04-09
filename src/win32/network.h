#ifndef __NETWORK_H__
#define __NETWORK_H__

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <windows.h>
#include <stdio.h>
#include <time.h>

// https://www.binarytides.com/winsock-socket-programming-tutorial/
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_SERVER_PORT "36000"
#define REC_BUF_SIZE 1024

int nm_network_startstream(void);

#endif