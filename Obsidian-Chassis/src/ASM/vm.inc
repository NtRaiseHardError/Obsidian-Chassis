%define vm_reg0 0
%define vm_reg1 1
%define vm_reg2 2
%define vm_reg3 3
%define vm_reg4 4
%define vm_reg5 5
%define vm_reg6 6
%define vm_reg7 7
%define vm_reg8 8
%define vm_reg9 9
%define vm_reg10 10
%define vm_reg11 11
%define vm_reg12 12
%define vm_reg13 13
%define vm_reg14 14
%define vm_reg15 15

%macro vm_hlt 0
    db 0x00
%endmacro

%macro vm_mov 2
    db 0x01, %1, %2
%endmacro

%macro vm_movi 2
    db 0x02, %1
    dd %2
%endmacro

%macro vm_add 2
    db 0x03, %1, %2
%endmacro

%macro vm_addi 2
    db 0x04, %1
    dd %2
%endmacro

%macro vm_sub 2
    db 0x05, %1, %2
%endmacro

%macro vm_subi 2
    db 0x06, %1
    dd %2
%endmacro

%macro vm_adc 2
    db 0x07, %1, %2
%endmacro

%macro vm_sbb 2
    db 0x08, %1, %2
%endmacro

%macro vm_inc 1
    db 0x09, %1
%endmacro

%macro vm_dec 1
    db 0x0A, %1
%endmacro

%macro vm_cmp 2
    db 0x0B, %1
    dd %2
%endmacro

%macro vm_lea 2
    db 0x0C, %1, %2
%endmacro

%macro vm_neg 1
    db 0x0D, %1
%endmacro

%macro vm_or 2
    db 0x0E, %1, %2
%endmacro

%macro vm_and 2
    db 0x0F, %1, %2
%endmacro

%macro vm_not 1
    db 0x10, %1
%endmacro

%macro vm_nor 2
    db 0x11, %1, %2
%endmacro

%macro vm_xor 2
    db 0x12, %1, %2
%endmacro

%macro vm_xori 2
    db 0x13, %1
    dd %2
%endmacro

%macro vm_test 2
    db 0x14, %1, %2
%endmacro

%macro vm_shr 2
    db 0x15, %1, %2
%endmacro

%macro vm_shl 2
    db 0x16, %1, %2
%endmacro

%macro vm_sar 2
    db 0x17, %1, %2
%endmacro

%macro vm_sal 2
    db 0x18, %1, %2
%endmacro

%macro vm_push 1
    db 0x19, %1
%endmacro

%macro vm_pushi 1
    db 0x1A
    dd %1
%endmacro

%macro vm_pop 1
    db 0x1B, %1
%endmacro

%macro vm_pushad 0
    db 0x1C
%endmacro

%macro vm_popad 0
    db 0x1D
%endmacro

%macro vm_jmp 1
    db 0x1E, %1
%endmacro

%macro vm_jmpi 1
    db 0x1F
    dd %1
%endmacro

%macro vm_je 1
    db 0x20, %1
%endmacro

%macro vm_jz 1
    vm_je, %1
%endmacro

%macro vm_jei 1
    db 0x21
    dd %1
%endmacro

%macro vm_jzi 1
    vm_jei %1
%endmacro

%macro vm_jne 1
    db 0x22, %1
%endmacro

%macro vm_jnei 1
    db 0x23
    dd %1
%endmacro

%macro vm_jnz 1
    vm_jne, %1
%endmacro

%macro vm_jnzi 1
    vm_jnei %1
%endmacro

%macro vm_jl 1
    db 0x24, %1
%endmacro

%macro vm_jli 1
    db 0x25
    dd %1
%endmacro

%macro vm_jle 1
    db 0x26, %1
%endmacro

%macro vm_jlei 1
    db 0x27
    dd %1
%endmacro

%macro vm_jnl 1
    db 0x28, %1
%endmacro

%macro vm_jnli 1
    db 0x29
    dd %1
%endmacro

%macro vm_jnle 1
    db 0x2A, %1
%endmacro

%macro vm_jnlei 1
    db 0x2B
    dd %1
%endmacro

%macro vm_jg 1
    vm_jnle %1
%endmacro

%macro vm_jgi 1
    vm_vm_jnlei %1
%endmacro

%macro vm_jge 1
    vm_jnl %1
%endmacro

%macro vm_jgei 1
    vm_jnli %1
