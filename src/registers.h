
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