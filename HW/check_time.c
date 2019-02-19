/*
ID: hrkim
LANG: C
TASK: friday
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>

double wtime();

int main(void){
	double start_time;
	double finish_time;
	int n;

	start_time = wtime();
	for(int i=0; i<10000; i++){
		n += i;
		printf("%d ", n);
	}
	printf("\n");
	finish_time= wtime() - start_time;

	printf("run_time: %f seconds\n",  finish_time);

	return 0;
}
