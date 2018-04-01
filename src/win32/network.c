#include "network.h"

// This tut: https://msdn.microsoft.com/en-us/library/windows/desktop/ms739167(v=vs.85).aspx

// Windows only
static int init_wsa(WSADATA **out) {
  *out = NULL;

  int error;
  WSADATA data;
  if ((error = WSAStartup(MAKEWORD(2, 2), &data)) == 0) {
    *out = &data;
  }

  return error;
}

static int init_sock(SOCKET **out) {
  addrinfo *res = NULL, hints;

  // memset on windows, I guess
  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  int error = getaddrinfo(NULL, DEFAULT_SERVER_PORT, &hints, &res);
  if (error) {
    WSACleanup();
    return error;
  }

  SOCKET sock = INVALID_SOCKET;
  sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (sock == INVALID_SOCKET) {
    WSACleanup();
    return 1; //TODO: some error here
  }

  error = bind(sock, res->ai_addr, (int)res->ai_addrlen);
  
  freeaddrinfo(res);

  if (error) {
    closesocket(sock);
    WSACleanup();
    return error;
  }

  *out = &sock;

  return error;
}
