all: server client

server: socket_server.c 
	gcc socket_server.c -o socket_server -lpthread

client: socket_client.c 
	gcc socket_client.c -o socket_client -lpthread
	
clean:
	rm -f socket_*
