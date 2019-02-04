/*
ID: hrkim
LANG: C
TASK: palsquare
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

void dtoB(int num, char* a, int B){	// decimal -> 2~20진법 변환
	int d;
	for(int i=0; (i<60)&&(num!=0); i++){	// 60비트(배열크기) 가정
		// 한칸씩 오른쪽으로
		for(int j=i; j>0; j--)
			a[j] = a[j-1];
		d = num%B;	// B로 나눈 나머지
		if(d<10)	// 2~10진수
			a[0] = d+48; 	
		else	// 11~20진수
			a[0] = d+55;
		num = num/B;
	}
};

int main(void)
{
	FILE *fin, *fout;

	fin = fopen("palsquare.in", "r");		
	fout = fopen("palsquare.out", "w");

	assert((fin != NULL) && (fout != NULL)); // stop program if condition is false

//	fscanf(fin, "%d", &size);



	// data 입력
	int B;
	fscanf(fin, "%d\n", &B);



	char a[60];
	char b[60];
	int g;

	// B진법 회문 찾기
	for(int i=1; i<=300; i++){
		dtoB(i*i, b, B);
		g = strlen(b);
		for(int j=1; j<=g+1; j++){
			if( j==g || j>g){	// 길이가 홀수,짝수 모두 고려
				dtoB(i, a, B);
				//fprintf(fout, "%s %s\n", a, b);
				for(int i=0; i<strlen(a); i++)
					fprintf(fout, "%c", a[i]);
				fprintf(fout, " ");
				for(int i=0; i<strlen(b); i++)
					fprintf(fout, "%c", b[i]);
				fprintf(fout, "\n");
				break;
			}
			if(b[j-1] != b[g-1])
				break;
			g--;
		}
	}







//	fprintf(fout, "%d\n", max_num);

	fclose(fin);
	fclose(fout);

	exit(0);
}
