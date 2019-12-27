#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	/* Variable */
	int sock;
	struct sockaddr_in server, client;
	int mysock;
	char buff[1024];
	int rval; 

	memset(&server, 0, sizeof(server));
	memset(&client, 0, sizeof(client))
	/* Create socket */
	sock = socket(AF_INET, SOCK_DGRAM, 0);
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
	n = recvfrom(sock, buff, 1024,MSG_WAITALL, )

	return 0;
}