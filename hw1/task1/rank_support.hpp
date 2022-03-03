#ifndef RANK_SUPPORT_H
#define RANK_SUPPORT_H

#include <string>
#include <cstdio>
#include <cmath>
#include <compact_vector.hpp>

#define bitvector compact::vector<unsigned,1>
#define comvector compact::vector<unsigned>

class rank_support {
  std::size_t n;
  std::size_t s;
  std::size_t b;
  const bitvector& bv;
  comvector& rs;
  comvector& rb;
  comvector& rp;
  
 public:
  rank_support(const bitvector& b_in)
      : n(b_in.size()), bv(b_in), s(ceil(pow(log2(n), 2) / 2)),
        rs(*(new comvector(ceil(log2(n)), ceil(n / s)))),
        b(ceil(log2(n) / 2)),
        rb(*(new comvector(ceil(log2(s)), ceil(n / b)))),
        rp(*(new comvector(ceil(log2(b)), pow(2, b)*b))) {
    auto scan = std::vector<unsigned long>(n);
    for (int i = 0; i < n; i++) {
      scan[i] = bv[i] + ((i == 0) ? 0 : scan[i-1]);
      if (i % s == 0) rs[i/s] = ((i == 0) ? 0 : scan[i-1]);
      if (i % b == 0) rb[i/b] = ((i == 0) ? 0 : scan[i-1] - rs[i/s]);
    }
    for (int i = 0; i < pow(2, b); i++) {
      for (int j = 0; j < b; j++) {
        rp[(i*b)+j] = (i % int(pow(2, b - j)))/pow(2, b - 1 - j) + (j == 0 ? 0 : rp[(i*b)+(j-1)]);
      }
    }
  }
  /*
  uint64_t rank1(uint64_t i);
  uint64_t operator()(uint64_t i);
  uint64_t overhead();
  void save(std::string& fname);
  void load(std::string& fname);
  */
};

#endif
