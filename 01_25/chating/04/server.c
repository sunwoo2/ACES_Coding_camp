#include "chat.h"

int main(){
	int			server_sockfd, client_sockfd;
	int			server_len, client_len;
	struct sockaddr_in	server_address;
	struct sockaddr_in	client_address;
	int			result;
	fd_set			readfds, testfds;

	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(20000);
	server_len = sizeof(server_address);

	bind(server_sockfd, (struct sockaddr*)&server_address, server_len);

	listen(server_sockfd, 5);

	FD_ZERO(&readfds);
	FD_SET(server_sockfd, &readfds);

	while(1){
		char ch;
		int fd, nread;

		testfds = readfds;

		printf("server waiting\n");
		result = select(FD_SETSIZE, &testfds, (fd_set*)0, (fd_set*)0, (struct timeval*)0);

		if(result < 1){			// 종료
			perror("server5");
			exit(1);
		}
		for(fd=0; fd<FD_SETSIZE; fd++){
			if(FD_ISSET(fd, &testfds)){
				if(fd==server_sockfd){
					client_len = sizeof(client_address);
					client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_address, &client_len);
					FD_SET(client_sockfd, &readfds);
					printf("adding client on fd%d\n", client_sockfd);
				}else{
					ioctl(fd, FIONREAD, &nread);

					if(nread==0){
						close(fd);
						FD_CLR(fd, &readfds);
						printf("removing client on fd %d\n", fd);
					}else{
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
//						read(fd, &ch, 1);
//						sleep(5);
//						printf("serving client on fd %d\n", fd);
//						ch++;
//						write(fd, &ch, 1);
					}
				}
			}
		}
	}
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

