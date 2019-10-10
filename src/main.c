/* Project VMC */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

bool running = true;					// status of current program

/** Stack */
#define STACK_SIZE 256
static int stack[STACK_SIZE]; // create stack


/** Instructions */
typedef enum
{
	PSH, // 0  -- PSH <val> 			:: pushes <val> to stack
	ADD, // 1  -- ADD 					:: adds top two vals on stack, result back on stack
	POP, // 2  -- POP 					:: removes top val on stack
	SET, // 3  -- SET <reg> <val>		:: set <reg> to hold <val>
	HLT, // 4  -- HLT					:: terminate program
	MOV, // 5  -- MOV <reg1> <reg2>		:: moves val from <reg2> to <reg1>
	SUB	 // 6  -- SUB					:: subtracts top two vals on stack, (top - next), result back on stack
} InstructionSet;


/** Registers */
typedef enum
{
	A, B, C, D, E, F, I, J,
	PC,
	SP,
	NUM_OF_REGISTERS
} Registers;

static int registers[NUM_OF_REGISTERS]; // holds register data

#define pc (registers[PC])				// program counter abstraction
#define sp (registers[SP])				// stack pointer abstraction


/** Status flags */
typedef enum
{
	CF, // [bit 0] -- Carry flag		:: Set if the last arithmetic operation carried (addition) or borrowed (subtraction) a bit beyond its register size
	PF, // [bit 1] -- Parity flag		:: Set if the number of set bits in the least significant byte is a multiple of 2
	AF, // [bit 2] -- Adjust flag		:: Carry of BCD numbers aruthmetic operations
	ZF, // [bit 3] -- Zero flag			:: Set if the result of an operation is 0
	SF, // [bit 4] -- Sign flag 		:: Set if the result of an operation is negative
	IF, // [bit 5] -- Interrupt flag 	:: Set if interrupts are enabled
	OF, // [bit 6] -- Overflow flag 	:: Set if signed arithmetic operations result in a value too large for the register to contain
	NF	// [bit 7] -- Empty flag		:: Currently empty (value doesn't matter) 
	
} Flags;

static unsigned char EFLAGS[1]; // single 8 bit register (1 bit per flag)

/* Flag Bit Masks
 *  use & with 0 to clear a bit
 *  use | with 1 to set a bit
 */
#define CF_BITS 0x80
#define PF_BITS 0x40
#define AF_BITS 0X20
#define ZF_BITS 0X10
#define SF_BITS 0X08
#define IF_BITS 0X04
#define OF_BITS 0X02
#define NF_BITS 0X01


/** Programs (for testing) */
// in final program files passed in as arg
const int program2[] = {
	PSH, 5,
	PSH, 6,
	ADD,
	POP,
	HLT
};

const int program1[] = {
	PSH, 1,
	PSH, 3,
	ADD,
	PSH, 2,
	ADD,
	PSH, 5,
	SUB,
	POP,
	HLT
};


/** Eval */
// determines what to do for a given instruction
void eval(int instr)
{
	switch(instr)
	{
		case PSH: {
			sp++;
			stack[sp] = program1[++pc];
			break;
		}
		case ADD: {
			int a = stack[sp--]; // pop first val
			int b = stack[sp--]; // pop second val (higher in stack)

			int result = b + a; // ORDER! Stacks are LIFO

			sp ++;
			stack[sp] = result;
			break;
		}
		case POP: {
			int popVal = stack[sp--];
			printf("Popped %d\n", popVal);
			break;
		}
		case SET: {
			registers[program1[pc++]] = program1[pc++];
			break;
		}
		case HLT: {
			running = false;
			break;
		}
		case MOV: {
			break;
		}
		case SUB: {
			int a = stack[sp--];
			int b = stack[sp--];

			int result = b - a;

			sp++;
			stack[sp] = result;
			break;
		}
	}
}

/** Fetch */
// reads current instruction (may be removed)
int fetch()
{
	return program1[pc]; // TODO: how to pick what program is running?
}


int main(int argc, char* argv[])
{
	//take in file to use as program


	// if HLT not called
	while(running)
	{
		eval(fetch());	// get and eval current instruction
		pc++;			// increment program counter
	}
}
