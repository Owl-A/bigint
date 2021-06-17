#include <bits/stdc++.h>
#include "bigint.h"
// utils

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

bigint::bigint(){
  head = NULL;
}

lst* eqhelper(lst* b){
  if(b == NULL) return NULL;
  lst* t = (lst*)malloc(sizeof(lst));
  t->val = b->val;
  t->next = eqhelper(b->next);
  return t;
}

// Constructor-Destructor

bigint::bigint (const bigint& b){
  this->head = eqhelper(b.head);
}

bigint::bigint(unsigned long long a){
  head = NULL;
  while(a != 0){
    uint32 tmp = a & MAX_LST;
    lst* t = (lst*) malloc(sizeof(lst));
    t->val = tmp;
    t->next = head;
    head = t;
    a >>= MAX_BIT;    
  }
  reverse(&head);
}

bigint::~bigint(){
  lst* p = this->head;
  this->head = NULL;
  while(p){
    lst* nxt = p->next;
    free(p);
    p = nxt;
  }  
}

// Equals

const bigint& bigint::operator= (const bigint& b){
  lst* p = this->head;
  while(p){
    lst* nxt = p->next;
    free(p);
    p = nxt;
  }  
  this->head = eqhelper( b.head);   
  return b;
}

const bigint& bigint::operator= (unsigned long long b){
  lst* p = this->head;
  while(p){
    lst* nxt = p->next;
    free(p);
    p = nxt;
  }  
  bigint tmp(b);
  this->head = eqhelper(tmp.head);
  return *this;
}

// Add

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

bigint bigint::operator+ (const bigint& b){
  bigint res;
  addhelper(&res.head, this->head, b.head, 0);
  reverse(&res.head);
  return res;
}

bigint bigint::operator+ (unsigned long long  b){
  bigint tmp(b);
  return tmp + *this;
}

bigint operator+ (unsigned long long a, bigint & b){
  return b + a;
}
  
// relational operators

uint32 count (lst* a){
  if(a) return 1 + count(a->next);
  else return 0;
}

bool bigint::operator< (const bigint& b1){
  uint32 al = count(this->head), bl = count(b1.head);
  if(bl > al) return true;
  if(al > bl) return false;
  reverse(&this->head);
  bigint b = b1;
  reverse(&b.head);
  lst* ap = this->head, *bp = b.head;
  while(ap){
    if(ap->val > bp->val){
      reverse(&this->head);
      return false;
    }else if(ap->val == bp->val){ 
      ap = ap->next;
      bp = bp->next;
    }else{
      reverse(&this->head);
      return true;
    }
  }
  reverse(&this->head);
  return false;
}

bool bigint::operator< (unsigned long long b){
  bigint tmp(b);
  return *this < tmp;
}

bool operator< (unsigned long long a, bigint & b){
  bigint tmp(a);
  return tmp < b;
}

bool bigint::operator== (const bigint& b){
  uint32 al = count(this->head), bl = count(b.head);
  if(bl > al) return false;
  if(al > bl) return false;
  lst* ap = this->head, *bp = b.head;
  while(ap){
    if(ap->val == bp->val){ 
      ap = ap->next;
      bp = bp->next;
    }else{
      return false;
    }
  }
  return true;
}

bool bigint::operator== (unsigned long long b){
  bigint tmp(b);
  return *this == tmp;
}

bool operator== (unsigned long long a, bigint & b){
  bigint tmp(a);
  return tmp == b;
}


// subtract 

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

bigint bigint::operator- (const bigint& b){ 
  bigint tmp(0);
  if(*this < b || *this == b) return tmp; 
  tmp = *this;
  subhelper(tmp.head, b.head);
  reverse(&tmp.head);
  lst* p = tmp.head;
  while(p && p->val == 0){
    lst* nxt = p->next;
    free(p);
    p = nxt;
  }
  tmp.head = p;
  reverse(&tmp.head);
  return tmp;
}

bigint bigint::operator- (unsigned long long b){
  bigint tmp(b);
  return *this - tmp;
}

bigint operator- (unsigned long long a, bigint & b){
  bigint tmp(a);
  return tmp - b;
}

// print 

const char* toHex = "0123456789abcdef";

std::ostream& operator<< (std::ostream& os, const bigint& b){
  bigint tmp = b;
  reverse(&tmp.head);
  lst* p = tmp.head;
  os << "0x";
  if(!p) {
    os << 0;
    return os;
  }
  bool state = false;
  for(int i = MAX_BIT-4; i >= 0; i -= 4){
    char tmp = toHex[(p->val >> i) & 0xf];
    if(tmp != '0') state = true;
    if(state){
      os << tmp;
    }
  }
  p = p->next;
  while(p){
    for(int i = MAX_BIT - 4 ; i >= 0; i -= 4){
      os << toHex[(p->val >> i) & 0xf];
    }
    p = p->next;
  }
  return os;
}

// multiply
// Currently uses gradeschool multiplcation
// so very slow

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

bigint bigint::operator* (const bigint& b){
  bigint tmp, res;
  lst* p = head;
  uint32 offset = 0;
  while(p){
    tmp = b;  
    shiftdigit(offset, &tmp.head);
    multone(p->val, &tmp.head);
    res = res + tmp;
    offset++; 
    p = p->next;
  }
  return res;
}

bigint bigint::operator* (unsigned long long b){
  bigint tmp(b);
  return tmp * (*this);
}

bigint operator* (unsigned long long a, bigint & b){
  bigint tmp(a);
  return tmp * b;
}
