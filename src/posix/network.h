#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define DEFAULT_SERVER_PORT "36000"
#define REC_BUF_SIZE 1024

int nm_network_startstream(void);
int nm_network_startserver(void);

#endif