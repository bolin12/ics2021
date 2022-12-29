def_EHelper(add){
  rtl_add(s, ddest, dsrc1, dsrc2);
}
def_EHelper(sub){
  rtl_sub(s, ddest, dsrc1, dsrc2);
}
def_EHelper(mul){
  rtl_mulu_lo(s, ddest, dsrc1, dsrc2);
}
def_EHelper(mulh){
  rtl_muls_hi(s, ddest, dsrc1, dsrc2);
}
def_EHelper(div){
  rtl_divs_q(s, ddest, dsrc1, dsrc2);
}
def_EHelper(rem){
  rtl_divs_r(s, ddest, dsrc1, dsrc2);
}
def_EHelper(slt){
  rtl_setrelop(s, RELOP_LT, ddest, dsrc1, dsrc2);
}
def_EHelper(sltu){
  rtl_setrelop(s, RELOP_LTU, ddest, dsrc1, dsrc2);
}
def_EHelper(sll){
  rtl_sll(s, ddest, dsrc1, dsrc2);
}
def_EHelper(sra){
  rtl_sra(s, ddest, dsrc1, dsrc2);
}
def_EHelper(srl){
  rtl_srl(s, ddest, dsrc1, dsrc2);
}
def_EHelper(and){
  rtl_and(s, ddest, dsrc1, dsrc2);
}
def_EHelper(xor){
  rtl_xor(s, ddest, dsrc1, dsrc2);
}
def_EHelper(or){
  rtl_or(s, ddest, dsrc1, dsrc2);
}
