#include <signal.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

// ./test [SIGNUM] [PID]

int main(int argc, char* argv[]){
	if(argc < 3){
		printf("[Usage] kill [SIGNUM] [PID]\n");
		exit(1);
	}

	int signum = atoi(argv[1]);
	int pid = atoi(argv[2]);

	kill(pid, signum);
	//kill(pid, SIGKILL);
}
