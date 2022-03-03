#ifndef RANK_SUPPORT_H
#define RANK_SUPPORT_H

#include <string>
#include <cstdio>
#include <cmath>
#include <include/compact_vector.hpp>

#define bitvector compact::vector<unsigned,1>

class rank_support {
  std::size_t n;
  std::size_t s;
  std::size_t b;
  bitvector b;
  bitvector rs;
  bitvector rb;
  bitvector rp;
  
 public:
  rank_support(bitvector& b_in) {
    n = b_in.size();
    b = b_in;
    s = ceil(pow(log2(n),2)/2);
    rs = new bitvector(ceil(n/s));
    b = ceil(log2(n)/2);
    rb = new bitvector(ceil(n/b));
    rp = new compact::vector<unsigned,ceil(log2(b))>(pow(2,b)*b);
  }
  
  uint64_t rank1(uint64_t i);
  uint64_t operator()(uint64_t i);
  uint64_t overhead();
  void save(std::string& fname);
  void load(std::string& fname);
}

#endif
