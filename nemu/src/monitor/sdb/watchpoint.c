#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  /* TODO: Add more members if necessary */
  char *expr;

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

WP* new_wp(const char *expr);
void free_wp(WP *wp);

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}


/* TODO: Implement the functionality of watchpoint */

WP* new_wp(const char *expr){
  WP* tail = head;
  while(tail->next !=NULL){
    tail = tail->next;
  }
  tail->next = free_;
  free_ = free_ +1;
  return tail->next;
}

void free_wp(WP* wp){
  
  WP* port = head;
  while(port->next!=NULL && port->next!=wp){
    port = port->next;
  }
  Assert(port->next!=NULL, "port not null");
  port->next = port->next->next;

  WP* tail_used=free_;
  while(tail_used->next!=NULL){
    tail_used = tail_used->next;
  }
  tail_used->next = &wp_pool[wp->NO];

  free(wp);
}


