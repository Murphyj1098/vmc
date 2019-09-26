//////////////////////////////////////////////
// Project VMC								//
// Creating a simple virtual machine in C	//
//////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/** Stack */
#define STACK_SIZE 256;
static int stack[STACK_SIZE]; // create stack


/** Instructions */
typedef enum
{
	PSH, // 0  -- psh val 		:: pushes <val> to stack
	ADD, // 1  -- add 			:: adds top two vals on stack, result back on stack
	POP,
	SET,
	HLT,
	MOV
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

#define pc (registers[PC])			// program counter
#define sp (registers[SP])			// stack pointer


bool running = true;				// status of current program


// programs
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
	POP,
	HLT
};


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
	}
}

// returns the current instruction from the program array
int fetch()
{
	return program1[pc]; // TODO: how to pick what program is running?
}


int main()
{
	// if HLT not called
	while(running)
	{
		eval(fetch());	// get and eval current instruction
		pc++;			// increment program counter
	}
}
