#ifndef __SDB_H__
#define __SDB_H__

#include <common.h>

void init_regex();
void init_wp_pool();

word_t paddr_read(paddr_t addr, int len);
void paddr_write(paddr_t addr, int len, word_t data);

word_t expr(char *e, bool *success);
typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  /* TODO: Add more members if necessary */
  char *expr;
  word_t addr;
  word_t expr_val;
} WP;

WP *new_wp();
void free_wp(WP* wp);
bool scan_wp();

#endif
