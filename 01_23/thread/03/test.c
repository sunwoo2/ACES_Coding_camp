#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

int sum = 0;
sem_t sem1;

void* thread_function(void* arg){
	for(int i=0; i<1000000; i++){
		sem_wait(&sem1);
		sum -= 1;
		sem_post(&sem1);
	}
}

int main(){


	sem_init(&sem1, 0, 1);	// 메모리 할당

	pthread_t pthread1;

	pthread_create(&pthread1, NULL, thread_function, NULL);

	for(int i=0; i<1000000; i++){
		sem_wait(&sem1);
		sum += 1;
		sem_post(&sem1);
	}

	pthread_join(pthread1, NULL);

	printf("result: %d\n", sum);

	sem_destroy(&sem1);		// 메모리 수거

	sleep(1);

	return 0;
}
