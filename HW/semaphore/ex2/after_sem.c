#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem1;	// 깃발 올린걸로 초기화(0으로) (사용 불가)
sem_t sem2;	// 깃발 내린걸로 초기화(1로) (사용가능)

void* writeo (void* arg);
void* writex ();

int main(){
	pthread_t thread1;
	pthread_t thread2;
	
	if( sem_init(&sem1, 0, 0) ){	// sem 변수, 프로세스간 공유X(현재프로세스에서만 사용), 초기값 1(스레드 실행가능)
		fprintf(stderr, "Failed to create thread\n");
		exit(EXIT_FAILURE);
	}
	
	if( sem_init(&sem2, 0, 1) ){	
		fprintf(stderr, "Failed to create thread\n");
		exit(EXIT_FAILURE);
	}

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

	sem_destroy(&sem1);	// sem객체 삭제, 할당된 메모리 수거
	sem_destroy(&sem2);	

	exit(EXIT_SUCCESS);
}

void* writeo (void* arg){
	char op_char = 'O';
	int pause_time;
	srand( (unsigned int) getpid() );

	sleep(2);

	for(int i=0; i<10; i++){
		sem_wait(&sem2);	// sem2 깃발 듬 	(깃발들고있으면 다른스레드가 사용 못함. 깃발이 내려가있으면 세마포 사용가능)

		printf("%c", op_char);
		fflush(stdout);		// stdout 버퍼 비워주기
		pause_time = rand() % 3;
		sleep(pause_time);

		sem_post(&sem1);	// sem1 깃발 내려줌
	}
}

void* writex (){
	char op_char = 'X';
	int pause_time;
	srand( (unsigned int) getpid() );

	sleep(2);

	for(int i=0; i<10; i++){
		sem_wait(&sem1);	// sem1 깃발 듬	

		printf("%c", op_char);
		fflush(stdout);	
		pause_time = rand() % 3;
		sleep(pause_time);

		sem_post(&sem2);	// sem2 깃발 내려줌	
	}
}

// 그러니깐 지할일할때 남의 깃발 들어서 남일 못하게 만들고 지 할일 다하고 다시 깃발 내려주고 이런식으로 세마포어 두개 사용하는 원리
