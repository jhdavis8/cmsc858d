#include "select_support.hpp"

#define bitvector compact::vector<unsigned long,1>
#define comvector compact::vector<unsigned long>

class sparse_array {
 public:
  bitvector bv;
  rank_support* ranksup;
  std::vector<std::string> vals;
  uint64_t sz;
  uint64_t nval;
  
  sparse_array(uint64_t s) {
    create(s);
  }

  ~sparse_array() {
    delete ranksup;
  }
  
  void create(uint64_t s) {
    sz = s;
    bv.resize(sz);
    ranksup = new rank_support(bv);
    nval = 0;
    vals.resize(0);
  }
  
  void append(std::string elem, uint64_t pos) {
    if (pos >= sz) {
      printf("ERROR: cannot insert beyond size.\n");
      return;
    } else if (bv[pos] == 1) {
      printf("ERROR: cannot insert to location with exisitng element.\n");
      return;
    }
    bv[pos] = 1;
    //(*ranksup).insert(pos);
    ranksup = new rank_support(bv);
    //printf("Begin of superblock in rs is %lu\n", (uint64_t) (*ranksup).rs[0]);
    vals.push_back(elem);
    nval++;
  }
  
  bool get_at_rank(uint64_t r, std::string& elem) {
    if (nval < r) {
      return false;
    } else {
      elem = vals.at(r);
      return true;
    }
  }
  
  bool get_at_index(uint64_t r, std::string& elem) {
    if (bv[r] == 0) {
      return false;
    } else {
      elem = vals.at((*ranksup)(r) - 1);
      return true;
    }
  }
  
  uint64_t num_elem_at(uint64_t r) {
    return (*ranksup)(r);
  }
  
  uint64_t size() {
    return sz;
  }
  
  uint64_t num_elem() {
    return nval;
  }
  
  void save(const std::string& fname) {
    (*ranksup).save(fname);
    std::ofstream outfile(fname, std::ios_base::app);
    outfile << sz << " " << nval;
    for (int i = 0; i < nval; i++) {
      outfile << vals[i] << std::endl;
    }
    outfile.close();
  }
  
  void load(const std::string& fname) {
    std::ifstream infile(fname);
    (*ranksup).load(infile);
    bv = (*ranksup).bv;
    infile >> sz >> nval;
    std::string line;
    while (getline(infile, line)) {
      vals.push_back(line);
    }
    infile.close();
  }
};
