/* Project VMC */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "registers.h"
// #include "statusFlags.h" currently unused


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
	SUB, // 6  -- SUB					:: subtracts top two vals on stack, (top - next), result back on stack
	MUL, // 7  -- MUL					:: multiplies top two vals on stack
	DIV, // 8  -- DIV					:: divides top two vals on stack
	SLT, // 9  -- SLT <reg1> <reg2>		:: pushes (<reg1> < <reg2>) to stack
	LOG, // 10 -- LOG <val/reg1>		:: print out <va/reg1>
	IF,  // 11 -- IF  <reg1> <val> <ip> :: if <reg1> == <val> branch to the pc
	IFN, // 12 -- IFN <reg1> <val> <ip> :: if <reg1> != <val> branch to the pc
	GLD, // 13 -- GLD <reg1>			:: loads <reg1> to stack
	GPT, // 14 -- GPT <reg1>			:: pushes top of stack to <reg1>
	NOP  // 15 -- NOP					:: empty instruction
} InstructionSet;

int *instructions; // array to hold instructions read in from file
int instruction_space = 4; // amount of space allocated for the instruction array (default = 4)
int instruction_count = 0; // number of instructions to be executed


/** Eval */
// determines what to do for a given instruction
void eval(int instr)
{
	switch(instr)
	{
		case PSH: {
			SP++;
			stack[SP] = instructions[++PC];
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
			int registerIn = instructions[PC++];
			int val = instructions[PC++];

			if(registerIn > 7)
			{
				running = false;
				printf("Error, set calls out of bound register\n");
				break;
			}

			registers[registerIn] = val;
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
// set to value of current instruction
#define FETCH (instructions[PC])


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
		eval(FETCH);	// get and eval current instruction
		PC++;			// increment program counter
	}

	free(instructions);

	return 0;
}
