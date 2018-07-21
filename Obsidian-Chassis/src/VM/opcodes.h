#ifndef __OPCODES_H__
#define __OPCODES_H__

/*
 * Standard CPU opcodes.
 */
#define VM_HLT 0x00 			    // Ends VM emulation execution.
#define VM_MOV 0x01					// mov reg, reg
#define VM_MOVI 0x02				// mov reg, imm32 (mov imm32 to reg)
#define VM_ADD 0x03					// add reg, reg
#define VM_ADDI 0x04				// iadd reg, imm32 (mov imm32 to reg)
#define VM_SUB 0x05					// sub reg, reg
#define VM_SUBI 0x06				// isub reg, imm32 (sub imm32 from reg)
#define VM_ADC 0x07					// adc reg, reg (add with carry)
#define VM_SBB 0x08					// sbb reg, reg (sub with borrow)
#define VM_INC 0x09					// inc reg
#define VM_DEC 0x0A 				// dec reg
#define VM_CMP 0x0B					// cmp reg, imm32
#define VM_LEA 0x0C 				// lea reg, reg
#define VM_NEG 0x0D					// neg reg
#define VM_OR 0x0E					// or reg, reg
#define VM_AND 0x0F 				// and reg, reg
#define VM_NOT 0x10 				// not reg
#define VM_NOR 0x11                 // nor reg, reg
#define VM_XOR 0x12 				// xor reg, reg
#define VM_XORI 0x13				// ixor reg, imm32 (xor with imm32)
#define VM_TEST 0x14 				// test reg, reg
#define VM_SHR 0x15 				// shr reg, reg
#define VM_SHL 0x16					// shl reg, reg
#define VM_SAR 0x17 				// sar reg, reg
#define VM_SAL 0x18 				// sal reg, reg
#define VM_PUSH 0x19 				// push reg
#define VM_PUSHI 0x1A 				// pushi imm32
#define VM_POP 0x1B 				// pop reg
#define VM_PUSHAD 0x1C 				// pushad
#define VM_POPAD 0x1D 				// popad
#define VM_JMP 0x1E 				// jmp reg (jump reg absolute)
#define VM_JMPI 0x1F 				// jmpi imm32 (jump imm32 absolute)
#define VM_JE 0x20 					// je reg (jump reg absolute)
#define VM_JZ VM_JE
#define VM_JEI 0x21 				// jei imm32 (jump imm32 absolute)
#define VM_JZI VM_JEI
#define VM_JNE 0x22 				// jne reg (jump reg absolute)
#define VM_JNEI 0x23                // jnei imm32 (jump imm32 absolute offset)
#define VM_JNZ VM_JNE
#define VM_JNZI VM_JNEI
#define VM_JL 0x24
#define VM_JLI 0x25
#define VM_JLE 0x26
#define VM_JLEI 0x27
#define VM_JNL 0x28
#define VM_JNLI 0x29
#define VM_JNLE 0x2A
#define VM_JNLEI 0x2B
#define VM_JG VM_JNLE
#define VM_JGI VM_JNLI
#define VM_JGE VM_JNL
#define VM_JGEI VM_JNLI
#define VM_JNG VM_JLE
#define VM_JNGI VM_JLEI
#define VM_JNGE VM_JL
#define VM_JNGEI VM_JLI
#define VM_JB 0x2C
#define VM_JBI 0x2D
#define VM_JBE 0x2E
#define VM_JBEI 0x2F
#define VM_JNB 0x30
#define VM_JNBI 0x31
#define VM_JNBE 0x32
#define VM_JNBEI 0x33
#define VM_JA VM_JNBE
#define VM_JAI VM_JNBEI
#define VM_JAE VM_JNB
#define VM_JAEI VM_JNBI
#define VM_JNA VM_JBE
#define VM_JNAI VM_JBEI
#define VM_JNAE VM_JB
#define VM_JNAEI VM_JBI
#define VM_JC 0x34
#define VM_JCI 0x35
#define VM_JNC 0x36
#define VM_JNCI 0x37
#define VM_JS 0x38
#define VM_JSI 0x39
#define VM_JNS 0x3A
#define VM_JNSI 0x3B
#define VM_JO 0x3C
#define VM_JOI 0x3D
#define VM_JNO 0x3E
#define VM_JNOI 0x3F
#define VM_DIV 0x40 				// div reg, reg
#define VM_IDIV	0x41				// idiv reg, reg (signed)
#define VM_MUL 0x42 			    // mul reg, reg
#define VM_IMUL 0x43				// imul reg, reg (signed)
#define VM_MOD 	0x44				// mod reg, reg (modulus)
#define VM_CALL 0x45                // call imm32 (absolute)
#define VM_RCALL 0x46               // call imm32 (relative)
#define VM_RET 0x47                 // ret
#define VM_XCHG 0x48                // xchg reg, reg

/*
 * Memory interaction opcodes.
 */
#define VM_LOADB 0x80               // loadb reg, mem[reg] (8 bits)
#define VM_LOADBI 0x81              // loadb reg, mem (8 bits)
#define VM_LOADW 0x82               // loadw reg, mem[reg] (16 bits)
#define VM_LOADWI 0x83              // loadw reg, mem (16 bits)
#define VM_LOADD 0x84               // loadd reg, mem[reg] (32 bits)
#define VM_LOADDI 0x85              // loadd reg, mem (32 bits)
#define VM_STORB 0x86               // storb mem, reg (8 bits)
#define VM_STORBI 0x87              // storib mem, imm8 (8 bits)
#define VM_STORW 0x88               // storw mem, reg (16 bits)
#define VM_STORWI 0x89              // storiw mem, imm16 (16 bits)
#define VM_STORD 0x8A               // stord mem, reg (32 bits)
#define VM_STORDI 0x8B              // storid mem, imm32 (32 bits)

/*
 * Special opcodes.
 */
#define VM_RC4K 0xFB                // rc4k mem, len
#define VM_RC4C 0xFC                // rc4d in, out, length, key
/*
 * Prints data out to the console output specifying the location of 
 * data in the virtual data section.
 */
#define VM_CONOUT 0xFD              // conout mem
#define VM_NOP 0xFE                 // nop
/* 
 * Starts passthru of unsupported OPCODES and executes natively.
 * 
 * To declare a region of native assembly, the relevant code must be 
 * surrounded by the defined assembly macros 'vm_passthru' and 
 * 'vm_passend'.
 */
#define VM_PASSTHRU 0xFF			// passthru

#endif // !__OPCODES_H__