#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) \
f(lui) f(auipc) \
f(lw) f(lbu) f(lh)\
f(sw) f(sh) f(sb)\
f(jal)  f(jalr)\
f(addi) f(sltiu) f(andi) f(xori) f(slli) f(srai) \
f(add) f(sub) f(mul) f(mulh) f(div) f(rem) f(sll) f(slt)f(sltu) f(and) f(xor) f(or)\
f(beq) f(bne) f(bge) f(blt)\
f(inv)  f(nemu_trap)

def_all_EXEC_ID();
