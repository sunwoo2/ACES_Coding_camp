#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem;

void* writeo (void* arg);
void* writex (void* arg);

int main(){
	pthread_t	thread_1;	
	pthread_t 	thread_2;	
	void* 		thread_result;

	if( sem_init(&sem, 0, 1) ){
		fprintf(stderr, "Failed to initialize sem\n");
		exit(EXIT_FAILURE);
	}

	if( pthread_create(&thread_1, NULL, writeo, NULL)){	// 성공할경우 0을 리턴
		fprintf(stderr, "Failed to create thread\n");
		exit(EXIT_FAILURE);
	}
	
	if( pthread_create(&thread_2, NULL, writex, NULL)){
		fprintf(stderr, "Failed to create thread\n");
		exit(EXIT_FAILURE);
	}

	pthread_join(thread_1, &thread_result);
	pthread_join(thread_2, &thread_result);

	sem_destroy(&sem);

	exit(EXIT_SUCCESS);
}

void* writeo (void* arg){
	int i = 0;
	char op_char = 'O';
	int pause_time;
	srand( (unsigned int) getpid() );

	sleep(2);

	for(i=0; i<10; i++){
		sem_wait(&sem);

		printf("%c", op_char);
		fflush(stdout);		// stdout 버퍼를 비워준다
		pause_time = rand() % 3;
		sleep(pause_time);
		printf("%c", op_char);
		fflush(stdout);
		sem_post(&sem);

		pause_time = rand() % 3;
		sleep(pause_time);
	}

}

void* writex (void* arg){
	int i = 0;
	char op_char = 'X';
	int pause_time;
	srand( (unsigned int) getpid() );
	
	sleep(2);

	for(i=0; i<10; i++){
		sem_wait(&sem);
		printf("%c", op_char);
		fflush(stdout);
		pause_time = rand() % 3;
		sleep(pause_time);
		printf("%c", op_char);
		fflush(stdout);
		sem_post(&sem);

		pause_time = rand() % 3;
		sleep(pause_time);
	}
}
