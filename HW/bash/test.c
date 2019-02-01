#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <wait.h>

void signal_handling(){
	printf("catch SIGINT!!\n");
}

void do_process(char* inst){
	pid_t pid  = fork();
	
	struct sigaction sa1;	

	char path[] = "/bin/";

	if(pid > 0){		// parent process(test)
		sa1.sa_handler = &signal_handling;	// handle registered
		sigfillset(&sa1.sa_mask);		
		sigaction(SIGINT, &sa1, NULL);		// signal번호랑 연결
		// wait함수는 무조건 써야되긴함. 메모리 관점에서 봐도
		wait(NULL);		// 부모 프로세스가 자식 프로세스가 끝날때까지 기다림
	}
	else if(pid == 0){	// child process (inst -> ls, pwd)
		// inst -> ls, pwd

		strcat(path, inst);
		execl(path, inst, NULL);
	}
	else{
		printf(" fork failed\n");
	}
}

int main(){
	char inst[20];
	
	while(1){		// 배쉬의 입력 프로그램 따라한거
		printf(" input>> ");
		scanf("%s", inst);
		printf(" instruction: %s\n", inst);

		if(!strcmp(inst, "ls")){		// strcmp : 두문자열을 비교해서 같으면 0을 반환
			printf(" ls starts\n");				
			do_process(inst);
		}
		else if(!strcmp(inst, "pwd")){
			printf(" pwd starts\n");				
			do_process(inst);
		}else if(!strcmp(inst, "counting_program")){
			printf(" counting_program starts\n");
			do_process(inst);
		}else{
			printf(" unknown instruction\n");
		}

	}
	
	return 0;
}
