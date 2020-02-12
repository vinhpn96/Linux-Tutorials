#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define PACKET_SIZE 1460

int count_packets = 0;

void *do_thread(void *data)
{
	int index = 0;
	while (1) 
	{
		sleep(1);
		printf("%d - %d %f Mbits packets = ", index, ++index, count_packets * PACKET_SIZE * 8 / 1024 / 1024, count_packets);
		count_packets = 0;
	}
}

int main()
{
	/* Variable */
	int sock;
	struct sockaddr_in server;
	int mysock;
	char buff[PACKET_SIZE];
	int rval; 
	/* Create socket */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		perror("Failed to create socket");
		exit(1);
	}
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(5000);
	/* Call bind */
	if (bind(sock, (struct sockaddr *)&server, sizeof(server)))
	{
		perror("bind Failed");
		exit(1);
	}
	/* Listen */
	listen(sock, 5);

	int thr_id;
	pthread_t p_thread;
	thr_id = pthread_create(&p_thread, NULL, do_thread, NULL);

	/* Accept */
	do {
		mysock = accept(sock, (struct sockaddr *) 0, 0);
		if (mysock == -1)
		{
			perror("accept Failed");
		}
		else 
		{
			// memset(buff, 0, sizeof(buff));
			if ((rval = recv(mysock, buff, PACKET_SIZE, 0)) <= 0)
				perror("reading stream messenge error");
			// else if (rval == 0)
			// 	printf("Ending connection\n");
			else count_packets++;

			printf("Got the messenge (rval = %d)\n", rval);
			close(mysock);
		}
	}
	while (1);

	return 0;
}