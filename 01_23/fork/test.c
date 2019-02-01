#include <stdio.h>
#include <unistd.h>
//#include <sys/types.h>

int main(){
	pid_t pid;

	pid = fork();		// completely copy this program

	if(pid>0){		// distinguish parent and child
		printf("[parent] hello world!\n");
	}
	else if(pid==0){
		printf("[child] hello world!\n");
	}
	else{
		printf("fork failed\n");
	}

	return 0;
}
