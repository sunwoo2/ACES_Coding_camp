/*
ID: hrkim
LANG: C
TASK: beads
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main(void)
{
	FILE *fin, *fout;

	fin = fopen("beads.in", "r");		
	fout = fopen("beads.out", "w");

	assert((fin != NULL) && (fout != NULL)); // stop program if condition is false

//	fscanf(fin, "%d", &size);





	int red, blue, white, count_white;	// count_white: 결정해야할 white
	int pre_red, pre_blue, beads_max;		// 최대 장식 개수
	char con_color = 'a';		// 현재 이어지고 있는 색깔
	int size;	fscanf(fin, "%d\n", &size);
	int check_r, check_b; 	// 이미 정렬 되있는지 check하기위해 ( 변하는 횟수 check)
	char beads[size+1];	fscanf(fin, "%s\n", beads);	// NULL문자 고려
	char con_beads[size*2+1];		// 2개 연결
	strcpy(con_beads, beads);
	strcat(con_beads, beads);


	// 시작 설정
	int start=0;
	while( con_color == 'a'){
		if( con_beads[start] == 'r'){
			con_color = 'r';
			red += (white+1);
		}else if( con_beads[start] == 'b'){
			con_color = 'b';
			blue += (white+1);
		}else{		// white인 경우
			white++;
		}
		start++;
		if( start > size*2)	// white로만 되어있는경우를 위해
			break;
	}

	for(int i=start; i<(strlen(con_beads)-start+1); i++){
		if(con_beads[i] == 'r'){	// 현재 색이 red
			if( con_color == con_beads[i]){		// 색이 이어질 경우
				red++;
				if( white != 0){		// 그전에 white가 있었던 경우
					red += white;
					white = 0;
				}
			}else{				// blue에서 red로	
				check_r++;		// red로 바뀌는 횟수 check
				if( con_beads[i-1] == 'w'){	// blue와 red 사이에 white가 끼여 있는경우

					// 이전에 count_white가 있는경우 까지 고려(count_white가 없으면 어차피 0이니깐)
					blue += count_white;
					if( (blue+pre_red+white) > beads_max)
						beads_max = blue+pre_red+white;
					count_white = white;	// count_white : 나중에 더해줄  white
					white = 0;
					con_color = 'r';
					pre_blue = blue;
					blue = 0;
					red++;
				}else{ 		// 그냥 blue에서 red로 바뀌는경우
					// 이경우는 count_white만 신경 써줌. white는 없으니깐
					blue += count_white;
					if( (blue+pre_red) > beads_max)
						beads_max = blue+pre_red;
					count_white = 0;
					con_color = 'r';
					pre_blue = blue;
					blue = 0;
					red++;
				}
			}
		}else if(con_beads[i] == 'b'){
			if( con_color == con_beads[i]){
				blue++;
				if( white != 0){
					blue += white;
					white = 0;
				}
			}else{			// red에서 blue로 바뀔때		
				check_b++;
				if( con_beads[i-1] == 'w'){	// red와 blue 사이에 white가 끼여 있는경우

					red += count_white;
					if( (red+pre_blue+white) > beads_max)
						beads_max = red+pre_blue+white;
					count_white = white;	
					white = 0;
					con_color = 'b';
					pre_red = red;
					red = 0;
					blue++;
				}else{ 		// 그냥 red에서 blue로 바뀌는경우
					red += count_white;
					if( (red+pre_blue) > beads_max)
						beads_max = red+pre_blue;
					count_white = 0;
					con_color = 'b';
					pre_red = red;
					red = 0;
					blue++;
				}
			}
		}else		// white인 경우
			white++;
	}
	

	// white로만 되어 있는경우
	if((white != 0) && (check_r==0) && (check_b==0))
		beads_max = white;

	// 장식이 이미 같은색끼리 정렬되어 있는경우
	if( (check_r<=2)&&(check_b<=2) )
		beads_max = size;

	fprintf(fout, "%d\n", beads_max);



//	fprintf(fout, "%d\n", max_num);

	fclose(fin);
	fclose(fout);

	exit(0);
}
