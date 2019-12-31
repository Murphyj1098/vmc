
/** Registers */
typedef enum Registers
{
	A, B, C, D, E, F, I, J,
	PC,
	SP,
	NUM_OF_REGISTERS
} Registers;

static int registers[NUM_OF_REGISTERS]; // holds register data

#define PC (registers[PC])				// program counter abstraction
#define SP (registers[SP])				// stack pointer abstraction