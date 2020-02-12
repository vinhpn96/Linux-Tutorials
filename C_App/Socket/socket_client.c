#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define DATA "Hello World!!!"
#define 

void *do_thread(void *data)
{
	int sock;
	struct sockaddr_in server;
	struct hostent *hp;
	char buff[1024];

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		perror("socket failed");
		exit(1);
	}
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(data);
	server.sin_port = htons(5000);
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) <0)
	{
		perror("connect failed");
		close(sock);
		exit(1);
	}
	if (send(sock, DATA, sizeof(DATA), 0) <0)
	{
		perror("send failed");
		close(sock);
		exit(1);
	}
	printf("Sent %s\n", DATA);
	close(sock);
	pthread_exit(NULL);
	return 0;
}

int main(int argc, char *argv[])
{
	int thr_id;
	pthread_t p_thread[2];
	thr_id = pthread_create(&p_thread[0], NULL, do_thread, argv[1]);
	thr_id = pthread_create(&p_thread[1], NULL, do_thread, argv[1]);
	printf("main thread\n");
	do_thread(argv[1]);
}