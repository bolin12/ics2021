#include "sdb.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

WP* new_wp();
void free_wp(WP *wp);

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = malloc(sizeof(WP));
  free_ = wp_pool;
}


/* TODO: Implement the functionality of watchpoint */

WP* new_wp(){
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

bool scan_wp(){
  WP* head_ = head;
  while(head_->next!=NULL){
    
    WP* cur_wp =  head_->next;
    word_t new_expr_val = paddr_read(cur_wp->addr, 4);
    if(cur_wp->expr_val != new_expr_val){

      printf("watchpoint %d val change to: %08x\n", cur_wp->NO, new_expr_val);
      return true;
    }
    head_ = head_->next;
  }
  return false;
}
