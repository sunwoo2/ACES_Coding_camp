#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	int			server_sockfd, client_sockfd;
	int			server_len, client_len;
	struct sockaddr_in	server_address;
	struct sockaddr_in	client_address;

	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

	server_address.sin_family = AF_INET:
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(9734);
	server_len = sizeof(server_address);

	bind(server_sockfd, (struct sockaddr*)&server_address, server_len);

	listen(server_sockfd, 5)
}
