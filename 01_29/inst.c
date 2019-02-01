#include <stdio.h>

int main(){
	int r0 = 0;
	int r1 = 1;
	int r2 = 15;
	int r3 = 1;	
	int r4 = 0;
	int r5 = 0;

BRANCH1:
	if(r2 == 0)
		goto BRANCH;	
	r5 = r3;	
	r5 += r4;
	r4 = r3;
	r3 = r5;	
	
	r2 -= r1;
	if(r0==0)	
		goto BRANCH1;

BRANCH:	

	printf("\n");
	printf("%d %d\n", r3, r4); 

	return 0;
}
