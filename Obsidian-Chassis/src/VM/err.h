#ifndef __ERR_H__
#define __ERR_H__

#include <cstdint>
#include <string>
#include <map>

#define ERR_OPCODE_UNIMPLEMENTED 1
#define ERR_OPCODE_INVALID 2
#define ERR_CODE_OUT_OF_BOUNDS 3            // Code segmentation fault.
#define ERR_DATA_OUT_OF_BOUNDS 4            // Data segmentation fault.
#define ERR_STACK_UNDERFLOW 5               // Popping value beneath stack base.
#define ERR_STACK_OVERFLOW 6                // Stack pointer somehow greater than stack size.

extern std::map<uint32_t, std::string> errmsg;

std::string strerr(uint32_t code);

#endif // !__ERR_H__