#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <iostream>

#include "err.h"
#include "opcodes.h"
#include "rc4.h"
#include "vm.h"

//#define DEBUG

void VM::panic(const uint32_t code) {
    /*
     * This could be OS-specific.
     */

#ifdef DEBUG
    std::cerr << "[-] Error (0x" << std::hex << code << std::dec << "): " << strerr(code) << ".\n";
#endif

    exit(code);
}

void VM::panic(const uint32_t code, const std::string& msg) {
    /*
     * This message could be application-specific.
     * e.g. Pop-up window for GUI applications.
     */

#ifdef DEBUG
    std::cerr << msg << "\n";
#else
    (void)msg;
#endif

    panic(code);
}

void VM::initialise(void) {
    /*
     * Zero all registers and in VM context.
     */
    for (int i = 0; i < NUM_REGISTERS; i++) {
        m_vreg[i] = 0;
        m_vctx.vreg[i] = 0;
    }

    /*
     * Point program counter to beginning of code section  
     * and in VM context.
     */
    m_vpc = 0;
    m_vctx.vpc = 0;

    /*
     * Point stack pointer to the top of the stack (0).
     */
    m_vsp = 0;
    m_vctx.vsp = 0;

    /*
     * Zero EFLAGS registers.
     */
    m_veflags = { 0, 0, 0, 0, 0, 0, 0, 0 };
    m_vctx.veflags = { 0, 0, 0, 0, 0, 0, 0, 0 };

    /*
     * Clear code section.
     */
    //m_vcode.clear();

    /*
     * Clear global data section.
     */
    m_vdata.clear();
    // TODO: fix size value to be dynamic?
    m_vdata.resize(DATA_SECTION_SIZE);

    /*
     * Clear stack section.
     */
    m_vstack.clear();
    m_vstack.resize(0);
}

OPCODE VM::fetch(void) {
    /*
     * Check that the program counter does not exceed the size of the 
     * code section. If exceeded, throw an exception!
     */
    if (m_vpc > m_vsize)
        panic(ERR_CODE_OUT_OF_BOUNDS);

#ifdef DEBUG
    std::cout << "[*] Executing opcode: 0x" << std::hex << (int)m_vcode[m_vpc] << std::dec << "\n";
#endif

    /*
     * Return the instruction in the code section pointed to by the 
     * program counter.
     */
    return m_vcode[m_vpc];
}

