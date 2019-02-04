/*
ID: hrkim
LANG: C
TASK: dualpal
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>

void dtoB(int num, char* a, int B){	// 2~10진법 변환
	for(int i=0; (i<32)&&(num!=0); i++){
		// 한칸씩 오른쪽으로
		for(int j=i; j>0; j--)
			a[j] = a[j-1];
		a[0] = (num%B)+48; 	
		num = num/B;
	}
};

bool pal(int num){	// 2~10진수 에서 적어도 2개의 진법에서 회문이면 true 반환 
	short check=0;
	short g;
	char a[32];
	for(int i=2; i<11; i++){
		memset(a, 0, sizeof(a));	// 문자열 초기화!!!(간단하게 문자열 메모리를 초기화 해주면댐)
		dtoB(num, a, i);
		g = strlen(a);
		for(int j=1; j<=g+1; j++){
			if( j==g || j>g){	// 길이가 홀수,짝수 모두 고려
				check++;
				if(check==2)
					return true;
			}
			if(a[j-1] != a[g-1])
				break;
			g--;
		}
	}
	return false;
};

int main(void)
{
	FILE *fin, *fout;

	fin = fopen("dualpal.in", "r");		
	fout = fopen("dualpal.out", "w");

	assert((fin != NULL) && (fout != NULL)); // stop program if condition is false

//	fscanf(fin, "%d", &size);



	// data 입력
	int N;
	fscanf(fin, "%d\n", &N);
	unsigned int S;
	fscanf(fin, "%d\n", &S);
	S++;

	
	int ch=0;
	while(1){
		if(pal(S)){
			fprintf(fout, "%d\n", S);
			ch++;
			if(ch==N)
				break;
		}
		S++;
		if(S>10000)	// 혹시 무한루프 예방
			break;
	}









//	fprintf(fout, "%d\n", max_num);

	fclose(fin);
	fclose(fout);

	exit(0);
}
