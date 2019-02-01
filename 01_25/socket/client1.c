#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	int sockfd;
	int len;
	struct sockaddr_un address;
	int result;
	char ch = 'A';

	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, "server_socket");
	// 여기서는  IP안쓰고 server_socket이라는 파일로 통신하는거임
	len = sizeof(address);

	result = connect(sockfd, (struct sockaddr*)&address, len);
	// connect : 서버에 접속

	if(result == -1){
		perror("oops: client1");
		exit(1);
	}
	while(1){
		//fscanf(stdin,"%c", &ch);
		write(sockfd, &ch, 1);
		read(sockfd, &ch, 1);		// server ->
		printf("char from server = %c\n", ch);
		close(sockfd);
	}
		exit(0);
}
