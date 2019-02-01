#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;
	char ch;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	//address.sin_port = 9734;
	address.sin_port = htons(9104);
	len = sizeof(address);


	// 연결요청
	result = connect(sockfd, (struct sockaddr*)&address, len);

	if(result == -1){
		perror("oops: client2");
		exit(1);
	}
	while(1){

		// 키보드로 채팅내용 입력받기
		fscanf(stdin, "%c", &ch);
		// 채팅내용 출력 
		printf("나: %c\n", ch);
		
		// 입력받은 채팅내용 쓰기
		write(sockfd, &ch, 1);		// ->server
		
		// server가 쓴거 읽기
/* server ->*/	read(sockfd, &ch, 1);		

		// 읽은거 출력
		printf("server: %c\n", ch);
	}

	close(sockfd);
	exit(0);
}
