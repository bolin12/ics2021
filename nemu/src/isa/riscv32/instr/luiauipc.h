// lui rd,imm U-type
def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
}
// auipc rd, imm (imm+pc->rd) U-type
def_EHelper(auipc){
  rtl_li(s, ddest, id_src1->imm + (rtlreg_t)s->pc);
}
