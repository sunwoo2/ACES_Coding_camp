#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define BUFF_SIZE 1024	// 128바이트 

void* read_function(void* sockfd);
void* write_function(void* sockfd);
