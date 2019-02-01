#include "chat.h"

int main(){
	int server_sockfd;	// 소켓 디스크립터
	int client_sockfd;
	int server_len; 
	int client_len;
	int port_num = 19100;
	// 소켓 상자 생성
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
		
	unlink("server_socket");	// 다 끝나면 파일 삭제!?

	// socket 생성(반환값은 int형 socket식별자) 
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if( server_sockfd == -1){
		printf("server socket 생성 실패!\n");
		exit(1);
	}else
		printf("server socket 생성\n");
	
	// 소켓의정보를 소켓구조체(상자)에 담아주는거임. 커널에 등록하기위해
	server_address.sin_family = AF_INET;	// IPv4 인터넷 프로토콜 
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);	// 32bit IPv4 주소 
	server_address.sin_port = htons(port_num);		// 사용할 port번호
	server_len = sizeof(server_address);		// 구조체의크기

	// bind() : socket에 server socket에 필요한 정보를 할당하고 커널에 등록
	// ( 커널에 등록해야 다른 시스템과 통신할 수 있는 상태가 됨)
	// (전화번호 부여)
	if( bind(server_sockfd, (struct sockaddr*)&server_address, server_len) == -1 ){
		printf("bind() error!\n");
		printf("다른포트로 연결\n");
		server_address.sin_port = htons(++port_num);
		if( bind(server_sockfd, (struct sockaddr*)&server_address, server_len) == -1 ){
			printf("bind() error!\n");
			exit(1);
		}
	}

	// listen() : client의 접속 요청을 기다리도록 설정
	// (전화 기다림)
	if( listen(server_sockfd, 5) == -1){
		printf("listen() error!\n");
		exit(1);
	}else
		printf("client 접속 요청 대기\n");

	// accept() : client의 접속 요청을 받아들이고, client와 통신하는 전용 소켓 생성
	// client socket식별자 반환
	// (전화받기)
	client_len = sizeof(client_address);
	client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_address, &client_len);
	if( client_sockfd == -1 ){
		printf("client 연결 수락 실패!\n");
		exit(1);
	}else
		printf("client와 연결\n");

	// data 통신(채팅)
	// multi-thread 이용
	pthread_t rt;	
	pthread_t wt;	

	if( pthread_create(&rt, NULL, read_function, &client_sockfd) != 0)
		printf("read thread 생성 실패!\n");
	if( pthread_create(&wt, NULL, write_function, &client_sockfd) != 0)
		printf("write thread 생성 실패!\n");

	if( pthread_join(rt, NULL) == 0)
		printf("read thread join\n");
	if( pthread_join(wt, NULL) == 0)
		printf("write thread join\n");

	// client socket 소멸 (data 통신 종료)
	if( close(client_sockfd) == 0)
		printf("client 소켓 통신 종료\n");

	exit(0);
}

void* read_function(void* sockfd){
	char buf_read[BUFF_SIZE+5];
	while(1){
		// read() : 전송된 자료 읽기
		// 만약 전송된 자료가 없으면 송신할 때까지 대기
/* -> */	read(*(int*)sockfd, buf_read, BUFF_SIZE);

		// exit thread
		if( strcmp(buf_read, "end") == 0)
			pthread_exit(NULL);
		
		// 읽은거 출력
		printf("client: %s\n", buf_read);
	}
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
		if( strcmp(buf_write, "end") == 0)
			pthread_exit(NULL);
	}
}

