/* Project VMC */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

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

int *instructions; // array to hold instructions read in from file
int instruction_space = 4; // amount of space allocated for the instruction array (default = 4)
int instruction_count = 0; // number of instructions to be executed


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
			SP++;
			stack[SP] = program1[++PC];
			break;
		}
		case ADD: {
			int a = stack[SP--]; // pop first val
			int b = stack[SP--]; // pop second val (higher in stack)

			int result = b + a; // ORDER! Stacks are LIFO

			SP++;
			stack[SP] = result;
			break;
		}
		case POP: {
			int popVal = stack[SP--];
			printf("Popped %d\n", popVal);
			break;
		}
		case SET: {
			registers[program1[PC++]] = program1[PC++];
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
			int a = stack[SP--];
			int b = stack[SP--];

			int result = b - a;

			SP++;
			stack[SP] = result;
			break;
		}
	}
}

/** Fetch */
// reads current instruction (may be removed)
int fetch()
{
	return program1[PC]; // TODO: how to pick what program is running?
}


int main(int argc, char* argv[])
{
	//take in file to use as program
	if(argc < 1)
	{
		printf("Error, no input program as argument");
		return -1;
	}

	char* fileName = argv[1];

	// check that file is appropriate type (.vmc)
	int nameLen = strlen(fileName);
	char fileType[4]; // holds last 4 characters of file name (should be ".vmc")

	// copy file extension to fileType
	int typePos = 0;
	int filePos = (nameLen - 4);
	for(; filePos<nameLen; filePos++)
	{
		fileType[typePos] = fileName[filePos];
		typePos++;
	}

	if(strcmp(fileType, ".vmc") != 0)
	{
		printf("Error, file is of wrong type");
		return -1;
	}

	// check that file is readable
	FILE *file = fopen(fileName, "r");
	if(!file)
	{
		printf("Error, file is unreadable");
		return -1;
	}

	// allocate memory for instruction array
	instructions = malloc(sizeof(*instructions) * instruction_space);

	// read instruction file in
	int num;
	int i = 0;
	while(fscanf(file, "%d", &num) > 0)
	{
		instructions[i] = num;
		i++;
		if(i >= instruction_space)
		{
			instruction_space *= 2;
			instructions = realloc(instructions, sizeof(*instructions) * instruction_space); // double array size
		}
	}

	instruction_count = i; // set number of instructions

	fclose(file);


	// if HLT not called
	while(running && PC < instruction_count)
	{
		eval(fetch());	// get and eval current instruction
		PC++;			// increment program counter
	}

	free(instructions);

	return 0;
}
