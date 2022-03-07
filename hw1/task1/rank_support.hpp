#ifndef RANK_SUPPORT_H
#define RANK_SUPPORT_H

#include <string>
#include <cstdio>
#include <cmath>
#include <bit>
#include <compact_vector.hpp>

#define bitvector compact::vector<unsigned long,1>
#define comvector compact::vector<unsigned long>

class rank_support {
  uint64_t n;
  uint64_t s;
  uint64_t b;
  const bitvector& bv;
  comvector rs;
  comvector rb;
  
 public:
  rank_support(const bitvector& b_in)
      : n(b_in.size()), bv(b_in), s(pow(ceil(log2(n)), 2)),
        rs(comvector(ceil(log2(n)), ceil(n / s))),
        b(ceil(log2(n))),
        rb(comvector(ceil(log2(s)), ceil(n / b))) {
    auto scan = std::vector<unsigned long>(n);
    for (uint64_t i = 0; i < n; i++) {
      scan[i] = bv[i] + ((i == 0) ? 0 : scan[i-1]);
      if (i % s == 0) rs[i/s] = ((i == 0) ? 0 : scan[i-1]);
      if (i % b == 0) rb[i/b] = ((i == 0) ? 0 : scan[i-1] - rs[i/s]);
    }
  }

  uint64_t rank1(uint64_t ind) {
    if (ind >= n) printf("ERROR: invalid index to call rank on\n");
    uint64_t my_s = floor(ind / s);
    uint64_t my_b = floor(ind / b);
    uint64_t block_start = my_b*b;
    uint64_t block_content = 0;
    for (uint64_t i = 0; i <= ind - block_start; i++) {
      block_content |= bv[block_start+i] << i;
    }
    return rs[my_s] + rb[my_b] + std::popcount(block_content);
  }

  uint64_t operator()(uint64_t ind) {
    return this->rank1(ind);
  }

  uint64_t overhead() {
    return 8*(sizeof(rank_support)) + ceil(log2(n))*ceil(n / s) + ceil(log2(s))*ceil(n / b);
  }

  void save(std::string& fname) {
    std::ofstream outfile(fname);
    outfile << n << s << b;
    rs.serialize(outfile);
    rb.serialize(outfile);
    outfile.close();
  }
  
  void load(std::string& fname) {
    std::ifstream infile(fname);
    infile >> n >> s >> b;
    rs = comvector(ceil(log2(n)), 1);
    rs.deserialize(infile);
    rb = comvector(ceil(log2(s)), 1);
    rb.deserialize(infile);
    infile.close();
  }
  
  void print_members() {
    for (uint64_t i = 0; i < n; i++) {
      printf("%lu ", bv[i]);
    }
    printf("\n");
    for (uint64_t i = 0; i < n; i++) {
      if (i % s == 0) printf("%lu ", (unsigned long) rs[i/s]);
      else printf("  ");
    }
    printf("\n");
    for (uint64_t i = 0; i < n; i++) {
      if (i % b == 0) printf("%lu ", (unsigned long) rb[i/b]);
      else printf("  ");
    }
    printf("\n\n");
    /*
    for (uint64_t i = 0; i < pow(2, b); i++) {
      for (uint64_t j = 0; j < b; j++) {
        printf("%lu ", (unsigned long) rp[(i*b)+j]);
      }
      printf("\n");
    }
    */
  }
};

#endif
