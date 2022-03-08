#include "rank_support.hpp"

class select_support {
  rank_support r;
  
 public:
  select_support(const rank_support& r_in) : r(r_in) {}
  
  uint64_t select1(uint64_t ind) {
    bool found = false;
    uint64_t mid = r.n / 2;
    uint64_t lo = 0;
    uint64_t hi = r.n;
    if (ind == 0) return 0;
    while (!found) {
      uint64_t rank = r(mid);
      if (rank == ind) {
        while (r.bv[mid] != 1) mid--;
        found = true;
      } else {
        if (lo == hi) {
          printf("ERROR: select query higher than number of 1's in bit vector\n");
          return 0;
        } else if (rank > ind) {
          hi = mid - 1;
          mid = (hi + lo) / 2;
        } else if (rank < ind) {
          lo = mid + 1;
          mid = (hi + lo) / 2;
        }
      }
    }
    return mid;
  }
  
  uint64_t operator()(uint64_t ind) {
    return this->select1(ind);
  }
  
  uint64_t overhead() {
    return 8*(sizeof(select_support)) + r.overhead() - 8*(sizeof(rank_support));
  }
  
  void save(const std::string& fname) {}
  void load(const std::string& fname) {}
};
