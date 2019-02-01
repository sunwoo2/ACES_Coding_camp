/*
ID: hrkim
LANG: C
TASK: friday
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main(void)
{
	FILE *fin, *fout;

	fin = fopen("friday.in", "r");		
	fout = fopen("friday.out", "w");

	assert((fin != NULL) && (fout != NULL)); // stop program if condition is false

//	fscanf(fin, "%d", &size);




	int day=0;	// %7로 요일 표시. 1-월, 2-화 . . .
	int month=1;	// %12로 월 표시. 1-1월, 2-2월 . . .
	int year=1900;
	int N;
	int result[7] = {0,0,0,0,0,0,0};	// 13일인 요일횟수  (일,월,화,수,목,금,토)
	int leaf_year;		// 윤년(29일)

	fscanf(fin, "%d", &N);
	N += year;

	for(year=1900; year<N; year++){
		switch((month%12)){
			case 1:		// 1월
//				for(int i=1; i<=31; i++)
//					if(++day == 13)		// 13일 요일 check
//						result[day%7]++;

				day += 13;
				result[day%7]++;
				day += 18;
				month++;
			case 2:		// 2월
				if( (year%4) == 0){		// 4년으로 나눠지면 윤년(4의 배수)
					leaf_year = 29;
					if( (year%100) == 0){	// 100년으로 나눠지면 윤년아님
						leaf_year = 28;
						if( (year%400) == 0)		// 그중에서 400년으로 나눠지는 해는 무조건 윤년
							leaf_year = 29;
					}
				}else
					leaf_year = 28;

				day += 13;
				result[day%7]++;
				day += (leaf_year - 13);
				month++;
			case 3:		
				day += 13;
				result[day%7]++;
				day += 18;
				month++;
			case 4:		
				day += 13;
				result[day%7]++;
				day += 17;
				month++;
			case 5:		
				day += 13;
				result[day%7]++;
				day += 18;
				month++;
			case 6:	
				day += 13;
				result[day%7]++;
				day += 17;
				month++;
			case 7:		
				day += 13;
				result[day%7]++;
				day += 18;
				month++;
			case 8:		
				day += 13;
				result[day%7]++;
				day += 18;
				month++;
			case 9:		
				day += 13;
				result[day%7]++;
				day += 17;
				month++;
			case 10:		
				day += 13;
				result[day%7]++;
				day += 18;
				month++;
			case 11:		
				day += 13;
				result[day%7]++;
				day += 17;
				month++;
			case 12:		
				day += 13;
				result[day%7]++;
				day += 18;
				month++;
		}
	}

	fprintf(fout, "%d %d %d %d %d %d %d\n", result[6], result[0], result[1], result[2], result[3], result[4], result[5]);




//	fprintf(fout, "%d\n", max_num);

	fclose(fin);
	fclose(fout);

	exit(0);
}
