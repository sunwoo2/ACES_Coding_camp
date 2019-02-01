#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int sum = 0;

void* thread_function(void* arg){
	for(int i=0; i<100000; i++){
		sum -= 1;
//		printf("[new thread] sum: %d\n", sum);
	}
}

int main(){

	pthread_t pthread1;

	pthread_create(&pthread1, NULL, thread_function, NULL);

	for(int i=0; i<1000000; i++){
		sum += 1;
//		printf("[main thread] sum: %d\n", sum);
	}

	pthread_join(pthread1, NULL);

	printf("result: %d\n", sum);

	sleep(1);

	return 0;
}
