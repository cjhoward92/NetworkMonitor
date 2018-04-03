#include "network.h"

// This tut: https://msdn.microsoft.com/en-us/library/windows/desktop/ms739167(v=vs.85).aspx

static int sock_init(SOCKET **out) {
  int error = 0;
  WSADATA data;
  if ((error = WSAStartup(MAKEWORD(2, 2), &data)) != 0) {
    return error;
  }

  struct addrinfo *res = NULL, hints;

  // memset on windows, I guess
  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  error = getaddrinfo(NULL, DEFAULT_SERVER_PORT, &hints, &res);
  if (error) {
    return error;
  }

  SOCKET sock = INVALID_SOCKET;
  sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (sock == INVALID_SOCKET) {
    printf("socket failed: %d\n", WSAGetLastError());
    return 1; //TODO: some error here
  }

  error = bind(sock, res->ai_addr, (int)res->ai_addrlen);
  
  freeaddrinfo(res);

  if (error) {
    printf("bind failed: %d\n", WSAGetLastError());
    return error;
  }

  *out = &sock;

  return error;
}

static int sock_listen_accept(SOCKET **out, SOCKET sock) {
  *out = NULL;
  if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
    printf("listen failed: %d\n", WSAGetLastError());
    return 1;
  }

  SOCKET client = INVALID_SOCKET;
  client = accept(sock, NULL, NULL);
  if (client == INVALID_SOCKET) {
    printf("accept failed: %d\n", WSAGetLastError());
    return 1;
  }

  *out = &client;
  return 0;
}

static int sock_receive(SOCKET client) {
  char rec_buf[REC_BUF_SIZE];
  int result = 0, sendres = 0;
  int buflen = REC_BUF_SIZE;

  do {
    result = recv(client, rec_buf, buflen, 0);
    if (result > 0) {
      printf("Data received: %s\n", rec_buf);

      sendres = send(client, rec_buf, result, 0);
      if (sendres == SOCKET_ERROR) {
        return 1;
      }
    } else if (result < 0)  {
      return 1;
    }
  } while (result > 0);
  
  return 0;
}

int nm_network_startserver(void) {
  SOCKET *server = NULL;
  SOCKET *client = NULL;
  int error = 0;

  if ((error = sock_init(&server)) != 0) {
    goto cleanup;
  }

  if ((error = sock_listen_accept(&client, (*server))) != 0) {
    goto cleanup;
  }

  error = sock_receive((*client));

cleanup:
  if (server)
    closesocket(*server);
  if (client)
    closesocket(*client);

  WSACleanup();

  return error;
}
