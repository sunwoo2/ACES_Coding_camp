#include <iostream>
#include <fstream>
#include <cstdlib>
#include "sim.h"	// C++코드를 C에서 쓰겟다.?

#define DEBUG

using namespace std;		// C++이 방대하기때문에 namespace 줘서 std 쓰기 

void disassemble(int pc);
void run_program(int size);
void info_regs();		// 16개의 레지스터값을 이쁘게 표현
void info_memory();		// 전체 memory중에서 값이 들어있는 메모리 표시
void info_memory1(int addr, int size);		// 부분 표시

int main(int argc, char* argv[]){
	
	int line;
	int size=0;
	char bit;
	int CODE ;
//	cout << "hello, C++ world!\n";

	if(argc != 3){		// argc : 명령어 자기자신 포함해서 터미널에서 인수 개수
		// C의 printf. 기본 입출력
		cout << "iss <input file> <line>\n";		// 핵심부터 짜라!! 변수선언도 나중에 해도됨
		cout << "ex) iss fib.bin 18\n";
		return -1;
	}

	ifstream infile;
	infile.open(argv[1], ios::in);		// file open

	line = atoi(argv[2]);		// atoi : "5000"을 5000 처럼 바꿔줌

	while(size < line){
		if(size>=max_size){		// 1024line 넘어가면 종료되게끔
			cout << "error: input binary code is too big to simulate\n";
			return -1;
		}

		// (1) decode OPCODE
		CODE = 0;	// clear한번 해주는거임 
		for(int i=0; i<4; i++){		// 이진법을 십진법으로나타내는?
			infile >> bit;
			if(bit == '1')		
				CODE |= 8 >> i;		// |= : 비트 더하기??
							// 8은 1000 비트
		}

		program[size].OPCODE = CODE;	// 0011이니깐 3이 OPCODE에 들어가는거임

		// (2) decode OP1
		CODE = 0;	
		for(int i=0; i<4; i++){	
			infile >> bit;
			if(bit == '1')		
				CODE |= 8 >> i;
		}

		program[size].OP1 = CODE;

		// (3) decode OP2
		CODE = 0;	
		for(int i=0; i<8; i++){	
			infile >> bit;
			if(bit == '1')		
				CODE |= 128 >> i;	// 최상위비트 msb 128
		}

		program[size].OP2 = CODE;

#ifndef DEBUG		// 디버깅용 참고.
		for(int i=0; i<16; i++){
			infile >> bit;
			cout << bit;
		}
		cout << "\n";
#endif


		size++;		// 항상 루프는 종료조건을 잘 정해줘야함!!!
	}

//	for(int i=0; i<line; i++)
//		disassemble(i);

	// run entire program
	run_program(size);


//	info_regs();
//	info_memory();
//	info_memory1(11,10);
	
	return 0;
}

void disassemble(int pc){

//	char ISA[7][5] = {"MOV0","MOV1" ,"MOV2" ,"MOV3" ,"ADD" ,"SUB" ,"JZ" };	// 명령어가 7개까지 지원된다고 보고

	switch( program[pc].OPCODE ){
		
		case MOV0:
			cout << "MOV0 R" << program[pc].OP1 << ", " << program[pc].OP2 <<endl;
			break;
		case MOV1:
			cout << "MOV1 " << program[pc].OP2 << ", R" << program[pc].OP1 << endl;
			break;
		case MOV2:
			cout << "MOV2 @R" << program[pc].OP1 << ", R" << (program[pc].OP2 >> 4) << endl;
			break;
		case MOV3:
			cout << "MOV3 R" << program[pc].OP1 << ", #" << (program[pc].OP2 >> 4) << endl;
			break;
		case MOV4:
			cout << "MOV4 R" << program[pc].OP1 << ", R" << (program[pc].OP2 >> 4) << endl;
			break;
		case ADD:
			cout << "ADD R" << program[pc].OP1 << ", R" << (program[pc].OP2 >> 4) << endl;
			break;
		case SUB:
			cout << "SUB R" << program[pc].OP1 << ", R" << (program[pc].OP2 >> 4) << endl;
			break;
		case JZ:
			if( program[pc].OP2 & 128 ){	// MSB is 1, so negative
				program[pc].OP2 = -(~(char)program[pc].OP2 + 1);
				cout << "JZ R"<< program[pc].OP1<<", #" << program[pc].OP2 <<  endl;
			}else
				cout << "JZ R"<< program[pc].OP1<<", #" << program[pc].OP2 <<  endl;
			break;
		case SIMD:
			cout << "SIMD R" << program[pc].OP1 << ", R" << (program[pc].OP2 >> 4) << endl;
			break;
		default:
			cout << "Undefined instruction" << endl;
			break;
	}
}