%endmacro

%macro vm_jng 1
    vm_jle %1
%endmacro

%macro vm_jngi 1
    vm_jlei %1
%endmacro

%macro vm_jnge 1
    vm_jl %1
%endmacro

%macro vm_jngei 1
    vm_jli %1
%endmacro

%macro vm_jb 1
    db 0x2C, %1
%endmacro

%macro vm_jbi 1
    db 0x2D
    dd %1
%endmacro

%macro vm_jbe 1
    db 0x2E, %1
%endmacro

%macro vm_jbei 1
    db 0x2F
    dd %1
%endmacro

%macro vm_jnb 1
    db 0x30, %1
%endmacro

%macro vm_jnbI 1
    db 0x31, %1
%endmacro

%macro vm_jnbe 1
    db 0x32, %1
%endmacro

%macro vm_jnbei 1
    db 0x33, %1
%endmacro

%macro vm_ja 1
    vm_jnbe %1
%endmacro

%macro vm_jai 1
    vm_jnbei %1
%endmacro

%macro vm_jae 1
    vm_jnb %1
%endmacro

%macro vm_jaei 1
    vm_jnbi %1
%endmacro

%macro vm_jna 1
    vm_jbe %1
%endmacro

%macro vm_jnai 1
    vm_jbei %1
%endmacro

%macro vm_jnae 1
    vm_jb %1
%endmacro

%macro vm_jnaei 1
    vm_jbi %1
%endmacro

%macro vm_jc 1
    db 0x34, %1
%endmacro

%macro vm_jci 1
    db 0x35, %1
%endmacro

%macro vm_jnc 1
    db 0x36, %1
%endmacro

%macro vm_jnci 1
    db 0x37, %1
%endmacro

%macro vm_js 1
    db 0x38, %1
%endmacro

%macro vm_jsi 1
    db 0x39, %1
%endmacro

%macro vm_jns 1
    db 0x3A, %1
%endmacro

%macro vm_jnsi 1
    db 0x3B, %1
%endmacro

%macro vm_jo 1
    db 0x3C, %1
%endmacro

%macro vm_joi 1
    db 0x3D, %1
%endmacro

%macro vm_jno 1
    db 0x3E, %1
%endmacro

%macro vm_jnoi 1
    db 0x3F, %1
%endmacro

%macro vm_div 2
    db 0x40, %1, %2
%endmacro

%macro vm_idiv 2
    db 0x41, %1, %2
%endmacro

%macro vm_mul 2
    db 0x42, %1, %2
%endmacro

%macro vm_imul 2
    db 0x43, %1, %2
%endmacro

%macro vm_mod 2
    db 0x44, %1, %2
%endmacro

%macro vm_call 1
    db 0x45
    dd %1
%endmacro

%macro vm_rcall 1
    db 0x46
    dd %1
%endmacro

%macro vm_ret 0
    db 0x47
%endmacro

%macro vm_xchg 2
    db 0x48, %1, %2
%endmacro

%macro vm_loadb 2
    db 0x80, %1, %2
%endmacro

%macro vm_loadbi 2
    db 0x81, %1, %2
%endmacro

%macro vm_loadw 2
    db 0x82, %1, %2
%endmacro

%macro vm_loadwi 2
    db 0x83, %1, %2
%endmacro

%macro vm_loadd 2
    db 0x84, %1, %2
%endmacro

%macro vm_loaddi 2
    db 0x85, %1, %2
%endmacro

%macro vm_storb 2
    db 0x86, %1, %2
%endmacro

%macro vm_storbi 2
    db 0x87, %1, %2
%endmacro

%macro vm_storw 2
    db 0x88, %1, %2
%endmacro

%macro vm_storwi 2
    db 0x89, %1
    dw %2
%endmacro

%macro vm_stord 2
    db 0x8A, %1, %2
%endmacro

%macro vm_stordi 2
    db 0x8B, %1
    dd %2
%endmacro

%macro vm_rc4k 2
    db 0xFB, %1
    dd %2
%endmacro

%macro vm_rc4c 4
    db 0xFC, %1, %2
    dd %3
    db %4
%endmacro

%macro vm_conout 1
    db 0xFD, %1
%endmacro

%macro vm_nop 0
    db 0xFE
%endmacro

%macro vm_passthru 1
    db 0xFF
    dd %1
%endmacro

%macro vm_passend 0
    ret
%endmacro

