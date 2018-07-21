/*
 * vm.h
 *
 * VM emulator for custom bytecode instruction set.
 *
 * instruction Set:
 * instructions are one byte in size represented by a uint8_t type 
 * allowing for 256 emulated instructions. In the case of 
 * non-emulated instructions, a special instruction will be used to 
 * switch to a pass-thru mode which will OPCODEuct the VM to pass the 
 * instruction through the switch-case control structure and execute  
 * it natively. The special byte requires a parameter to identify the 
 * number of instructions to execute before returning back to its 
 * emulaton mode.
 *
 * CPU Design:
 * The CPU is a loop in which the instructions are fetched from the 
 * m_code member of the VM class and then executed in a switch-case 
 * control statement.
 *
 * Registers:
 * The VM has 16 32-bit general purpose registers for use (m_vreg), 
 * a dedicated program counter register (m_pc). Return values will be 
 * stored in v_reg[0].
 *
 * EFLAGS:
 * EFLAGS is an 8-bit used to maintain the results of operations such as 
 * addition, subtraction or comparisons.
 * 
 * EFLAGS Layout:
 *   Z   C   O   S   D   R   R   R
 * +---+---+---+---+---+---+---+---+
 * | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
 * +---+---+---+---+---+---+---+---+  
 *
 * Code Section:
 * The code section is stored within the m_code member of the VM 
 * class with a predefined maximum size. This is allocated on the 
 * stack by default and can be modified to use the heap if necessary.
 *
 * Data Section
 * 
 * TODO
 * 
 * 
 */

#ifndef __VM_H__
#define __VM_H__

#include <cstdint>
#include <string>
#include <vector>

#define NUM_REGISTERS 16
#define DATA_SECTION_SIZE 0x100

#define VM_REG(x) (x)

/*
 * Primitives.
 */
#define OR(x, y) ((x) | (y))
#define NEG(x) (~(x))
#define NOT(x) NEG(x) 
#define NOR(x, y) (NOT(OR(x, y)))

#define AND(x, y) (x & y)
#define XOR(x, y) (x ^ y)
#define ADD(x, y) (x + y)
#define CARRY(x, y) (AND(x, y))
#define ADC(x, y) (ADD(x, NEG((CARRY(x, y) + (x)))))
#define SUB(x, y) (x - y)

typedef int (*PASSTHRU)(void);

/*
 * Create OPCODE type to represent an instruction.
 */
typedef uint8_t OPCODE;

/*
 * Create a REG type to represent a register.
 */
typedef uint32_t REG;

/*
 * Create a IMM8, IMM16 and IMM32 type to represent immediate 8-, 
 * 16- and 32-bit values.
 */
typedef uint8_t IMM8;
typedef uint16_t IMM16;
typedef uint32_t IMM32;

/*
 * Define start and size of virtual ASM code section.
 */
extern OPCODE _vm_start;
extern uint32_t _vm_size;

/*
 * EFLAGS struct using bit fields for each flag.
 * Don't typedef to 'veflags' because of name conflict.
 */
struct _veflags {
	uint8_t zero : 1;			// Zero flag.
	uint8_t carry : 1;			// Carry flag.
	uint8_t overflow : 1;		// Overflow flag.
	uint8_t sign : 1;			// Sign flag.
	uint8_t direction : 1;		// Direction flag.
	uint8_t res0 : 1;			// Reserved.
	uint8_t res1 : 1;			// Reserved.
	uint8_t res2 : 1;			// Reserved.
};

/*
 * Context structure to save the state of the VM.
 */
typedef struct _vcontext {
	REG vreg[NUM_REGISTERS];
	REG vpc;
	REG vsp;
	struct _veflags veflags;
} vcontext;

class VM {
	private:
	/*
	 * 16 general purpose virtual registers.
	 */
	REG m_vreg[NUM_REGISTERS];
	
	/*
	 * Virtual program counter to track current instruction in code section.
	 */
	REG m_vpc;

	/*
	 * Virtual stack pointer to track the current top stack position.
	 */
	REG m_vsp;

	/*
	 * Virtual EFLAGS for tracking state of instruction operations.
	 */
	struct _veflags m_veflags;
	
	/*
	 * Virtual code section to hold set of instructions.
	 */
	//std::vector<OPCODE> m_vcode;
	OPCODE *m_vcode;

	/*
	 * Size of the code section.
	 */
	uint32_t m_vsize;

	/*
	 * Virtual stack section.
	 */
	std::vector<uint32_t> m_vstack;

	/*
	 * Virtual context to save state of VM.
	 */
	vcontext m_vctx;
	
	/*
	 * VM passthru function pointer to handle unsupported instructions.
	 */
	void (*m_passthru)(void);

	/* 
	 * Panic if an unexpected error occured.
	 * Exit process with specified code.
	 */
	void panic(const uint32_t code);

	/*
	 * Wrapper on panic to include custom output string.
	 */
	void panic(const uint32_t code, const std::string& msg);
	
	/*
	 * Initialises the VM class.  
	 * Must be called before starting a new instance.
	 */
	void initialise();

	/*
	 * Fetches an instruction from the code section.
	 */
	OPCODE fetch();

	/*
	 * Executes a given instruction.
	 */
	bool execute(const OPCODE opcode);

	/*
	 * CPU fetch and execute loop.
	 */
	uint32_t loop();

	public:
	/*
	 * Publically accessible virtual data section.
	 */
	std::vector<uint8_t> m_vdata;

	/*
	 * Start VM execution with predefined data.
	 */
	uint32_t start(const std::vector<uint8_t>& data);

	/*
	 * Start VM execution.
	 */
	uint32_t start();
};


#endif // !__VM_H__
