#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define DATA "Hello World!!!"

int main(int argc, char *argv[])
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
	server.sin_addr.s_addr = inet_addr(argv[1]);
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

	return 0;
}