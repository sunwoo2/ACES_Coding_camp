#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	struct sockaddr_un server_address;
	struct sockaddr_un client_address;

	unlink("server_socket");	// 이미 존재하면 지워주는거
	server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);	// 소켓 껍데기
	// AF_UNIX : 프로세스안에서 통신 하겠다.
	// SOCK_STRAM : tcp통신 쓰겟다.
	server_address.sun_family = AF_UNIX;
	strcpy(server_address.sun_path, "server_socket");
	server_len = sizeof(server_address);
	bind(server_sockfd, (struct sockaddr*)&server_address, server_len);
	// bind : 소켓 껍데기랑 다른속성들이랑 묶어주는거

	listen(server_sockfd, 5);
	// 대기열 만들어 주기 대기열 5개 늘린거임 지금은
	while(1){
		char ch;

		printf("server waiting\n");

		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_address, &client_len);
		// accept : 여기서 묶여있음
		// client에서 connect되면 이제 연결된거임
		read(client_sockfd, &ch, 1);	// client ->
		ch++;
		printf("client1: %c\n",ch);
		write(client_sockfd, &ch, 1);	// ->client
		close(client_sockfd);
	}
}
