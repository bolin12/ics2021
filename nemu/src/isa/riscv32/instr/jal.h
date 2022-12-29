// jal rd, imm J-type unsigned??? or U
def_EHelper(jal) {
  rtl_li(s, ddest, id_src1->imm + (rtlreg_t)s->pc);
  rtl_jr(s, ddest);
  rtl_li(s, ddest, (rtlreg_t)s->pc+4);
}

// jarl rd, rs1, imm I-type
def_EHelper(jalr){
  rtl_addi(s, ddest, dsrc1, id_src2->simm);
  rtl_jr(s, ddest);
  rtl_li(s, ddest, (rtlreg_t)s->pc+4);
}