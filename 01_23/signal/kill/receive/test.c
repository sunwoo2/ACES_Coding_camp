#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

// SIGINT handler function
void signal_handling(){
	printf("catch SIGINT!!\n");
}

int main(){

	// sigaction structure generated
	struct sigaction sa1;	
	// initialize
	sa1.sa_handler = &signal_handling;	// handle registered
	// block every signal during the handler
	sigfillset(&sa1.sa_mask);		

	// connection
	sigaction(SIGINT, &sa1, NULL);		// signal번호랑 연결

	pid_t pid = getpid();
	printf("my_pid: %d\n", pid);

	for(int i=0; i<100; i++){
		printf("count: %d\n", i);
		sleep(1);
	}

	return 0;
}
