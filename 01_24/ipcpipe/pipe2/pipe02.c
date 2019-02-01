#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(){
	char buffer[BUFSIZ +1];
	
	sprintf(buffer, "Once upon a time, there was...\n");
	printf("%s", buffer);	
	exit(EXIT_SUCCESS);
}
