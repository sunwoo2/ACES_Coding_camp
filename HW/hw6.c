/*
ID: hrkim
LANG: C
TASK: transform
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

int main(void)
{
	FILE *fin, *fout;

	fin = fopen("transform.in", "r");		
	fout = fopen("transform.out", "w");

	assert((fin != NULL) && (fout != NULL)); // stop program if condition is false

//	fscanf(fin, "%d", &size);






	char a;
	int N;
	fscanf(fin, "%d", &N);
	fscanf(fin, "%c", &a);		// '\n' 처리용

	char before[N][N];
	char after[N][N];

	// data 입력
	for(int i=0; i<N; i++)
		for(int j=0; j<N+1; j++){
			fscanf(fin, "%c", &a);
			if(isspace(a))
				break;
			before[i][j] = a;
		}
	for(int i=0; i<N; i++)
		for(int j=0; j<N+1; j++){
			fscanf(fin, "%c", &a);
			if(isspace(a))
				break;
			after[i][j] = a;
		}

	// 입력받은 데이터 확인
/*	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			printf("%c", before[i][j]);
		}
		printf("\n");
	}
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			printf("%c", after[i][j]);
		}
		printf("\n");
	}
*/
	
	int temp;
	char c1[N][N];
	char c2[N][N];
	char c3[N][N];
	char c4[N][N]; 
	memcpy(c4, before, sizeof(before));
	char c5[N][N]; 
	memcpy(c5, before, sizeof(before));
	char c51[N][N];
	char c52[N][N];
	char c53[N][N];

	// 90도 회전
	for(int i=0; i<N; i++)
		for(int j=0; j<N; j++)
			c1[j][N-1-i] = before[i][j];

	// 180도 회전
	for(int i=0; i<N; i++)
		for(int j=0; j<N; j++)
			c2[N-1-i][N-1-j] = before[i][j];

	// 270도(-90도) 회전
	for(int i=0; i<N; i++)
		for(int j=0; j<N; j++)
			c3[N-1-j][i] = before[i][j];

	// 반사(수직선 중심)	- 얘는 자기가 바뀐다.
	for(int i=0; i<(N/2); i++)
		for(int j=0; j<N; j++){
			temp = c4[j][i];		
			c4[j][i] = c4[j][N-1-i];
			c4[j][N-1-i] = temp;
		}

	// 수평반사
	for(int i=0; i<(N/2); i++)
		for(int j=0; j<N; j++){
			temp = c5[i][j];		
			c5[i][j] = c5[N-1-i][j];
			c5[N-1-i][j] = temp;
		}

	// 5-1
	for(int i=0; i<N; i++)
		for(int j=0; j<N; j++)
			c51[j][N-1-i] = c5[i][j];

	// 5-2
	for(int i=0; i<N; i++)
		for(int j=0; j<N; j++)
			c52[N-1-i][N-1-j] = c5[i][j];

	// 5-3
	for(int i=0; i<N; i++)
		for(int j=0; j<N; j++)
			c53[N-1-j][i] = c5[i][j];


	// 패턴 찾기
	if( memcmp(after[0], c1[0], sizeof(after)) == 0)
		fprintf(fout, "%d\n", 1);
	else if( memcmp(after[0], c2[0], sizeof(after)) == 0)
		fprintf(fout, "%d\n", 2);
	else if( memcmp(after[0], c3[0], sizeof(after)) == 0)
		fprintf(fout, "%d\n", 3);
	else if( memcmp(after[0], c4[0], sizeof(after)) == 0)
		fprintf(fout, "%d\n", 4);
	else if( memcmp(after[0], c5[0], sizeof(after)) == 0)
		fprintf(fout, "%d\n", 5);
	else if( memcmp(after[0], c51[0], sizeof(after)) == 0)
		fprintf(fout, "%d\n", 5);
	else if( memcmp(after[0], c52[0], sizeof(after)) == 0)
		fprintf(fout, "%d\n", 5);
	else if( memcmp(after[0], c53[0], sizeof(after)) == 0)
		fprintf(fout, "%d\n", 5);
	else if( memcmp(after[0], before[0], sizeof(after)) == 0)
		fprintf(fout, "%d\n", 6);
	else
		fprintf(fout, "%d\n", 7);







//	fprintf(fout, "%d\n", max_num);

	fclose(fin);
	fclose(fout);

	exit(0);
}
