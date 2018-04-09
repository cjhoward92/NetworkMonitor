#include "network.h"

// This tut: https://msdn.microsoft.com/en-us/library/windows/desktop/ms739167(v=vs.85).aspx
static SOCKET stream_init(void) {
  int error = 0;
  WSADATA data;
  if ((error = WSAStartup(MAKEWORD(2, 2), &data)) != 0)
    return INVALID_SOCKET;

  struct addrinfo *res = NULL, hints;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  error = getaddrinfo(NULL, DEFAULT_SERVER_PORT, &hints, &res);
  if (error)
    return INVALID_SOCKET;

  SOCKET sock = INVALID_SOCKET;
  sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (sock == INVALID_SOCKET) {
    printf("socket failed: %d\n", WSAGetLastError());
    return INVALID_SOCKET;
  }

  error = bind(sock, res->ai_addr, (int)res->ai_addrlen);
  
  freeaddrinfo(res);

  if (error) {
    printf("bind failed: %d\n", WSAGetLastError());
    return INVALID_SOCKET;
  }

  return sock;
}

static SOCKET stream_init_client(SOCKET sock) {
  if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
    printf("listen failed: %d\n", WSAGetLastError());
    return INVALID_SOCKET;
  }

  SOCKET client = INVALID_SOCKET;
  client = accept(sock, NULL, NULL);
  if (client == INVALID_SOCKET) {
    printf("accept failed: %d\n", WSAGetLastError());
    return INVALID_SOCKET;
  }

  return client;
}

static int stream_receive(SOCKET client) {
  char rec_buf[REC_BUF_SIZE];
  int result = 0, send_res = 0;
  int buf_len = REC_BUF_SIZE;

  memset(rec_buf, 0, REC_BUF_SIZE);

  do {
    result = recv(client, rec_buf, buf_len, 0);
    if (result > 0) {
      printf("Data received: %s\n", rec_buf);

      send_res = send(client, rec_buf, result, 0);
      if (send_res == SOCKET_ERROR) {
        return 1;
      }
    } else if (result < 0)  {
      return 1;
    }
  } while (result > 0);
  
  return 0;
}

int nm_network_start_server(void) {
  int error = 0;

  SOCKET server = stream_init();
  if (server == INVALID_SOCKET)
    goto cleanup;

  SOCKET client = stream_init_client(server);
  if (client == INVALID_SOCKET)
    goto cleanup;

  error = stream_receive(client);

cleanup:
  if (server != INVALID_SOCKET)
    closesocket(server);
  if (client != INVALID_SOCKET)
    closesocket(client);

  WSACleanup();

  return error;
}

static SOCKET client_init(void) {
  struct addrinfo *result = NULL, *ptr = NULL, hints;
  
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  // localhost should be replaced with the server name
  int error = getaddrinfo("127.0.0.1", DEFAULT_SERVER_PORT, &hints, &result);
  if (error) {
    WSACleanup();
    return error;
  }

  // Use all of the addresses returned from getaddrinfo to try and connect
  ptr = result;
  SOCKET sock = INVALID_SOCKET;
  while (ptr) {
    sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (sock == INVALID_SOCKET) {
      ptr = ptr->ai_next;
      continue;
    }

    error = connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (error == SOCKET_ERROR) {
      closesocket(sock);
      sock = INVALID_SOCKET;
      ptr = ptr->ai_next;  
      continue;
    }
    break;
  }

  freeaddrinfo(result);
  
  if (sock == INVALID_SOCKET) {
    printf("Unable to connect to server!\n");
    WSACleanup();
    return error;
  }

  return sock;
}

int nm_network_createclient(void) {
  SOCKET sock = client_init();
  if (sock == INVALID_SOCKET)
    return 1;

  int rec_buf_len = REC_BUF_SIZE;
  char *send_buf = "Hello darkness my old friend";
  char rec_buf[REC_BUF_SIZE];
  memset(rec_buf, 0, rec_buf_len);

  int result = send(sock, send_buf, (int)strlen(send_buf), 0);
  if (result == SOCKET_ERROR)
    goto cleanup;

  printf("Sent %d bytes\n", result);

  result = shutdown(sock, SD_SEND);
  if (result == SOCKET_ERROR)
    goto cleanup;

  do {
    result = recv(sock, rec_buf, rec_buf_len, 0);
    if (result > 0)
      printf("Recieved %s\n", rec_buf);
    else if (result < 0)
      printf("recv failed: %d\n", WSAGetLastError());
  } while (result > 0);

cleanup:
  closesocket(sock);
  WSACleanup();
  return 0;
}
