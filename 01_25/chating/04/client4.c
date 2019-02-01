#include "chat.h"

int main(){
	int sockfd;
	int len;
	struct sockaddr_in address;
	int port_num = 20000;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		printf("socket 생성 실패!\n");
		exit(1);
	}else
		printf("socket 생성\n");

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");	// 서버주소 (여기선 시스템 자기주소사용)
	address.sin_port = htons(port_num);
	len = sizeof(address);

	// connect() : 서버로 접속 시도 
	if( connect(sockfd, (struct sockaddr*)&address, len) == -1){
		printf("서버 접속 실패\n");
		printf("다른포트로 연결\n");
		address.sin_port = htons(++port_num);
		if( connect(sockfd, (struct sockaddr*)&address, len) == -1){
			printf("서버 접속 실패\n");
			exit(1);
		}
	}else
		printf("서버에 접속\n");
	
	
	// 스레드
	pthread_t rt;	
	pthread_t wt;	

	if( pthread_create(&rt, NULL, read_function, &sockfd) != 0)
		printf("read thread 생성 실패!\n");
	if( pthread_create(&wt, NULL, write_function, &sockfd) != 0)
		printf("write thread 생성 실패!\n");

	if( pthread_join(rt, NULL) == 0)
		printf("read thread join\n");
	pthread_cancel(wt);
	if( pthread_join(wt, NULL) == 0)
		printf("write thread join\n");


	// socket 소멸 (data 통신 종료)
	if( close(sockfd) == 0)
		printf("서버와 소켓 통신 종료\n");

	exit(0);
}

void* read_function(void* sockfd){
	char buf_read[BUFF_SIZE+5];
	while(1){
		// read() : 전송된 자료 읽기
		// 만약 전송된 자료가 없으면 송신할 때까지 대기
/* -> */	read(*(int*)sockfd, buf_read, BUFF_SIZE);

		// exit thread
		if( strcmp(buf_read, "end") == 0){
			pthread_exit(NULL);
			printf("read_function while문을 빠져나갑니다.\n");
			break;
		}
		
		// 읽은거 출력
		printf("client: %s\n", buf_read);
	}
	printf("read_function while문을 빠져나왔습니다.\n");
}

void* write_function(void* sockfd){
	char buf_write[BUFF_SIZE+5];
	while(1){
		// 키보드로 채팅내용 입력받기
		fscanf(stdin, "%s", buf_write);
		// 채팅내용 출력 
		printf("나: %s\n", buf_write);

		// write() : data 전송
		write(*(int*)sockfd, buf_write, BUFF_SIZE);	// ->	

		// exit thread
		if( strcmp(buf_write, "end") == 0){
			pthread_exit(NULL);
			printf("write_function while문을 빠져나갑니다.\n");
			break;
		}
	}
	printf("write_function while문을 빠져나왔습니다.\n");
}

