#include "rank_support.hpp"

class select_support {
 public:
  select_support(const rank_support& r_in) {
  }
  
  uint64_t select1(uint64_t ind) {}
  
  uint64_t operator()(uint64_t ind) {
    return this->select1(ind);
  }
  
  uint64_t overhead() {}
  void save(const std::string& fname) {}
  void load(const std::string& fname) {}
};
