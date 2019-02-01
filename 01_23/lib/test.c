#include <stdio.h>
#include "sub.h"
#include "sum.h"

int main(){
	int a=15;
	int b=5;

	int c = sum(a,b);
	printf("value: %d\n", c);
	c = sub(a,b);
	printf("value: %d\n", c);

	return 0;
}
