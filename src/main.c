/* Project VMC */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "registers.h"
#include "statusFlags.h"


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


/** Programs (for testing) */
// in final program files passed in as arg
const int program1[] = {
	PSH, 5,
	PSH, 6,
	ADD,
	POP,
	HLT
};

const int program2[] = {
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
