/*
ID: hrkim
LANG: C
TASK: friday
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main(void)
{
	FILE *fin, *fout;
	char comet[7], group[7];

	fin = fopen("friday.in", "r");
	fout = fopen("friday.out", "w");

	assert((fin != NULL) && (fout != NULL)); // stop program if condition is false

	fscanf(fin, "%s", comet);
	fscanf(fin, "%s", group);


//	fprintf(fout, "%d\n", max_num);

	fclose(fin);
	fclose(fout);

	exit(0);
}
