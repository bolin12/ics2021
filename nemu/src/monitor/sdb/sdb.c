#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"

static int is_batch_mode = false;

static int cmd_help(char *args);
static int cmd_si(char *args);
static int cmd_info(char *args);
static int cmd_x(char *args);
static int cmd_p(char *args);
static int cmd_w(char *args);
static int cmd_d(char *args);


/* We use the `readline' library to provide more flexibility to read from stdin. */
static char *rl_gets()
{
  static char *line_read = NULL;

  if (line_read)
  {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read)
  {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args)
{
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args)
{
  return -1;
}

static struct
{
  const char *name;
  const char *description;
  int (*handler)(char *);
} cmd_table[] = {
    {"help", "Display informations about all supported commands", cmd_help},
    {"c", "Continue the execution of the program", cmd_c},
    {"q", "Exit NEMU", cmd_q},
    {"si", "Run N steps and pause, N is 1 by default", cmd_si},
    {"info", "Print state of registers or infomation of watchpoints", cmd_info},
    {"x", "Scan memory from position EXPR to succeding N Bytes in hex form", cmd_x},
    {"p", "Calculate value of EXPR", cmd_p},
    {"w", "Set watchpoint to EXPR", cmd_w},
    {"d", "Delete watchpoint of number N", cmd_d},

    /* TODO: Add more commands */

};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args)
{
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL)
  {
    /* no argument given */
    for (i = 0; i < NR_CMD; i++)
    {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else
  {
    for (i = 0; i < NR_CMD; i++)
    {
      if (strcmp(arg, cmd_table[i].name) == 0)
      {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

static int cmd_si(char *args)
{
  if (args == NULL)
  {
    return 1;
  }
  int steps = atoi(args);
  cpu_exec(steps);
  return 0;
}

static int cmd_info(char *args)
{
  if (!strcmp(args, "r"))
  {
    isa_reg_display();
  }
  else if (!strcmp(args, "w"))
  {
  }
  else
  {
    printf("No such thing to show info\n");
  }
  return 0;
}

static int cmd_x(char *args)
{

  if (args == NULL)
  {
    printf("Nothing to scan!\n");
    return 0;
  }
  char *N_ = strtok(args, " ");
  char *expr = args + strlen(N_) + 1;
  int N = atoi(N_);
  paddr_t addr = strtol(expr, NULL, 0);
  printf("%s succeeding %d values are: \n", expr, N);
  for (int i = 0; i < N; i++)
  {

    word_t val = paddr_read(addr + i * 4, 4);
    printf("%08x ", val);
    if ((i + 1) % 10 == 0)
    {
      printf("\n");
    }
  }
  printf("\n");

  return 0;
}

static int cmd_p(char *args)
{
  if (args == NULL)
  {
    printf("Nothing to scan!\n");
    return 0;
  }
  bool *success = malloc(sizeof(bool));
  *success = true;
  word_t val = expr(args, success);
  Assert(*success==true, "Illegal expr!");
  printf("value is: %d\n", val);
  return 0;
}

static int cmd_w(char *args)
{
  if (args == NULL)
  {
    printf("Nothing to watch!\n");
    return 0;
  }

  bool *success = malloc(sizeof(bool));
  *success = true;
  word_t val = expr(args, success);
  Assert(*success==true, "cmd_w, Illegal expr!");
  WP* w =  new_wp();
  w->expr = malloc(32);
  // printf("value is: %d\n", val);
  printf("watchpoint %d at %08x\n", w->NO, val);
  sprintf(w->expr, "0x%08x%c",val, '\0');
  return 0;
}

static int cmd_d(char *args)
{
  if (args == NULL)
  {
    printf("Nothing to delete!\n");
    return 0;
  }
  return 0;
}
void sdb_set_batch_mode()
{
  is_batch_mode = true;
}

void sdb_mainloop()
{
  if (is_batch_mode)
  {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL;)
  {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL)
    {
      continue;
    }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end)
    {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i++)
    {
      if (strcmp(cmd, cmd_table[i].name) == 0)
      {
        if (cmd_table[i].handler(args) < 0)
        {
          return;
        }
        break;
      }
    }

    if (i == NR_CMD)
    {
      printf("Unknown command '%s'\n", cmd);
    }
  }
}

void init_sdb()
{
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
