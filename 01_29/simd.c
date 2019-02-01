#include <stdio.h>

int main(){
	int r0 = 0;	// flag register
	int r1 = 1;	// i--	
	int r2 = 2;	// 반복 횟수

	int R3[6] = {1,1,2,3,5,8};
	int R9[6] = {1,2,3,5,8,13};	

BRANCH1:
	if(r2 == 0)	
		goto BRANCH;	

	for(int i=0; i<6; i++)
		R3[i] += R9[i];
	for(int i=0; i<6; i++)
		R9[i] += R3[i];
	for(int i=0; i<6; i++)
		R3[i] += R9[i];
	for(int i=0; i<6; i++)
		R9[i] += R3[i];
	for(int i=0; i<6; i++)
		R3[i] += R9[i];
	for(int i=0; i<6; i++)
		R9[i] += R3[i];

	r2 -= r1;
	if(r0==0)
		goto BRANCH1;

BRANCH:	
	return 0;

	for(int i=0; i<6; i++)
		printf("%d ", R9[i]); 
	printf("\n");

	return 0;
}
