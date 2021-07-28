#ifndef UTILS_H
#define UTILS_H
#include "bigint.h"

void reverse(lst** head){
  if (*head == NULL) return; 
  lst *p, *c;
  p = NULL;
  c = *head;
  while(c != NULL){
    lst* tmp = c->next;
    c->next = p;
    p = c;
    c = tmp;
  }
  *head = p;
}

lst* eqhelper(lst* b){
  if(b == NULL) return NULL;
  lst* t = (lst*)malloc(sizeof(lst));
  t->val = b->val;
  t->next = eqhelper(b->next);
  return t;
}

void addhelper(lst** res, lst* a, lst* b, uint32 carry){
  uint32 sum = 0;
  if(a || b || carry){
    sum = (a ? a->val : 0) + ( b ? b->val : 0) + carry;
    carry = sum >> MAX_BIT;
    sum = sum & MAX_LST;
    
    lst* p = (lst*) malloc(sizeof(lst));
    p->val = sum;
    p->next = *res;
    *res = p; 
    addhelper(res, (a ? a->next : NULL), (b ? b->next : NULL), carry);
  }
}

uint32 count (lst* a){
  if(a) return 1 + count(a->next);
  else return 0;
}

void borrow(lst* a){
  while(a && a->val == 0){
    a->val = MAX_LST;
    a = a->next;
  }
  a->val -= 1;
}

void subhelper( lst* a, lst* b){
  if(!b) return;
  if((a)->val < b->val){
    a->val |= (unsigned long long)1 << MAX_BIT;
    a->val -= b->val;
    borrow(a->next);
  }else{
    a->val -= b->val;
  }  
  subhelper(a->next, b->next);
}

void multone(uint32 a, lst** b){
  // a < (1 << MAX_BIT)
  if(a == 0){
    lst* p = *b;
    while(p){
      lst* nxt = p->next;
      free(p);
      p = nxt;
    }
    *b = NULL;
    return;
  }
  lst* p = *b;
  while(p){
    p->val *= a;
    p = p->next;
  }
  uint32 carry = 0;
  p = *b;
  lst* d = NULL;
  while(p){
    p->val += carry;
    carry = p->val >> MAX_BIT;
    p->val &= MAX_LST; 
    d = p;
    p = p->next;
  }
  if(carry){
    lst* z = (lst*) malloc(sizeof(lst));
    z->val = carry;
    z->next = NULL;
    d->next = z;
  }
}

void shiftdigit(uint32 count, lst** b){
  while(count--){
    lst* p = (lst*) malloc(sizeof(lst));
    p->val = 0;
    p->next = *b;
    *b = p;
  }
}

#endif
