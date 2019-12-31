
/** Status flags */
typedef enum Flags
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