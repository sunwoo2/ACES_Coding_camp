#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* writeo (void* arg);
void* writex ();

int main(){
	pthread_t thread1;
	pthread_t thread2;
	
	if( pthread_create(&thread1, NULL, writeo, NULL) ){	// 스레드 변수, 옵션, 동작 함수, 스레드 입력값 (create함수 인수는 싹 다 포인터)
		fprintf(stderr, "Failed to create thread1\n");
		exit(EXIT_FAILURE);
	}
	
	if( pthread_create(&thread2, NULL, writex, NULL) ){	
		printf("Failed to create thread2\n");
		return 0;	
	}

	pthread_join(thread1, NULL);	// 스레드 thread1 종료 기다림. 리턴값은 NULL로 지정
	pthread_join(thread2, NULL);	

	exit(EXIT_SUCCESS);
}

void* writeo (void* arg){
	char op_char = 'O';
	int pause_time;
	srand( (unsigned int) getpid() );

	sleep(2);

	for(int i=0; i<10; i++){
		printf("%c", op_char);
		fflush(stdout);		// stdout 버퍼 비워주기
		pause_time = rand() % 3;
		sleep(pause_time);
	}
}

void* writex (){
	char op_char = 'X';
	int pause_time;
	srand( (unsigned int) getpid() );

	sleep(2);

	for(int i=0; i<10; i++){
		printf("%c", op_char);
		fflush(stdout);		// stdout 버퍼 비워주기
		pause_time = rand() % 3;
		sleep(pause_time);
	}
}
