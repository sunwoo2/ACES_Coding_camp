#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(){
	int res = mkfifo("my_fifo", 0744);
	if(res == 0)		// check error
		printf("Fifo created\n");
	
	exit(EXIT_SUCCESS);
}
