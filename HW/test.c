/*
ID: hrkim
LANG: C
TASK: friday
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

int main(void){
	char s[] = "hello";

	int i=0;
	while(s[i] != '\0'){
		printf("%c %d\n", s[i], i);
		i++;
	}
		printf("%s\n", s);
}
