#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE *fp;
	char filename[80];
	
	printf("열고 싶은 파일명을 입력하세요.");
	gets(filename);

	fp = fopen(filename, "r");
	if(!fp){
		printf("\n열기 에러: %s 파일이 있는지 확인해보세요.\n", filename);
		exit(1);
	}
	printf("\n파일을 읽기 모드로 성공적으로 열었습니다.");
	fclose(fp);
	printf("\n파일을 닫고 프로그램을 종료합니다.\n");

	return 0;
}
