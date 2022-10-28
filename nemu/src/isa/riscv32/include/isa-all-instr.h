#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) \
f(lui) f(auipc) \
f(lw)  f(sw) \
f(jal)  f(jalr)\
f(addi)  \
f(add) f(sub) f(sltiu)\
f(beq) f(bne)\
f(inv)  f(nemu_trap)

def_all_EXEC_ID();
