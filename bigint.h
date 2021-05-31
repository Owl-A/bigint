#ifndef BIGINT_H
#define BIGINT_H

#define uint32 unsigned long long 
#define MAX_LST 4294967295
#define MAX_BIT 32

struct lst{
  lst* next;
  uint32 val;
};

class bigint{
  lst* head;
public:
  bigint();
  ~bigint();
  bigint(unsigned long long a);
  bigint(const bigint& a);

  const bigint& operator= (const bigint& b);
  const bigint& operator= (unsigned long long b);

  bigint operator+ (const bigint& b);
  bigint operator+ (unsigned long long  b);

  bigint operator- (const bigint& b);
  bigint operator- (unsigned long long b);

  bigint operator* (const bigint& b);
  bigint operator* (unsigned long long b);

  bool operator< (const bigint& b);
  bool operator< (unsigned long long b);
  
  bool operator== (const bigint& b);
  bool operator== (unsigned long long b);

  friend bigint operator+ (unsigned long long a, bigint & b);
  friend bigint operator- (unsigned long long a, bigint & b);
  friend bigint operator* (unsigned long long a, bigint & b);
  friend bool operator< (unsigned long long a, bigint & b);
  friend bool operator== (unsigned long long a, bigint & b);
  friend std::ostream& operator<< (std::ostream& os, const bigint& b);

};

bigint operator+ (unsigned long long a, bigint & b);
bigint operator- (unsigned long long a, bigint & b);
bigint operator* (unsigned long long a, bigint & b);
bool operator< (unsigned long long a, bigint & b);
bool operator== (unsigned long long a, bigint & b);
std::ostream& operator<< (std::ostream& os, const bigint& b);
#endif
