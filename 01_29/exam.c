#include <stdio.h>

int fib(int n);		// 피보나치수열의 n항 리턴

int main(){
	int r0 = 1;
	int r1 = 0;
	int r2 = -5;	
	int r3 = 0;	// flag register
	int r4 = 1;	
	int r5 = 10;	// 위의 레지스터값일때, 피보나치수열의 n항
	int r6 = 0;	// 현재값 들어갈 변수

	int m0=0;
	int m1=0;

	r5 -= r2;



// recursive fibonacci
//	printf("%d\n", fib(r5));




// while loop
/*
	while(r3 == 0){
		if(r5==0)
			break;
		r6 = r0;
		r6 += r1;
		r1 = r0;
		r0 = r6;

		r5 -= r4;
	}
*/




// goto
/*	printf("%d ", r0);
BRANCH1:
	if(r5 == 0)	
		goto BRANCH;	
	m0 = r0;
	r6 = m0;	
	r6 += r1;
	m1 = r0;	
	r1 = m1;	
	m0 = r6;	
	r0 = m0;
	r5 -= r4;
	if(r3==0)	
		goto BRANCH1;

	r0 = m0;
BRANCH:	
*/
//	printf("\n");
//	printf("%d %d %d %d %d %d %d\n", r0, r1,r2,r3,r4,r5,r6); 
//	printf("%d %d\n", m0, m1);

	for(int i=0; i<30; i++)
		printf("피보나치%d항: %d\n", i, fib(i));

	return 0;
}

int fib(int n){		
	if(n<2)
		return 1;
	return fib(n-1) + fib(n-2);
}

