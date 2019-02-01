#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "sim.h"

void asm_to_bin(char buffer[], int line);

int main(int argc, char* argv[]){

	int line=0; 	// 현재 program의 몇번째줄에서 연산 중인지 보기위해
	char buffer[100];

	if(argc != 2){
		printf("binary로 변환할 assemly파일을 입력하시오!\n");
		return 0;
	}

	FILE *fp = fopen(argv[1], "r");

	while(1){
		if( fgets(buffer, sizeof(buffer), fp) == NULL){
			break;
		}

//		printf("%s", buffer);		// original assembly

		asm_to_bin(buffer, line);		// assembly -> C

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

void asm_to_bin(char buffer[], int line){

	int j=0;
	int OP1, OP2;
	char* s;
	char num[5];
	char bin[4] = "0000";
	char bin1[8] = "00000000";

	s = strtok(buffer, " ");

	if(strcmp(s, "MOV0") == 0){	
//		printf("MOV0 처리 시작\n");
		s = strtok(NULL, " ");		// OP1부분
		for(int i=1; isdigit(s[i])!=0; i++)	// 숫자가아니면 0반환
			num[i-1] = s[i];
		OP1 = atoi(num);		// OP1 int형으로 변환
		for(int i=3; i>=0 && OP1!=0; i--){		// OP1 2진수로 변환
			if( OP1%2 == 1)
				bin[i] = '1';
			OP1 /= 2;
		}
		while(num[j] != '\0')	// 배열 비우기
			num[j++] = '\0';

		s = strtok(NULL, " ");		// OP2부분
		for(int i=0; isdigit(s[i])!=0; i++)
			num[i] = s[i];
		OP2 = atoi(num);		// OP2 숫자로 변환
		for(int i=7; i>=0 && OP2!=0; i--){		// OP2 2진수로 변환
			if( OP2%2 == 1)
				bin1[i] = '1';
			OP2 /= 2;
		}
		while(num[j] != '\0')	
			num[j++] = '\0';

		printf("0000");
		for(int i=0; i<4; i++){
			printf("%c", bin[i]);
		}
		for(int i=0; i<8; i++){
			printf("%c", bin1[i]);
		}
		printf("\n");
	//	printf("0000%s", bin);		// 이렇게 %s로 문자열 출력하면 맨뒤에 NULL까지 생각 해줘야함!!!!!!!!!
	//	printf("%s\n", bin1);
		strcpy(bin, "0000");
		strcpy(bin1, "00000000");

	}else if(strcmp(s, "MOV1") == 0){
		s = strtok(NULL, " ");		
		for(int i=0; isdigit(s[i])!=0; i++)
			num[i] = s[i];
		OP1 = atoi(num);		
		for(int i=7; i>=0 && OP1!=0; i--){
			if( OP1%2 == 1)
				bin1[i] = '1';
			OP1 /= 2;
		}
		while(num[j] != '\0')	
			num[j++] = '\0';

		s = strtok(NULL, " ");	
		for(int i=1; isdigit(s[i])!=0; i++)
			num[i-1] = s[i];
		OP2 = atoi(num);
		for(int i=3; i>=0 && OP2!=0; i--){
			if( OP2%2 == 1)
				bin[i] = '1';
			OP2 /= 2;
		}
		while(num[j] != '\0')	
			num[j++] = '\0';

		printf("0001");
		for(int i=0; i<4; i++){
			printf("%c", bin[i]);
		}
		for(int i=0; i<8; i++){
			printf("%c", bin1[i]);
		}
		printf("\n");
		strcpy(bin, "0000");
		strcpy(bin1, "00000000");

	}else if(strcmp(s, "MOV2") == 0){
		s = strtok(NULL, " ");		
		for(int i=2; isdigit(s[i])!=0; i++)
			num[i-2] = s[i];
		OP1 = atoi(num);		
		for(int i=3; i>=0 && OP1!=0; i--){		
			if( OP1%2 == 1)
				bin[i] = '1';
			OP1 /= 2;
		}
		while(num[j] != '\0')	
			num[j++] = '\0';

		s = strtok(NULL, " ");	
		for(int i=1; isdigit(s[i])!=0; i++)
			num[i-1] = s[i];
		OP2 = atoi(num);
		for(int i=3; i>=0 && OP2!=0; i--){	// 8비트중에 상위4비트에 정보 저장 되있음
			if( OP2%2 == 1)
				bin1[i] = '1';
			OP2 /= 2;
		}
		while(num[j] != '\0')	
			num[j++] = '\0';

		printf("0010");
		for(int i=0; i<4; i++){
			printf("%c", bin[i]);
		}
		for(int i=0; i<8; i++){
			printf("%c", bin1[i]);
		}
		printf("\n");
		strcpy(bin, "0000");
		strcpy(bin1, "00000000");

	}else if(strcmp(s, "MOV3") == 0){
		s = strtok(NULL, " ");		
		for(int i=1; isdigit(s[i])!=0; i++)
			num[i-1] = s[i];
		OP1 = atoi(num);		
		for(int i=3; i>=0 && OP1!=0; i--){		
			if( OP1%2 == 1)
				bin[i] = '1';
			OP1 /= 2;
		}
		while(num[j] != '\0')	
			num[j++] = '\0';

		s = strtok(NULL, " ");	
		if(s[1]=='-'){		// OP2가 음수값일때
			for(int i=2; isdigit(s[i])!=0; i++)
				num[i-2] = s[i];
			OP2 = atoi(num);	// 2의보수 고려해서 2진법으로
			OP2 = ~( (~255) | (OP2-1) );		// 2의보수 표현 back tracking, bit mask
			for(int i=7; i>=0 && OP2!=0; i--){	 
				if( OP2%2 == 1)
					bin1[i] = '1';
				OP2 /= 2;
			}
		}else{			// 양수 일때
			for(int i=1; isdigit(s[i])!=0; i++)
				num[i-1] = s[i];
			OP2 = atoi(num);
			for(int i=7; i>=0 && OP2!=0; i--){	
				if( OP2%2 == 1)
					bin1[i] = '1';
				OP2 /= 2;
			}
		}
		while(num[j] != '\0')	
			num[j++] = '\0';

		printf("0011");
		for(int i=0; i<4; i++){
			printf("%c", bin[i]);
		}
		for(int i=0; i<8; i++){
			printf("%c", bin1[i]);
		}
		printf("\n");
		strcpy(bin, "0000");
		strcpy(bin1, "00000000");

	}else if(strcmp(s, "MOV4") == 0){
		s = strtok(NULL, " ");		
		for(int i=1; isdigit(s[i])!=0; i++)
			num[i-1] = s[i];
		OP1 = atoi(num);		
		for(int i=3; i>=0 && OP1!=0; i--){
			if( OP1%2 == 1)
				bin[i] = '1';
			OP1 /= 2;
		}
		while(num[j] != '\0')	
			num[j++] = '\0';

		s = strtok(NULL, " ");	
		for(int i=1; isdigit(s[i])!=0; i++)
			num[i-1] = s[i];
		OP2 = atoi(num);
		for(int i=3; i>=0 && OP2!=0; i--){
			if( OP2%2 == 1)
				bin1[i] = '1';
			OP2 /= 2;
		}
		while(num[j] != '\0')	
			num[j++] = '\0';

		printf("0100");
		for(int i=0; i<4; i++){
			printf("%c", bin[i]);
		}
		for(int i=0; i<8; i++){
			printf("%c", bin1[i]);
		}
		printf("\n");
		strcpy(bin, "0000");
		strcpy(bin1, "00000000");

	}else if(strcmp(s, "ADD") == 0){
		s = strtok(NULL, " ");		
		for(int i=1; isdigit(s[i])!=0; i++)
			num[i-1] = s[i];
		OP1 = atoi(num);		
		for(int i=3; i>=0 && OP1!=0; i--){		
			if( OP1%2 == 1)
				bin[i] = '1';
			OP1 /= 2;
		}
		while(num[j] != '\0')	
			num[j++] = '\0';

		s = strtok(NULL, " ");	
		for(int i=1; isdigit(s[i])!=0; i++)
			num[i-1] = s[i];
		OP2 = atoi(num);
		for(int i=3; i>=0 && OP2!=0; i--){	
			if( OP2%2 == 1)
				bin1[i] = '1';
			OP2 /= 2;
		}
		while(num[j] != '\0')	
			num[j++] = '\0';

		printf("0101");
		for(int i=0; i<4; i++){
			printf("%c", bin[i]);
		}
		for(int i=0; i<8; i++){
			printf("%c", bin1[i]);
		}
		printf("\n");
		strcpy(bin, "0000");
		strcpy(bin1, "00000000");

	}else if(strcmp(s, "SUB") == 0){
		s = strtok(NULL, " ");		
		for(int i=1; isdigit(s[i])!=0; i++)
			num[i-1] = s[i];
		OP1 = atoi(num);		
		for(int i=3; i>=0 && OP1!=0; i--){		
			if( OP1%2 == 1)
				bin[i] = '1';
			OP1 /= 2;
		}
		while(num[j] != '\0')	
			num[j++] = '\0';

		s = strtok(NULL, " ");	
		for(int i=1; isdigit(s[i])!=0; i++)
			num[i-1] = s[i];
		OP2 = atoi(num);
		for(int i=3; i>=0 && OP2!=0; i--){	
			if( OP2%2 == 1)
				bin1[i] = '1';
			OP2 /= 2;
		}
		while(num[j] != '\0')	
			num[j++] = '\0';

		printf("0110");
		for(int i=0; i<4; i++){
			printf("%c", bin[i]);
		}
		for(int i=0; i<8; i++){
			printf("%c", bin1[i]);
		}
		printf("\n");
		strcpy(bin, "0000");
		strcpy(bin1, "00000000");

	}else if(strcmp(s, "JZ") == 0){
		s = strtok(NULL, " ");		
		for(int i=1; isdigit(s[i])!=0; i++)
			num[i-1] = s[i];
		OP1 = atoi(num);		
		for(int i=3; i>=0 && OP1!=0; i--){		
			if( OP1%2 == 1)
				bin[i] = '1';
			OP1 /= 2;
		}
		while(num[j] != '\0')	
			num[j++] = '\0';

		s = strtok(NULL, " ");	
		if(s[1]=='-'){		// OP2가 음수값일때
			for(int i=2; isdigit(s[i])!=0; i++)
				num[i-2] = s[i];
			OP2 = atoi(num);	// 2의보수 고려해서 2진법으로
			OP2 = ~( (~255) | (OP2-1) );		// 2의보수 표현 back tracking, bit mask
			for(int i=7; i>=0 && OP2!=0; i--){	 
				if( OP2%2 == 1)
					bin1[i] = '1';
				OP2 /= 2;
			}
		}else{			// 양수 일때
			for(int i=1; isdigit(s[i])!=0; i++)
				num[i-1] = s[i];
			OP2 = atoi(num);
			for(int i=7; i>=0 && OP2!=0; i--){	
				if( OP2%2 == 1)
					bin1[i] = '1';
				OP2 /= 2;
			}
		}
		while(num[j] != '\0')	
			num[j++] = '\0';

		printf("0111");
		for(int i=0; i<4; i++){
			printf("%c", bin[i]);
		}
		for(int i=0; i<8; i++){
			printf("%c", bin1[i]);
		}
		printf("\n");
		strcpy(bin, "0000");
		strcpy(bin1, "00000000");
	}else if(strcmp(s, "SIMD") == 0){
		s = strtok(NULL, " ");		
		for(int i=1; isdigit(s[i])!=0; i++)
			num[i-1] = s[i];
		OP1 = atoi(num);		
		for(int i=3; i>=0 && OP1!=0; i--){		
			if( OP1%2 == 1)
				bin[i] = '1';
			OP1 /= 2;
		}
		while(num[j] != '\0')	
			num[j++] = '\0';

		s = strtok(NULL, " ");	
		for(int i=1; isdigit(s[i])!=0; i++)
			num[i-1] = s[i];
		OP2 = atoi(num);
		for(int i=3; i>=0 && OP2!=0; i--){	
			if( OP2%2 == 1)
				bin1[i] = '1';
			OP2 /= 2;
		}
		while(num[j] != '\0')	
			num[j++] = '\0';

		printf("1000");
		for(int i=0; i<4; i++){
			printf("%c", bin[i]);
		}
		for(int i=0; i<8; i++){
			printf("%c", bin1[i]);
		}
		printf("\n");
		strcpy(bin, "0000");
		strcpy(bin1, "00000000");
	}
}
