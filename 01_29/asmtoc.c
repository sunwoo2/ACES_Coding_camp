#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "sim.h"

void asm_to_c(char buffer[], int line);

int main(){

	int line=0; 	// 현재 program의 몇번째줄에서 연산 중인지 보기위해
	char buffer[100];

	FILE *fp = fopen("asm.s", "r");

	while(1){
		if( fgets(buffer, sizeof(buffer), fp) == NULL){
//			printf("파일 읽기 끝!\n");
			break;
		}

//		printf("%s", buffer);		// original assembly

		asm_to_c(buffer, line);		// assembly -> C

		line++;		


		// 문자열 자르기
//		s = strtok(buffer, " ");
//		printf("%s\n", s);
//		s = strtok(NULL, " ");
//		printf("%s\n", s);
//		s = strtok(NULL, " ");
//		printf("%s", s);
	
	}

	return 0;
}

void asm_to_c(char buffer[], int line){

	int j=0;
	int OP1, OP2;
	char* s;
	char num[5];

	s = strtok(buffer, " ");

	if(strcmp(s, "MOV0") == 0){	
//		printf("MOV0 처리 시작\n");
		s = strtok(NULL, " ");		// OP1부분
		for(int i=1; isdigit(s[i])!=0; i++)	// 숫자가아니면 0반환
			num[i-1] = s[i];
		OP1 = atoi(num);		// OP1 숫자로 변환
		while(num[j] != '\0')	// 배열 비우기
			num[j++] = '\0';

		s = strtok(NULL, " ");		// OP2부분
		for(int i=0; isdigit(s[i])!=0; i++)
			num[i] = s[i];
		OP2 = atoi(num);		// OP2 숫자로 변환
		while(num[j] != '\0')	
			num[j++] = '\0';

		// C문법으로 출력
		printf("program[%d] : regs[%d] = memory[%d]\n", line, OP1, OP2);

	}else if(strcmp(s, "MOV1") == 0){
		s = strtok(NULL, " ");		
		for(int i=0; isdigit(s[i])!=0; i++)
			num[i] = s[i];
		OP1 = atoi(num);		
		while(num[j] != '\0')	
			num[j++] = '\0';

		s = strtok(NULL, " ");	
		for(int i=1; isdigit(s[i])!=0; i++)
			num[i-1] = s[i];
		OP2 = atoi(num);
		while(num[j] != '\0')	
			num[j++] = '\0';

		printf("program[%d] : memory[%d] = regs[%d]\n",line, OP1, OP2);
	}else if(strcmp(s, "MOV2") == 0){
		s = strtok(NULL, " ");		
		for(int i=2; isdigit(s[i])!=0; i++)
			num[i-2] = s[i];
		OP1 = atoi(num);		
		while(num[j] != '\0')	
			num[j++] = '\0';

		s = strtok(NULL, " ");	
		for(int i=1; isdigit(s[i])!=0; i++)
			num[i-1] = s[i];
		OP2 = atoi(num);
		while(num[j] != '\0')	
			num[j++] = '\0';

		printf("program[%d] : memory[regs[%d]] = regs[%d]\n",line, OP1, OP2);
	}else if(strcmp(s, "MOV3") == 0){
		s = strtok(NULL, " ");		
		for(int i=1; isdigit(s[i])!=0; i++)
			num[i-1] = s[i];
		OP1 = atoi(num);		
		while(num[j] != '\0')	
			num[j++] = '\0';

		s = strtok(NULL, " ");	
		for(int i=1; isdigit(s[i])!=0; i++)
			num[i-1] = s[i];
		OP2 = atoi(num);
		while(num[j] != '\0')	
			num[j++] = '\0';

		printf("program[%d] : regs[%d] = %d\n",line,  OP1, OP2);
	}else if(strcmp(s, "ADD") == 0){
		s = strtok(NULL, " ");		
		for(int i=1; isdigit(s[i])!=0; i++)
			num[i-1] = s[i];
		OP1 = atoi(num);		
		while(num[j] != '\0')	
			num[j++] = '\0';

		s = strtok(NULL, " ");	
		for(int i=1; isdigit(s[i])!=0; i++)
			num[i-1] = s[i];
		OP2 = atoi(num);
		while(num[j] != '\0')	
			num[j++] = '\0';

		printf("program[%d] : regs[%d] += regs[%d]\n", line, OP1, OP2);
	}else if(strcmp(s, "SUB") == 0){
		s = strtok(NULL, " ");		
		for(int i=1; isdigit(s[i])!=0; i++)
			num[i-1] = s[i];
		OP1 = atoi(num);		
		while(num[j] != '\0')	
			num[j++] = '\0';

		s = strtok(NULL, " ");	
		for(int i=1; isdigit(s[i])!=0; i++)
			num[i-1] = s[i];
		OP2 = atoi(num);
		while(num[j] != '\0')	
			num[j++] = '\0';

		printf("program[%d] : regs[%d] -= regs[%d]\n",line, OP1, OP2);
	}else if(strcmp(s, "JZ") == 0){
		s = strtok(NULL, " ");		
		for(int i=1; isdigit(s[i])!=0; i++)
			num[i-1] = s[i];
		OP1 = atoi(num);		
		while(num[j] != '\0')	
			num[j++] = '\0';

		s = strtok(NULL, " ");	
		for(int i=1; isdigit(s[i])!=0; i++)
			num[i-1] = s[i];
		OP2 = atoi(num);
		while(num[j] != '\0')	
			num[j++] = '\0';

		printf("program[%d] : \n", line);
		printf("\t\tif(regs[%d] == 0)\n", OP1);
		printf("\t\t\tpc += %d\n", OP2);
	}
}
