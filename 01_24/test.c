#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(){
	int i=1;
	int j=2;

	for(int n=0; n<20; n++){
		printf("%d ", i);
		printf("%d ", j);
	}
		printf("%d ", i-j);

	return 0;
}
