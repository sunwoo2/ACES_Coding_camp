/*
ID: hrkim
LANG: C
TASK: namenum
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

int main(void)
{
	FILE *fin, *fout;

	fin = fopen("namenum.in", "r");		
	fout = fopen("namenum.out", "w");

	assert((fin != NULL) && (fout != NULL)); // stop program if condition is false

//	fscanf(fin, "%d", &size);















//	fprintf(fout, "%d\n", max_num);

	fclose(fin);
	fclose(fout);

	exit(0);
}
