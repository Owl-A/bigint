#include <bits/stdc++.h>
#include "bigint.h"
#include "utils.h"

bigint::bigint(){
  head = NULL;
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
