#include "err.h"

std::map<uint32_t, std::string> errmsg {
    { ERR_OPCODE_UNIMPLEMENTED, "Unimplemented opcode" },
    { ERR_OPCODE_INVALID, "Invalid opcode" },
    { ERR_CODE_OUT_OF_BOUNDS, "Code section exceeded bounds" },
    { ERR_DATA_OUT_OF_BOUNDS, "Data section exceeded bounds" },
    { ERR_STACK_UNDERFLOW, "Stack underflow" },
    { ERR_STACK_OVERFLOW, "Stack overflow" }
};

std::string strerr(uint32_t code) {
    return errmsg.find(code)->second;
}