/*
ID: hrkim
LANG: C
TASK: entrance_exam
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int compare(const void* a,const void* b){
	int num1 = *(int*)a;
	int num2 = *(int*)b;

	if(num1<num2)
		return -1;
	if(num1>num2)
		return 1;
	return 0;
};

int main(void)
{
	FILE *fin, *fout;

	fin = fopen("entrance_exam.in", "r");		
	fout = fopen("entrance_exam.out", "w");

	assert((fin != NULL) && (fout != NULL)); // stop program if condition is false

//	fscanf(fin, "%d", &size);


	// 데이터 입력 받기
	int N,K,E,M;
	fscanf(fin, "%d %d %d %d", &N, &K, &E, &M);
	int array[N-1][E];
	for(int i=0; i<N-1; i++)
		for(int j=0; j<E; j++)
			fscanf(fin, "%d", &array[i][j]);
	// 데이터 확인
//	for(int i=0; i<E; i++){
//		for(int j=0; j<N-1; j++)
//			printf("%d ", array[i][j]);
//		printf("\n");
//	}

	int mine[E];
	int my_total = 0;
//	if(E==1){
//		printf("시험 한번만 칠경우\n");
//		fscanf(fin, "%d", &my_total);
//	}else {
		for(int i=0; i<E-1; i++)
			fscanf(fin, "%d", &mine[i]);
		for(int i=0; i<E-1; i++)
			my_total += mine[i];
//	}
//	printf("my_total: %d\n", my_total);

	//데이터 확인
//	for(int i=0; i<E-1; i++)
//		printf("%d ", mine[i]);
//	printf("\n");


	// 나 빼고 나머지 학생들 총합점수 배열&정렬
	int score[N-1];		
	for(int i=0; i<N-1; i++)	// 배열 초기화 해줘야함!!!
		score[i] = 0;
	

	for(int i=0; i<N-1; i++)
		for(int j=0; j<E; j++)
			score[i] += array[i][j];
	qsort(score, N-1, sizeof(int), compare);

//	for(int i=0; i<N-1; i++)
//		printf("%d ", score[i]);
//	printf("\n");


	// 필요한 점수 구하기
	int need_score;
	need_score = (score[K-1]+1) - my_total;



	fprintf(fout, "%d\n", need_score);

//	fprintf(fout, "%d\n", max_num);

	fclose(fin);
	fclose(fout);

	exit(0);
}