void run_program(int size){		

	int pc = -1;

	// clock수를 세어보자
	int clks[9] = {4,4,8,6,2,7,7,15,12};		// mov0 는 4클럭, mov1도 4클럭 ...
	// 명령어의 실행 빈도수
	int freq[9] = {0,0,0,0,0,0,0,0,0};

	while(++pc < size){		// 안에서 while돌리면 장점 : 끝날려 했는데 JZ되서 점프되면 다시 돌아와서 다시 while또 돌고있는거임 JZ되면 계~속 도는거임 (동적)
		disassemble(pc);	// dynamic하게 다~보여주자
		switch(program[pc].OPCODE){
			case MOV0:
				// 	MOV0 Rn, direct,
				//	: Rn <- mem[direct]
				// ex)	MOV0 R1, 24
				//	: R1 <- mem[24]
				regs[program[pc].OP1] = memory[program[pc].OP2];
				freq[MOV0]++;
//				cout << "pc: " << pc<< endl;
				break;

			case MOV1:
				// ex)	MOV1 24, R1
				//	: mem[24] <- R1
				memory[program[pc].OP2] = regs[program[pc].OP1];  // 이게 캐시
				freq[MOV1]++;
//				cout << pc<< endl;
				break;

			case MOV2:
				// ex)	MOV2 @R2, R0
				//	: mem[R2] <- R0		// mem이 D램 같은 메모리, R2 얘들은 전부 캐시
				memory[ regs[program[pc].OP1] ] = regs[program[pc].OP2 >> 4];		// indirect 메모리 access
					// 값을 가져옴					// 상위 4비트에 있으니깐 비트이동 해주는거임
				freq[MOV2]++;
//				cout << pc<< endl;
				break;
			
			case MOV3:
				// ex)	MOV3 R1, #12		// #12자체가 값
				if( program[pc].OP2 & 128 )	// MSB is 1, so negative
					program[pc].OP2 = -(~(char)program[pc].OP2 + 1);	// 8비트로 변환( 2의보수표현 으로 음수 처리)
				// 1111 1111 -> ~(1111 1111) -> 0000 0000 -> 0000 0001 -> -1
				regs[program[pc].OP1] = program[pc].OP2;		// 얘는 그냥 OP2 8비트 다 써도 되잖아
				freq[MOV3]++;
//				cout << pc<< endl;
				break;

			case MOV4:
				// ex)	MOV4 R1, R0
				//	: R1 <- R0
				// OP2의 레지스터는 상위 4비트를 인식
				regs[program[pc].OP1] = regs[program[pc].OP2 >> 4];
				freq[MOV4]++;
//				cout << "pc: " << pc<< endl;
				break;

			case ADD:
				// ex)	ADD R1, R2
				//	R1 <- R1+R2;
				regs[program[pc].OP1] = regs[program[pc].OP1] + regs[program[pc].OP2 >> 4];
				freq[ADD]++;
//				cout << pc<< endl;
				break;

			case SUB:
				// ex)	SUB R1, R2
				//	R1 <- R1-R2;
				regs[program[pc].OP1] = regs[program[pc].OP1] - regs[program[pc].OP2 >> 4];		// 8비트중에 상위4비트에 표시되있다고 가정
				freq[SUB]++;
//				cout << pc<< endl;
				break;

			case JZ:	// 조건 분기문!! (c의 if문 역할)	
				// 함수 call하면 일어나는게 이 동작임
				// 포맷을 모르겟네..!?!?!?
				// ex: JZ R3, #-5
				// 일단 이렇게 가정하고 쓰자 ( OP1이 0로면 점프)
				if( program[pc].OP2 & 128 )	// MSB is 1, so negative
					program[pc].OP2 = -(~(char)program[pc].OP2 + 1);
				if( regs[program[pc].OP1 ] == 0)
					pc += program[pc].OP2;
				freq[JZ]++;
//				cout << pc<< endl;
				break;

			case SIMD:
				// ex)	SIMD R1, R2
				//	R1[4] <- R1[4] + R2[4];
				// R1포함 뒤로 6개, R2포함 뒤로 6개로 가정
				for(int i=0; i<6; i++)
					regs[program[pc].OP1 + i] = regs[program[pc].OP1 + i] + regs[(program[pc].OP2 >> 4) + i];
				freq[SIMD]++;
//				cout << pc<< endl;
				break;

			default:
				cout << "Undefined instruction" << endl;
				break;
		} // end of switch
		if(regs[5] < 0){
			cout << "0일때 점프 해야되잖아" << endl;
			break;
		}
	} // end of while
	

// opt ( MOV4 쓸때)
	cout << "fibonacci value: ";
	for(int i=11; i>5; i--){
		if(regs[i+3]==0)
			break;
		cout << regs[i+3] << " ";
	}
//	cout << regs[0];
	cout <<  endl;


//	cout << "fibonacci value: " << regs[3]<<", "<<regs[4]<<", "<<regs[5]<<", "<<regs[6]<<", "<<regs[7]<<", "<<regs[8]<<", "<<regs[9]<<", "<<regs[10]<<", "<<regs[11]<<", "<<regs[12]<<", "<<regsendl;


// SIMD Check(6개짜리)
//	cout << regs[8]<<", "<<regs[7]<<", "<<regs[6] <<", "<<regs[5]<<", "<<regs[4]<<", "<<regs[3]<<endl;
//	cout << regs[14] <<", "<<regs[13]<<", "<<regs[12]<<", "<<regs[11]<<", "<<regs[10]<<", "<<regs[9]<<endl;

	// calculate execution time
	int time = 0;
	int total = 0;
	for(int i=0; i<9; i++){
		time = freq[i]*clks[i];
		total += freq[i]*clks[i];
		if(i==0)
			cout << "MOV0 - ";
		if(i==1)
			cout << "MOV1 - ";
		if(i==2)
			cout << "MOV2 - ";
		if(i==3)
			cout << "MOV3 - ";
		if(i==4)
			cout << "MOV4 - ";
		if(i==5)
			cout << "ADD  - ";
		if(i==6)
			cout << "SUB  - ";
		if(i==7)
			cout << "JZ   - ";
		if(i==8)
			cout << "SIMD - ";
		cout << "time: "<<time<<", freq: "<< freq[i] << endl;
	}
	cout << "total time: " << total << endl;
}

void info_regs(){
	cout << endl <<"<사용중인 resgister>" << endl;
	for(int i=0; i<16; i++){
		if(regs[i] != 0)
			cout << "register[" << i << "] = " << regs[i] << endl;
	}
}

void info_memory(){
	cout << endl <<"<사용중인 memory>" << endl;
	for(int i=0; i<256; i++){
		if(memory[i] != 0)
			cout << "memory[" << i << "] = " << memory[i] << endl;
	}
}

void info_memory1(int addr, int size){
	cout << endl <<"<"<<addr<<"~"<<addr+size-1<<" memory>" << endl;
	for(int i=addr; i<addr+size; i++){
		cout << "memory[" << i << "] = " << memory[i] << endl;
	}
}
