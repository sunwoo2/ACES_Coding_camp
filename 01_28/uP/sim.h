int max_size = 1024;

struct instruction{			// RISC 16비트 
	int OPCODE;	// 4bit
	int OP1;	// 4bit
	int OP2;	// 8bit
};

instruction program[1024];		// code는 1024word 까지 가능
int	    	memory[256] = {0};	// RAM	(0~255, 8비트)
int		regs[16] = {0};		// 0~15, 4비트

enum { MOV0, MOV1, MOV2, MOV3, ADD, SUB, JZ };
