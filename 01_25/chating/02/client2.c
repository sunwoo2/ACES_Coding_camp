#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void* read_function(void* sockfd){
	while(1){
		char ch[100];
	
		// client가 쓴거 읽기
/* client-> */	read(*(int*)sockfd, &ch, sizeof(ch));
		
		// 읽은거 출력
		printf("server: %s\n", ch);
		
	}

}

void* write_function(void* sockfd){
	while(1){
		char ch[100];
		// 키보드로 채팅내용 입력받기
		fscanf(stdin, "%s", ch);
		// 채팅내용 출력 
		printf("나: %s\n", ch);

		// 내가 쓰기
		write(*(int*)sockfd, &ch, sizeof(ch));	// ->client	
	}
}

int main(){
	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;
	char ch[100];

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
	
	
	// 스레드
	pthread_t rt;	
	pthread_t wt;	

	pthread_create(&rt, NULL, read_function, &sockfd);
	pthread_create(&wt, NULL, write_function, &sockfd );

	pthread_join(rt, NULL);
	pthread_join(wt, NULL);





	close(sockfd);
	exit(0);
}
