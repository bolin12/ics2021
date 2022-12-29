// addi rd, rs1, imm I-type
def_EHelper(addi){
  rtl_addi(s, ddest, dsrc1, id_src2->imm);
}
def_EHelper(sltiu){
  rtl_setrelopi(s, RELOP_LT, ddest, dsrc1, id_src2->imm);
}
def_EHelper(andi){
    rtl_andi(s, ddest, dsrc1, id_src2->imm);
}
def_EHelper(xori){
    rtl_xori(s, ddest, dsrc1, id_src2->imm);
}
def_EHelper(srai){
    rtl_srai(s, ddest, dsrc1, id_src2->imm);
}
def_EHelper(slli){
    rtl_slli(s, ddest, dsrc1, id_src2->imm);
}
def_EHelper(srli){
    printf("srli imm is %d\n", id_src2->imm);
    rtl_srli(s, ddest, dsrc1, id_src2->imm);
}