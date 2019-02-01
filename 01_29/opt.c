#include <stdio.h>

int main(){
	int r0 = 0;	// flag register
	int r1 = 1;	// i--	
	int r2 = 8;	

	int r3 = 1;	// start fib
	int r4 = 0;	// 현재값 들어갈 변수

BRANCH1:
	if(r2 == 0)	
		goto BRANCH;	

	r4 += r3;
	r3 += r4;

	r2 -= r1;
	if(r0==0)
		goto BRANCH1;

BRANCH:	
	
	printf("%d %d\n", r3, r4); 

	return 0;
}
