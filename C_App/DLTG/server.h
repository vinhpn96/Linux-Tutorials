#ifndef _SERVER_H_
#define _SERVER_H_

#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <sys/types.h> 
#include "dltg_main.h"

typedef struct server
{
	Sint32 server_fd,
	sockaddr_in server_addr,
	Sint32 ngu_fd,
	sockaddr_in ngu_addr,
	Uint32 teid
} server_t;


Sint32 create_server(Sint8 ip_addr, Uint32 port);

#endif