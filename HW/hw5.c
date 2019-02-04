/*
ID: hrkim
LANG: C
TASK: milk2
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct{
	int st;
	int ft;
}COW;

int main(void)
{
	FILE *fin, *fout;

	fin = fopen("milk2.in", "r");		
	fout = fopen("milk2.out", "w");

	assert((fin != NULL) && (fout != NULL)); // stop program if condition is false

//	fscanf(fin, "%d", &size);





	int cow_num;
	fscanf(fin, "%d", &cow_num);

	// data 입력
	COW c1[cow_num];
	for(int i=0; i<cow_num; i++)
		fscanf(fin, "%d %d", &c1[i].st, &c1[i].ft);

	// sorting!!!!!!!! 
	int temp;
	int walker;
	if(cow_num > 1){
		for(int i=1; i<cow_num; i++){
			walker = i;
			for(int j=walker-1; j>=0; j--){
				if( c1[walker].st < c1[j].st ){
					temp = c1[walker].st;
					c1[walker].st = c1[j].st;
					c1[j].st = temp;
					temp = c1[walker].ft;	// 자기 짝 찾아가기
					c1[walker].ft = c1[j].ft;
					c1[j].ft = temp;
					walker--;
				}
			}
		}
	}
	
	// time 구하기
	int work_st = c1[0].st;
	int work_ft = c1[0].ft;
	int work_max = c1[0].ft - c1[0].st;
	int break_max = 0;

	for(int i=1; i<cow_num; i++){
		if( work_ft < c1[i].st){	// break time 생길때
			if( work_ft-work_st > work_max )	// work time 비교
				work_max = work_ft-work_st;
			if( c1[i].st-work_ft > break_max )	// break time 비교	
				break_max = c1[i].st-work_ft;
			work_st = c1[i].st;	// work 시작
			work_ft = c1[i].ft;
		}else{		// continue work time
			if( work_ft < c1[i].ft)		// work_ft 보다 더 일할때
				work_ft = c1[i].ft;
		}
	}

	fprintf(fout, "%d %d\n", work_max, break_max);





//	fprintf(fout, "%d\n", max_num);

	fclose(fin);
	fclose(fout);

	exit(0);
}
