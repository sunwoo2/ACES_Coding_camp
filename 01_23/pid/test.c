#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
	pid_t pid;
	pid_t ppid;

	pid = getpid();
	ppid = getppid();

	printf("pid: %d\n", pid);
	printf("ppid: %d\n", ppid);

	return 0;
}
