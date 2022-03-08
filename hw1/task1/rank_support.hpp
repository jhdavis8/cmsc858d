#ifndef RANK_SUPPORT_H
#define RANK_SUPPORT_H

#include <string>
#include <cstdio>
#include <cmath>         // for pow, ceil, log2
#include <bit>           // for popcount
#include <algorithm>     // for max
#include <compact_vector.hpp>

#define bitvector compact::vector<unsigned long,1>
#define comvector compact::vector<unsigned long>

class rank_support {
 public:  
  uint64_t n;
  uint64_t s;
  uint64_t b;
  bitvector bv;
  comvector rs;
  comvector rb;
  
  rank_support(const bitvector& b_in)
      : n(b_in.size()), bv(b_in),
        s(std::max(1.0,pow(ceil(log2(n)), 2))),
        rs(comvector(ceil(log2(n)), ceil(double(n) / double(s)))),
        b(std::max(1.0,ceil(log2(n)))),
        rb(comvector(ceil(log2(s)), ceil(double(n) / double(b)))) {
    //printf("Rank support construct:\n");
    //printf("n = %lu\n", n);
    //printf("s = %lu\n", s);
    //printf("b = %lu\n", b);
    auto scan = std::vector<unsigned long>(n);
    for (uint64_t i = 0; i < n; i++) {
      scan[i] = bv[i] + ((i == 0) ? 0 : scan[i-1]);
      if (i % s == 0) rs[i/s] = ((i == 0) ? 0 : scan[i-1]);
      if (i % b == 0) rb[i/b] = ((i == 0) ? 0 : scan[i-1] - rs[i/s]);
    }
  }

  void insert(uint64_t ind) {
    bv[ind] = 1;
    //printf("Loop from %lu to %lu in rs\n", (ind / s) + 1, (uint64_t) ceil(double(n) / double(s)));
    for (uint64_t i = (ind / s) + 1; i < ceil(double(n) / double(s)); i++) {
      rs[i] = rs[i] + 1;
    }
    //printf("Loop from %lu to %lu in rb\n", (ind / b) + 1, (ind / b) + 1 + (s / b));
    for (uint64_t i = (ind / b) + 1; i < (ind / b) + 1 + (s / b); i++) {
      if (i / (s / b) == ind / s && i < ceil(double(n) / double(b))) rb[i] = rb[i] + 1;
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
    return 8*(sizeof(rank_support)) + ceil(log2(n))*ceil(n / s) + ceil(log2(s))*ceil(n / b) + n;
  }

  void save(const std::string& fname) {
    std::ofstream outfile(fname);
    outfile << " " << n << " " << s << " " << b;
    rs.serialize(outfile);
    rb.serialize(outfile);
    bv.serialize(outfile);
    outfile.close();
  }
  
  void load(const std::string& fname) {
    std::ifstream infile(fname);
    load(infile);
    infile.close();
  }

  void load(std::ifstream& infile) {
    infile >> n >> s >> b;
    rs.deserialize(infile);
    rb.deserialize(infile);
    bv.deserialize(infile);
  }
  
  void print_members() {
    for (uint64_t i = 0; i < n; i++) {
      printf("%lu ", (unsigned long) bv[i]);
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
  }
};

#endif
