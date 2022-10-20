#include <common.h>
#include "monitor/sdb/sdb.h"

void init_monitor(int, char *[]);
void am_init_monitor();
void engine_start();
int is_exit_status_bad();

int main(int argc, char *argv[])
{

  /* Initialize the monitor. */
#ifdef CONFIG_TARGET_AM
  am_init_monitor();
#else
  init_monitor(argc, argv);
#endif

  /* Start engine. */
  engine_start();


  // FILE *fp = fopen("/home/lbl/courses/ics2021/nemu/tools/gen-expr/input", "r");
  // FILE *fp_ = fopen("/home/lbl/courses/ics2021/nemu/tools/gen-expr/input1", "w+");
  // if (fp == NULL)
  // {
  //   perror("Unable to open file!");
  //   exit(1);
  // }

  // char chunk[512];
  // char *expr_to_cal;
  // while (fgets(chunk, sizeof(chunk), fp) != NULL)
  // {
  //   chunk[strlen(chunk)-1]='\0';
  //   char *res = strtok(chunk, " ");
  //   word_t res_val = atoi(res);
  //   expr_to_cal = chunk + strlen(res) + 1;
  //   bool *success = malloc(1);
  //   *success = false;
  //   printf("char to make:%s\n", expr_to_cal);
  //   word_t eval_val = expr(expr_to_cal, success);
  //   if (eval_val == res_val)
  //     printf("same val of %u, %u!\n", eval_val, res_val);
  //   else{
  //     fputs(chunk, fp_);
  //     fputs(" ", fp_);
  //     fputs(expr_to_cal, fp_);
  //     fputs("\n", fp_);
  //     printf("diff  val of %u, %u!\n\n", eval_val, res_val);
  //   }
  // }
  

  // fclose(fp);
  // fclose(fp_);


  return is_exit_status_bad();

}
