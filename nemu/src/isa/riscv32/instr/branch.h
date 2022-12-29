def_EHelper(beq){
    rtl_jrelop(s, RELOP_EQ, dsrc1, dsrc2, id_dest->imm+ (rtlreg_t)s->pc);
}
def_EHelper(bge){
    rtl_jrelop(s, RELOP_GE, dsrc1, dsrc2, id_dest->imm+ (rtlreg_t)s->pc);
}

def_EHelper(bne){
    rtl_jrelop(s, RELOP_NE, dsrc1, dsrc2, id_dest->imm + (rtlreg_t)s->pc);
}

def_EHelper(blt){
    rtl_jrelop(s, RELOP_LT, dsrc1, dsrc2, id_dest->imm + (rtlreg_t)s->pc);
}
def_EHelper(bltu){
    rtl_jrelop(s, RELOP_LTU, dsrc1, dsrc2, id_dest->imm + (rtlreg_t)s->pc);
}