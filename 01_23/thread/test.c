#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* thread_function(void* arg){
	printf("[new thread] hello world\n");
}

int main(){

	pthread_t pthread1;

	printf("hello world\n");
	pthread_create(&pthread1, NULL, thread_function, NULL);

	sleep(1);

	return 0;
}
