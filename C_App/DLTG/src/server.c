#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include "server.h"

server_t g_server;

static void server_loop();

Sint32 create_server(Sint8 server_ip_addr, Uint32 server_port, Sint8 ngu_ip_addr, Uint32 ngu_port)
{
	Sint32 dl_server_fd, dl_client_fd, thr_id; 
	pthread_t p_thread;
	Sint8 buffer[MAXLINE]; 
	struct sockaddr_in dl_server_addr, dl_client_addr; 
	
	// Creating socket file descriptor 
	if ( (dl_server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) 
	{ 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	g_server.server_fd = dl_server_fd;

	if ( (dl_client_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) 
	{ 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	}
	g_server.ngu_fd = dl_client_addr;
	
	// Filling server information 
	g_server.server_addr.sin_family = AF_INET; // IPv4 
	g_server.server_addr.sin_addr.s_addr = inet_addr(server_ip_addr); 
	g_server.server_addr.sin_port = htons(server_port); 

	g_server.ngu_addr.sin_family = AF_INET; // IPv4 
	g_server.ngu_addr.sin_addr.s_addr = inet_addr(ngu_ip_addr); 
	g_server.ngu_addr.sin_port = htons(ngu_port);
	
	// Bind the socket with the server address 
	if ( bind(dl_server_fd, (const struct sockaddr *)&g_server.server_addr, 
			sizeof(g_server.server_addr)) < 0 ) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 

	thr_id = pthread_create(&p_thread, NULL, server_loop, NULL);

	return dl_server_addr;
}

static void server_loop()
{
	struct sockaddr_in cliaddr;
	while (1)
	{
		n = recvfrom(g_server.server_fd, (char *)buffer, MAXLINE,  
                MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
                &len);
		sendto(sockfd, (const char *)hello, strlen(hello), 
        MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
            sizeof(servaddr));
	}
}