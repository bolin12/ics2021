// lui rd,imm U-type
def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
}
// auipc rd, imm (imm+pc->rd) U-type
def_EHelper(auipc){
  rtl_li(s, ddest, id_src1->imm + (rtlreg_t)s->pc);
}
// addi rd, rs1, imm I-type
def_EHelper(addi){
  rtl_addi(s, ddest, dsrc1, id_src2->simm);
}