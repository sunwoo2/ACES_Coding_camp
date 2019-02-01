#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	struct sockaddr_in server_address;	// 소켓 껍데기 생성
	struct sockaddr_in client_address;

	unlink("server_socket");
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(9104);;
	server_len = sizeof(server_address);
	bind(server_sockfd, (struct sockaddr*)&server_address, server_len);

	// 대기열 만들어 주기 대기열 5개 늘린거임 지금은
	listen(server_sockfd, 5);

	client_len = sizeof(client_address);
	// client 연결 대기
	client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_address, &client_len);
	while(1){
		char ch;
	
		// client가 쓴거 읽기
/* client-> */	read(client_sockfd, &ch, 1);
		
		// 읽은거 출력
		printf("client: %c\n", ch);
		
		// 키보드로 채팅내용 입력받기
		fscanf(stdin, "%c", &ch);
		// 채팅내용 출력 
		printf("나: %c\n", ch);

		// 내가 쓰기
		write(client_sockfd, &ch, 1);	// ->client	
	}
	close(client_sockfd);
}
