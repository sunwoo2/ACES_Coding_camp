int max_size = 1024;

typedef struct {	// RISC 16bit 
	int OPCODE;	// 4bit
	int OP1;	// 4bit
	int OP2;	// 8bit
}instruction;

instruction program[1024];		
int	    	memory[256] = {0};
int		regs[16] = {0};	

enum { MOV0, MOV1, MOV2, MOV3, MOV4, ADD, SUB, JZ, SIMD };
