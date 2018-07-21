#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "opcodes.h"
#include "rc4.h"
#include "vm.h"

#define MAX_INPUT 0x100

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    // std::string flag = "VM{LooP_tH3_l00p}";
    // //std::vector<uint8_t> in(flag.begin(), flag.end());
    // std::vector<uint8_t> in = { 0xD8, 0x5f, 0x18, 0xc7, 0xc9, 0x22, 0xb2, 0x3f, 0x92, 0x2d, 0x87, 0x12, 0xea, 0x2c, 0x24, 0x11, 0x27, 0x83 };
    // std::vector<uint8_t> out(flag.length() + 1);

    // std::vector<uint8_t> key = { 0x9E, 0xB2, 0x7A, 0xC3, 0x11, 0x6C, 0xD5, 0x8A };
    // RC4 r;
    // r.set_for_cipher(8, key.data());
    // r.cipher(in.data(), flag.length() + 1, &out[0], key.data());

    // for (size_t i = 0; i < flag.length() + 1; i++)
    //     printf("%c ", out.at(i));
    // std::cout << "\n";

    VM vm;
    vm.start();

    return 0;
}