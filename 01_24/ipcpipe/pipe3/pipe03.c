#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//#define PRINT		// 디버깅용 printf, 디버깅 끝나면 주석처리해서 안보이게

int main(){
	int data_processed;
	int file_pipes[2];
	const char some_data[] = "123";
	char buffer[ BUFSIZ + 1];

	memset(buffer, '\0', sizeof(buffer));

	if(pipe(file_pipes) == 0){
		data_processed = write(file_pipes[1], some_data, strlen(some_data));
#ifdef PRINT
		printf("Wrote %d bytes\n", data_processed);
#endif
		data_processed = read(file_pipes[0], buffer, BUFSIZ);
#ifdef PRINT
		printf("Read %d bytes: %s\n", data_processed, buffer);
#endif
		exit(EXIT_SUCCESS);		// same 'return 0'
	}
	exit(EXIT_FAILURE);		// same 'return -1'
}