bool VM::execute(const OPCODE opcode) {
    static RC4 r;

    switch (opcode) {
        case VM_MOV:
            m_vreg[m_vcode[m_vpc + 1]] = m_vreg[m_vcode[m_vpc + 2]];
            m_vpc += 3;
            break;

        case VM_MOVI:
            //memcpy(&m_vreg[m_vcode[m_vpc + 1]], &m_vcode[m_vpc + 2], 4);
            m_vreg[m_vcode[m_vpc + 1]] = *(IMM32 *)&m_vcode[m_vpc + 2];
            m_vpc += 6; 
            break;

        case VM_ADD:
            //m_vreg[m_vcode[m_vpc + 1]] += m_vreg[m_vcode[m_vpc + 2]];
            m_vreg[m_vcode[m_vpc + 1]] = ADD(m_vreg[m_vcode[m_vpc + 1]], m_vreg[m_vcode[m_vpc + 2]]);
            m_vpc += 3;
            break;

        case VM_ADDI:
            // TODO: check if correct
            //m_vreg[m_vcode[m_vpc + 1]] += *(int32_t *)&m_vcode[m_vpc + 2];
            m_vreg[m_vcode[m_vpc + 1]] = ADD(m_vreg[m_vcode[m_vpc + 1]], *(IMM32 *)&m_vcode[m_vpc + 2]);
            m_vpc += 6;
            break;

        case VM_SUB:
            // TODO: carry flag
            m_vreg[m_vcode[m_vpc + 1]] -= m_vreg[m_vcode[m_vpc + 2]];
            m_veflags.sign = (int32_t)m_vreg[m_vcode[m_vpc + 1]] >= 0 ? 0 : 1;               // If result >= 0, unset sign flag (positive), else set sign flag.
            m_veflags.zero = m_vreg[m_vcode[m_vpc + 1]] ? 0 : 1;                    // If result == 0, set zero flag.
            m_vpc += 3;
            break;

        case VM_SUBI:
            // TODO: check if correct; carry flag
            m_vreg[m_vcode[m_vpc + 1]] -= *(IMM32 *)&m_vcode[m_vpc + 2];
            m_veflags.sign = (int32_t)m_vreg[m_vcode[m_vpc + 1]] >= 0 ? 0 : 1;               // If result >= 0, unset sign flag (positive), else set sign flag.
            m_veflags.zero = m_vreg[m_vcode[m_vpc + 1]] ? 0 : 1;                    // If result == 0, set zero flag.
            m_vpc += 6;
            break;

        case VM_ADC:
            m_vreg[m_vcode[m_vpc + 1]] = ADC(m_vreg[m_vcode[m_vpc + 1]], m_vreg[m_vcode[m_vpc + 2]]);
            m_vpc += 3;
            break;

        case VM_SBB:
            // TODO
            panic(ERR_OPCODE_UNIMPLEMENTED);
            break;

        case VM_INC:
            m_vreg[m_vcode[m_vpc + 1]] += 1;
            m_vpc += 2;
            break;

        case VM_DEC:
            m_vreg[m_vcode[m_vpc + 1]] -= 1;
            m_vpc += 2;
            break;

        case VM_CMP:
            /*
             * If equal, set EFLAGS zero flag to 1.
             * Else, set EFLAGS zero flag to 0.
             */
            m_veflags.zero = m_vreg[m_vcode[m_vpc + 1]] - *(IMM32 *)&m_vcode[m_vpc + 2] ?  0 : 1;           // Modify zero flag.
            m_veflags.sign = m_vreg[m_vcode[m_vpc + 1]] >= *(IMM32 *)&m_vcode[m_vpc + 2] ?  0 : 1;          // Modify sign flag.
            m_vpc += 6;
            break;

        case VM_LEA:
            m_vreg[m_vcode[m_vpc + 1]] = *(IMM32 *)&m_vreg[m_vcode[m_vpc + 2]];
            m_vpc += 3;
            // TODO
            //panic(ERR_OPCODE_UNIMPLEMENTED);
            break;

        case VM_NEG:
            m_vreg[m_vcode[m_vpc + 1]] = NEG(m_vreg[m_vcode[m_vpc + 1]]);
            m_vpc += 2;
            break;

        case VM_OR:
            m_vreg[m_vcode[m_vpc + 1]] = OR(m_vreg[m_vcode[m_vpc + 1]], m_vreg[m_vcode[m_vpc + 2]]);
            m_vpc += 3;
            break;

        case VM_AND:
            m_vreg[m_vcode[m_vpc + 1]] = AND(m_vreg[m_vcode[m_vpc + 1]], m_vreg[m_vcode[m_vpc + 2]]);
            m_vpc += 3;
            break;

        case VM_NOT:
            m_vreg[m_vcode[m_vpc + 1]] = NOT(m_vreg[m_vcode[m_vpc + 1]]);
            m_vpc += 2;
            break;

        case VM_NOR:
            m_vreg[m_vcode[m_vpc + 1]] = NOR(m_vreg[m_vcode[m_vpc + 1]], m_vreg[m_vcode[m_vpc + 2]]);
            m_vpc += 3;
            break;

        case VM_XOR:
            m_vreg[m_vcode[m_vpc + 1]] = XOR(m_vreg[m_vcode[m_vpc + 1]], m_vreg[m_vcode[m_vpc + 2]]);
            m_vpc += 3;
            break;

        case VM_XORI:
            m_vreg[m_vcode[m_vpc + 1]] = XOR(m_vreg[m_vcode[m_vpc + 1]], *(IMM32 *)&m_vcode[m_vpc + 2]);
            m_vpc += 6;
            break;

        case VM_TEST:
            m_veflags.zero = AND(m_vreg[m_vcode[m_vpc + 1]], m_vreg[m_vcode[m_vpc + 2]]) ? 0 : 1;
            m_vpc += 3;
            break;

        case VM_SHR:
            m_vreg[m_vcode[m_vpc + 1]] >>= m_vreg[m_vcode[m_vpc + 2]];
            m_vpc += 3;
            break;

        case VM_SHL:
            m_vreg[m_vcode[m_vpc + 1]] <<= m_vreg[m_vcode[m_vpc + 2]];
            m_vpc += 3;
            break;

        case VM_SAR:
            // TODO
            panic(ERR_OPCODE_UNIMPLEMENTED);
            break;

        case VM_SAL:
            // TODO
            panic(ERR_OPCODE_UNIMPLEMENTED);
            break;

        case VM_PUSH:
            m_vstack.push_back(m_vreg[m_vcode[m_vpc + 1]]);                         // Add value to stack.
            m_vsp++;                                                                // Increment stack pointer.
            m_vpc += 2;
            break;

        case VM_PUSHI:
            m_vstack.push_back(*(IMM32 *)&m_vcode[m_vpc + 1]);                      // Add value to stack.
            m_vsp++;                                                                // Increment stack pointer.
            m_vpc += 5;
            break;

        case VM_POP:
            if (m_vsp == 0 || m_vstack.empty())                                     // Check stack pointer or size.
                panic(ERR_STACK_UNDERFLOW);                                         // Panic on attempt to pop from invalid position.
            else if (m_vsp > m_vstack.size())
                panic(ERR_STACK_OVERFLOW);
            m_vreg[m_vcode[m_vpc + 1]] = m_vstack[m_vsp - 1];                       // Obtain value.
            //m_vstack.pop_back();                                                  // Don't actually remove value from stack.
            m_vsp--;                                                                // Decrement stack pointer.
            m_vpc += 2;
            break;

        case VM_PUSHAD:
            // TODO
            panic(ERR_OPCODE_UNIMPLEMENTED);
            break;

        case VM_POPAD:
            // TODO
            if (m_vsp == 0 || m_vstack.empty())                                     // Check stack pointer or size.
                panic(ERR_STACK_UNDERFLOW);                                         // Panic on attempt to pop from invalid position.
            panic(ERR_OPCODE_UNIMPLEMENTED);
            break;

        case VM_JMP:
            m_vpc += 2;
            m_vpc = m_vreg[m_vcode[m_vpc - 1]];
            break;

        case VM_JMPI:
            m_vpc += 5;
            m_vpc = *(IMM32 *)&m_vcode[m_vpc - 4];
            break;

        case VM_JE:
            m_vpc += 2;
            m_vpc = m_veflags.zero ? m_vreg[m_vcode[m_vpc - 1]] : m_vpc;
            break;

        case VM_JEI:
            m_vpc += 5;
            m_vpc = m_veflags.zero ? *(IMM32 *)&m_vcode[m_vpc - 4] : m_vpc;
            break;

        case VM_JNE:
            m_vpc += 2;
            m_vpc = m_veflags.zero ? m_vpc : m_vreg[m_vcode[m_vpc - 1]];
            break;

        case VM_JNEI:
            m_vpc += 5;
            m_vpc = m_veflags.zero ? m_vpc : *(IMM32 *)&m_vcode[m_vpc - 4];
            break;

        case VM_DIV:
            m_vreg[m_vcode[m_vpc + 1]] /= m_vreg[m_vcode[m_vpc + 2]];
            m_vpc += 3;
            break;

        case VM_IDIV:
            m_vreg[m_vcode[m_vpc + 1]] /= (IMM32)m_vreg[m_vcode[m_vpc + 2]];
            m_vpc += 3;
            break;

        case VM_MUL:
            m_vreg[m_vcode[m_vpc + 1]] *= m_vreg[m_vcode[m_vpc + 2]];
            m_vpc += 3;
            break;

        case VM_IMUL:
            m_vreg[m_vcode[m_vpc + 1]] *= (IMM32)m_vreg[m_vcode[m_vpc + 2]];      
            m_vpc += 3;
            break;

        case VM_MOD:
            // TODO
            panic(ERR_OPCODE_UNIMPLEMENTED);
            break;

        case VM_CALL:
            m_vpc += 5;                                                             // Point pc to next instruction after return.
            m_vstack.push_back(m_vpc);                                              // Save pc onto stack for return.
            m_vsp++;                                                                // Increment stack pointer.
            m_vpc = *(IMM32 *)&m_vcode[m_vpc - 4];                                  // Set pc to the start routine (absolute).
            break;

        case VM_RCALL:
            m_vpc += 5;                                                             // Point pc to next instruction after return.
            m_vstack.push_back(m_vpc);                                              // Save pc onto stack for return.
            m_vsp++;                                                                // Increment stack pointer.
            m_vpc += *(IMM32 *)&m_vcode[m_vpc - 4];                                 // Set pc to the start routine (relative).
            break;

        case VM_RET:
            m_vpc = m_vstack[m_vsp - 1];                                            // Retrieve saved pc value.
            //m_vstack.pop_back();                                                  // Don't actually remove value from stack.
            m_vsp--;                                                                // Decrement stack pointer.
            break;
        
        case VM_XCHG:
            m_vreg[m_vcode[m_vpc + 1]] = XOR(m_vreg[m_vcode[m_vpc + 1]], m_vreg[m_vcode[m_vpc + 2]]);               // XOR swap.
            m_vreg[m_vcode[m_vpc + 2]] = XOR(m_vreg[m_vcode[m_vpc + 2]], m_vreg[m_vcode[m_vpc + 1]]);
            m_vreg[m_vcode[m_vpc + 1]] = XOR(m_vreg[m_vcode[m_vpc + 1]], m_vreg[m_vcode[m_vpc + 2]]);
            m_vpc += 3;
            break;

        case VM_LOADB:
            if (m_vcode[m_vpc + 2] > m_vdata.size() - 1)                            // Check memory access location.
                panic(ERR_DATA_OUT_OF_BOUNDS);
            m_vreg[m_vcode[m_vpc + 1]] = *(IMM8 *)&m_vdata[m_vreg[m_vcode[m_vpc + 2]]];
            m_vpc += 3;
            break;

        case VM_LOADBI:
            if (m_vcode[m_vpc + 2] > m_vdata.size() - 1)                            // Check memory access location.
                panic(ERR_DATA_OUT_OF_BOUNDS);
            m_vreg[m_vcode[m_vpc + 1]] = *(IMM8 *)&m_vdata[m_vcode[m_vpc + 2]];
            m_vpc += 3;
            break;

        case VM_LOADW:
            if (m_vcode[m_vpc + 2] > m_vdata.size() - 1)                            // Check memory access location.
                panic(ERR_DATA_OUT_OF_BOUNDS);
            m_vreg[m_vcode[m_vpc + 1]] = *(IMM16 *)&m_vdata[m_vreg[m_vcode[m_vpc + 2]]];
            m_vpc += 3;
            break;

        case VM_LOADWI:
            if (m_vcode[m_vpc + 2] > m_vdata.size() - 1)                            // Check memory access location.
                panic(ERR_DATA_OUT_OF_BOUNDS);
            m_vreg[m_vcode[m_vpc + 1]] = *(IMM16 *)&m_vdata[m_vcode[m_vpc + 2]];
            m_vpc += 3;
            break;

        case VM_LOADD:
            if (m_vcode[m_vpc + 2] > m_vdata.size() - 1)                            // Check memory access location.
                panic(ERR_DATA_OUT_OF_BOUNDS);
            m_vreg[m_vcode[m_vpc + 1]] = *(IMM32 *)&m_vdata[m_vreg[m_vcode[m_vpc + 2]]];
            m_vpc += 3;
            break;

        case VM_LOADDI:
            if (m_vcode[m_vpc + 2] > m_vdata.size() - 1)                            // Check memory access location.
                panic(ERR_DATA_OUT_OF_BOUNDS);
            m_vreg[m_vcode[m_vpc + 1]] = *(IMM32 *)&m_vdata[m_vcode[m_vpc + 2]];
            m_vpc += 3;
            break;

        case VM_STORB:
            if (m_vcode[m_vpc + 1] > m_vdata.size() - 1)                            // Check memory access location.
                panic(ERR_DATA_OUT_OF_BOUNDS);
            *(IMM8 *)&m_vdata[m_vcode[m_vpc + 1]] = (IMM8)m_vreg[m_vcode[m_vpc + 2]];
            m_vpc += 3;
            break;
        
        case VM_STORBI:
            if (m_vcode[m_vpc + 1] > m_vdata.size() - 1)                            // Check memory access location.
                panic(ERR_DATA_OUT_OF_BOUNDS);
            *(IMM8 *)&m_vdata[m_vcode[m_vpc + 1]] = *(IMM8 *)&m_vcode[m_vpc + 2];
            m_vpc += 3;
            break;

        case VM_STORW:
            if (m_vcode[m_vpc + 1] > m_vdata.size() - 1)                            // Check memory access location.
                panic(ERR_DATA_OUT_OF_BOUNDS);
            *(IMM16 *)&m_vdata[m_vcode[m_vpc + 1]] = (IMM16)m_vreg[m_vcode[m_vpc + 2]];
            m_vpc += 3;
            break;

        case VM_STORWI:
            if (m_vcode[m_vpc + 1] > m_vdata.size() - 1)                            // Check memory access location.
                panic(ERR_DATA_OUT_OF_BOUNDS);
            *(IMM16 *)&m_vdata[m_vcode[m_vpc + 1]] = *(IMM16 *)&m_vcode[m_vpc + 2];
            m_vpc += 4;
            break;

        case VM_STORD:
            if (m_vcode[m_vpc + 1] > m_vdata.size() - 1)                            // Check memory access location.
                panic(ERR_DATA_OUT_OF_BOUNDS);
            *(IMM32 *)&m_vdata[m_vcode[m_vpc + 1]] = (IMM32)m_vreg[m_vcode[m_vpc + 2]];
            m_vpc += 3;
            break;

        case VM_STORDI:
            if (m_vcode[m_vpc + 1] > m_vdata.size() - 1)                            // Check memory access location.
                panic(ERR_DATA_OUT_OF_BOUNDS);
            *(IMM32 *)&m_vdata[m_vcode[m_vpc + 1]] = *(IMM32 *)&m_vcode[m_vpc + 2];
            m_vpc += 6;
            break;

        case VM_HLT:

#ifdef DEBUG
            std::cout << "[*] Halting VM...\n";
#endif

            return false;

        case VM_RC4K:
            r.set_for_cipher(*(IMM32 *)&m_vcode[m_vpc + 2], (uint8_t *)&m_vdata[m_vcode[m_vpc + 1]]);
            m_vpc += 6;
            break;

        case VM_RC4C:
            r.cipher((uint8_t *)&m_vdata[m_vcode[m_vpc + 1]], *(IMM32 *)&m_vcode[m_vpc + 3], (uint8_t *)&m_vdata[m_vcode[m_vpc + 2]], (uint8_t *)&m_vdata[m_vcode[m_vpc + 7]]);
            m_vpc += 8;
            break;

        case VM_CONOUT:
            std::cout << (char *)&m_vdata[m_vcode[m_vpc + 1]];
            m_vpc += 2;
            break;

        case VM_NOP:
            m_vpc++;
            break;

        case VM_PASSTHRU:
            /*
             * Allow (unsupported) native instructions to pass 
             * through. Call the address where the native 
             * instructions start and let vm_passthru macro 
             * return back here when completed.
             */
            ((PASSTHRU)(&m_vcode[m_vpc + 5]))();                                    // Call handler with location of the code.
            m_vpc += 6 + *(IMM32 *)&m_vcode[m_vpc + 1];                             // Move program counter size of native instructions + vm_passthru and vm_passend.
            break;

        default:
            panic(ERR_OPCODE_INVALID);                                              // Invalid instruction! Panic!
            break;
    }

    /*
     * Default exit return value.
     * Returns true to continue emulation.
     */
    return true;
}

uint32_t VM::loop(void) {
    /*
     * Loop fetch and execute until halted.
     */
    while (execute(fetch()));

    /*
     * Return the value in vreg[0] containing exit status.
     */
    return m_vreg[0];
}

uint32_t VM::start(const std::vector<uint8_t>& data) {

#ifdef DEBUG
    std::cout << "[*] Initialising VM...\n";
#endif

    /*
     * Initialise the VM and assign the set of instructions.
     */
    initialise();

    /*
     * Copy data into virtual data section.
     */
    for (size_t i = 0; i < data.size(); i++)
        m_vdata[i] = data[i];

    /*
     * Point code to .text section that contains the virtualised 
     * intructions and set size for code bounds checking.
     */
    m_vcode = &_vm_start;
    m_vsize = _vm_size;

#ifdef DEBUG
    std::cout << "[*] Starting VM execution cycle...\n";
#endif

    return loop();
}

uint32_t VM::start() {
    /*
     * Start CPU loop cycle and return exit value.
     */
    return VM::start(std::vector<uint8_t>{ 0 });
}