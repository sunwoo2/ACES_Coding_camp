/*
ID: hrkim
LANG: C
TASK: puppy
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main(void)
{
	FILE *fin, *fout;

	fin = fopen("puppy.in", "r");		
	fout = fopen("puppy.out", "w");

	assert((fin != NULL) && (fout != NULL)); // stop program if condition is false

//	fscanf(fin, "%d", &size);


	int test_case;
	fscanf(fin, "%d", &test_case);

	for(int i=0; i<test_case; i++){
		int N, K;
		int coin;
		fscanf(fin, "%d %d", &N, &K);
		coin = N%K;
		fprintf(fout, "%d\n", coin);
	}




//	fprintf(fout, "%d\n", max_num);

	fclose(fin);
	fclose(fout);

	exit(0);
}
