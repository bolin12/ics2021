#include <common.h>
#include "monitor/sdb/sdb.h"
#include <isa.h>
#include <cpu/cpu.h>
void init_monitor(int, char *[]);
void am_init_monitor();
void engine_start();
int is_exit_status_bad();

int main(int argc, char *argv[])
{
  // printf("hello world!\n");
  // FILE *fp = fopen("../tools/gen-expr/input", "r");

  // // if (fp == NULL)exit(EXIT_FAILURE);

  // char *line = malloc(256);
  // size_t len = 0;
  // ssize_t read;
  // while ((read = getline(&line, &len, fp)) != -1)
  // {
  //   printf("Retrieved line of length %zu:\n", read);
  //   printf("%s", line);
  // }

  // fclose(fp);
  // if (line)free(line);
  FILE *fp = fopen("/home/lbl/courses/ics2021/nemu/tools/gen-expr/input", "r");
  if (fp == NULL)
  {
    perror("Unable to open file!");
    exit(1);
  }

  char chunk[256];
  char *expr_to_cal;
  while (fgets(chunk, sizeof(chunk), fp) != NULL)
  {
    char *res = strtok(chunk, " ");
    word_t res_val = atoi(res); 
    expr_to_cal = chunk+ strlen(res)+1;
    printf("res is:%s, expr_to_cal is:%s\n", res, expr_to_cal);
    
    bool *success = malloc(1);
    *success = false;
    printf("here?\n");
    word_t eval_val = expr(expr_to_cal, success);
    
    // if(*success == false){
    //   printf("here can be any questions?\n");
    //   return 0;
    // }
    if(eval_val == res_val) printf("same val of %u, %u!\n", eval_val, res_val);
    fputs(chunk, stdout);
    fputs("|*\n", stdout); // marker string used to show where the content of the chunk array has ended
  }

  fclose(fp);
  /* Initialize the monitor. */
  // #ifdef CONFIG_TARGET_AM
  //   am_init_monitor();
  // #else
  //   init_monitor(argc, argv);
  // #endif

  //   /* Start engine. */
  //   engine_start();

  //   return is_exit_status_bad();
  return 0;
}
