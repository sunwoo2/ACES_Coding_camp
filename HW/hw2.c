/*
ID: hrkim
LANG: C
TASK: gift1
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define PRINT

typedef struct{
	char name[15];
	int money;
} PERSON;

int main(void)
{
	FILE *fin, *fout;
	int size, gift, num;
	char who[15];


	fin = fopen("gift1.in", "r");		
	fout = fopen("gift1.out", "w");

	assert((fin != NULL) && (fout != NULL)); // stop program if condition is false

	fscanf(fin, "%d", &size);

	PERSON group[size];

	for(int i=0; i<size; i++){		// 그룹내 사람이름 받기
		fscanf(fin, "%s", group[i].name);
		group[i].money = 0;
	}

	for(int m=0; m<size; m++){
		fscanf(fin, "%s", who);		// 돈빠져나가는사람(선물 주는사람)
		fscanf(fin, "%d %d", &gift, &num);
		for(int j=0; j<size; j++)	// 선물 주는 사람 찾기
			if( strcmp(group[j].name, who) == 0 ){
				group[j].money -= gift;
				(num!=0) ? (group[j].money+=(gift%num)) : (group[j].money += 0); 
			}
	
		for(int i=0; i<num; i++){
			fscanf(fin, "%s", who);		// 선물받는사람 리스트 
			for(int j=0; j<size; j++){	// 선물받는사람 찾기
				if( strcmp(group[j].name, who) == 0 ){
					(num!=0) ? (group[j].money+=(gift/num)) : (group[j].money += 0); 
				}
			}
		}
	}	

	for(int i=0; i<size; i++){
		fprintf(fout, "%s %d\n", group[i].name, group[i].money);
	}


//	fprintf(fout, "%d\n", max_num);

	fclose(fin);
	fclose(fout);

	exit(0);
}
