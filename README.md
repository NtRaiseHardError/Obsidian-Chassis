# Obsidian-Chassis
Simple emulator for a custom instruction set

NOTE: rc4 implementation is optional and was implemented for a challenge (`crackme2.vasm`).

# How-to Compile

1. Compile virtualised instructions using NASM.

`nasm -felf32 -o FILE.o FILE.vasm`

2. Compile binary with virtualised object code.

`g++ -Wall -Werror -Wextra -m32 -O -g -o vm vm.cpp main.cpp err.cpp rc4.cpp FILE.o`

---

## TODO

1. Implement remaining jumps.
