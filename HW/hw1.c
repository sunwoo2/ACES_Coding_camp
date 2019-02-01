/*
ID: hrkim
LANG: C
TASK: ride 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int stn(char s[]){
	int n=1;

	for(int i=0; i<6 && s[i]!='\0'; i++)
		n *= s[i] - 64;
	
	return n;
};

int main(void)
{
	FILE *fin, *fout;
	char comet[7], group[7];

	fin = fopen("ride.in", "r");
	fout = fopen("ride.out", "w");

	assert((fin != NULL) && (fout != NULL)); // stop program if condition is false

	fscanf(fin, "%s", comet);
	fscanf(fin, "%s", group);

	if((stn(comet)%47) == (stn(group)%47))
		fprintf(fout, "GO\n");
	else
		fprintf(fout, "STAY\n");


//	fprintf(fout, "%d\n", max_num);

	fclose(fin);
	fclose(fout);

	exit(0);
}
