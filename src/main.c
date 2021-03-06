/* Project VMC */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "registers.h"
#include "statusFlags.h"


bool running = true;			// status of current program

/** Stack */
#define STACK_SIZE 256
static int stack[STACK_SIZE];	// create stack


/** Instructions */
typedef enum InstructionSet
{
	PSH, // 0  -- PSH <val> 			:: pushes <val> to stack
	ADD, // 1  -- ADD 					:: adds top two vals on stack, result back on stack
	POP, // 2  -- POP <reg1>			:: pushes top of stack to <reg1>
	SET, // 3  -- SET <reg1> <val>		:: set <reg1> to hold <val>
	HLT, // 4  -- HLT					:: terminate program
	MOV, // 5  -- MOV <reg1> <reg2>		:: moves val from <reg2> to <reg1>
	SUB, // 6  -- SUB					:: subtracts top two vals on stack, (top - next), result back on stack
	MUL, // 7  -- MUL					:: multiplies top two vals on stack
	DIV, // 8  -- DIV					:: divides top two vals on stack
	SLT, // 9  -- SLT <reg1> <reg2>		:: pushes (<reg1> < <reg2>) to stack
	LOG, // 10 -- LOG <reg1>			:: print out value of <reg1>
	IF,  // 11 -- IF  <reg1> <val> <ip> :: if <reg1> == <val> branch to the pc
	IFN, // 12 -- IFN <reg1> <val> <ip> :: if <reg1> != <val> branch to the pc
	GLD, // 13 -- GLD <reg1>			:: loads <reg1> to stack
	NOP, // 14 -- NOP					:: No operation
	XXX, // 15 -- XXX					:: Placeholder operation
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
			stack[++SP] = instructions[++PC];
			break;
		}
		case ADD: {
			int a = stack[SP--]; // pop first val
			int b = stack[SP--]; // pop second val (higher in stack)

			int result = b + a; // ORDER! Stacks are LIFO

			stack[++SP] = result;

			if(result == 0)				// If the result is 0
				EFLAGS = EFLAGS | ZF;	// Set the zero flag

			break;
		}
		case POP: {
			int popVal = stack[SP--];
			registers[instructions[++PC]] = popVal; // Put top stack value in destination register

			// printf("Popped %d\n", popVal); // debug statement
			break;
		}
		case SET: {
			int registerSrc = instructions[++PC];
			int val = instructions[++PC];

			if(registerSrc > 7)
			{
				running = false;
				printf("Error, SET calls out of bound register\n");
				break;
			}

			registers[registerSrc] = val;
			break;
		}
		case HLT: {
			running = false;
			break;
		}
		case MOV: {
			int registerDes = instructions[++PC];
			int registerSrc = instructions[++PC];

			if(registerSrc > 7 || registerDes > 7)
			{
				running = false;
				printf("Error, MOV calls out of bound register\n");
			}

			registers[registerDes] = registers[registerSrc]; // Move value from source register to destination register
			// printf("Moved value: %d\n", registers[registerDes]); // debug statement
			break;
		}
		case SUB: {
			int a = stack[SP--];
			int b = stack[SP--];

			int result = b - a;

			stack[++SP] = result;

			if(result == 0)				// If the result is 0
				EFLAGS = EFLAGS | ZF;	// Set the zero flag

			break;
		}
		case MUL: {
			int a = stack[SP--];
			int b = stack[SP--];

			int result = b * a;

			stack[++SP] = result;
			break;
		}
		case DIV: {
			int a = stack[SP--];
			int b = stack[SP--];

			int result = b / a;

			stack[++SP] = result;
			break;
		}
		case SLT: {
			SP--;
			stack[SP] = stack[SP+1] < stack[SP];
			break;
		}
		case LOG: {
			int val = registers[instructions[++PC]];
			printf("%d\n", val);
			break;
		}
		case  IF: {
			break;
		}
		case IFN: {
			break;
		}
		case GLD: {
			int val = registers[instructions[++PC]]; // store val in <reg1>
			stack[++SP] = val; // put val on top of stack
			break;
		}
		case NOP: {
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
	if(argc <= 1)
	{
		printf("Error, no input program as argument\n");
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
		printf("Error, file is of wrong type\n");
		return -1;
	}

	// check that file is readable
	FILE *file = fopen(fileName, "r");
	if(!file)
	{
		printf("Error, file is unreadable\n");
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
