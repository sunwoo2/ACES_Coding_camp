#include <stdio.h>
#include <string.h>

// 두개의 파일이 동일한지 비교

int main(int argc, char* argv[]){
	FILE *fp1, *fp2;
	int state1, state2;
	char a,b;
	char f1[100];
	char f2[100];

	if(argc != 3){
		printf("파일을 두개 입력하세요!\n");
		return -1;
	}

	fp1 = fopen(argv[1], "r");
	fp2 = fopen(argv[2], "r");

	if(fp1 == NULL || fp2 == NULL){
		printf("스트림 생성시 오류발생\n");
		return -1;
	}


	while(1){
		fgets(f1, sizeof(f1), fp1);
		fgets(f2, sizeof(f2), fp2);

		if( strcmp(f1,f2)!=0){
			printf("파일이 일치하지 않음\n");
			break;
		}
		if(feof(fp1)!=0){
			printf("파일이 일치함\n");
			break;
		}
	}





	// 비교 시작
/*	while(1){
		// 두개의 파일 모두 끝에 도달하지 않을 경우
		if( feof(fp1)==0 && feof(fp2)==0){
			a = fgetc(fp1);
			b = fgetc(fp2);

			if(a != b){
				printf("두개의 파일이 일치하지 않습니다.\n");
				break;
			}
		}else if( feof(fp1)!=0 && feof(fp2)==0){	// 하나의 파일만 끝에 도달할 경우
			printf("두개의 파일이 일치하지 않습니다.\n");
			break;
		}else if( feof(fp1)==0 && feof(fp2)!=0){	// 하나의 파일만 끝에 도달할 경우
			printf("두개의 파일이 일치하지 않습니다.\n");
			break;
		}else{
			printf("두개의 파일이 일치합니다.\n");
			break;
		}
	}
*/
	return 0;

}
