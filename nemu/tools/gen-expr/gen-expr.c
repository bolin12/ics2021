#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = 0u; "
"  result = result + %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

static int buf_offset = 0;
static int op_limit=20;

static void gen(char str){
  *(buf+buf_offset) = str;
  buf_offset++;

}



static void gen_num(){
  if(rand()%6==0)gen(' ');
  if(rand()%50==0){
    gen('0');
    gen('u');
    return;
  }
  int len = rand()%3+2;
  gen((char)(rand()%9+'1'));
  for(int i=2;i<=len;i++){
    gen((char)(rand()%10+'0'));
  }
  gen('u');
  if(rand()%6==0)gen(' ');
}

static void gen_rand_op(){
  if(rand()%6==0)gen(' ');
  switch(rand()%4){
    case 0:gen('+');break;
    case 1:gen('-');break;
    case 2:gen('*');break;
    case 3:gen('/');break;
    default: break;
  }
  if(rand()%6==0)gen(' ');
}

static void gen_rand_expr() {
  int cur_rand;
  if(op_limit==0)cur_rand = rand()%2;
  else cur_rand = rand()%3;
  switch(cur_rand){
    case 0:gen_num();break;
    case 1:gen('(');gen_rand_expr();gen(')');break;
    case 2:{
      op_limit--;
      gen_rand_expr();gen_rand_op();gen_rand_expr();break;
    }
  }
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    buf_offset=0;
    op_limit = 40;
    gen_rand_expr();
    buf[buf_offset]='\0';
    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr -Wall -Werror");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    int fscanf_return  = fscanf(fp, "%d", &result);// to avoid warning
    if(fscanf_return==0);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
