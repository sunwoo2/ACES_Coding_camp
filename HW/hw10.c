/*
ID: hrkim
LANG: C
TASK: milk
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct{
	int m;
	int n;
}MILK;

int main(void)
{
	FILE *fin, *fout;

	fin = fopen("milk.in", "r");		
	fout = fopen("milk.out", "w");

	assert((fin != NULL) && (fout != NULL)); // stop program if condition is false

//	fscanf(fin, "%d", &size);





	// data 입력
	int N,M;
	fscanf(fin, "%d %d", &N, &M);
	MILK m1[M];
	for(int i=0; i<M; i++)
		fscanf(fin, "%d %d", &m1[i].m, &m1[i].n);

	// sorting
	int temp;
	int walker;
	if(M> 1){
		for(int i=1; i<M; i++){
			walker = i;
			for(int j=walker-1; j>=0; j--){
				if( m1[walker].m < m1[j].m ){
					temp = m1[walker].m;
					m1[walker].m = m1[j].m;
					m1[j].m = temp;
					temp = m1[walker].n;	// 자기 짝 찾아가기
					m1[walker].n = m1[j].n;
					m1[j].n = temp;
					walker--;
				}
			}
		}
	}

	// 최소비용계산
	int check = 0;
	int cost = 0; 
	for(int i=0; i<M; i++){
		for(int j=1; j<=m1[i].n; j++){
			if(check==N)
				break;
			check++;
			cost += m1[i].m;
		}
		if(check==N)
			break;
	}

	fprintf(fout, "%d\n", cost);






//	fprintf(fout, "%d\n", max_num);

	fclose(fin);
	fclose(fout);

	exit(0);
}
