// C++ standard lib includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

// External lib includes
#include "FastaReader.hpp"
#include "sdsl/qsufsort.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/unordered_map.hpp"
#include "cereal/types/array.hpp"

bool is_prefix(std::string prefix, std::string ref) {
  return std::mismatch(prefix.begin(), prefix.end(), ref.begin(), ref.end()).first == prefix.end();
}

std::string suffix(std::string full, int start) {
  return full.substr(start, full.length() - start);
}

int main(int argc, char* argv[]) {
  // Process command line arguments
  if (argc < 5) {
    std::cerr << "Usage: " << argv[0] << " index query mode output" << std::endl;
    return 1;
  }
  std::string index_file = argv[1];
  std::string query_file = argv[2];
  std::string mode = argv[3];
  std::string out_file = argv[4];
  if (mode != "naive" && mode != "simpaccel") {
    std::cerr << "Invalid query mode: " << mode << std::endl;
    return 1;
  }

  // Load data from suffix array file
  std::string full_ref;
  sdsl::int_vector<64> sa;
  int pref_len;
  std::unordered_map<std::string, std::array<int, 2>> preftab;
  
  std::ifstream is(index_file, std::ios::binary);
  cereal::BinaryInputArchive archive_in(is);
  archive_in(full_ref);
  sa.load(is);
  archive_in(pref_len);
  if (pref_len > 0) {
    archive_in(preftab);
  }

  // Read queries and search
  FastaReader fr(query_file);
  while (fr.hasNextSequence()) {
    fr.readSequence();
    std::string query = fr.Sequence();
    int l = 0;
    int h = sa.size();
    int c = floor((l + h)/2);
    int loc = -1;
    while (loc < 0) {
      int cmp = query.compare(suffix(full_ref, sa[c]));
      if (cmp > 0) {
        if (c == h - 1) {
          loc = h;
        }
        l = c;
      } else if (cmp < 0) {
        if (c == l + 1) {
          loc = c;
        }
        h = c;
      }
      c = floor((l + h)/2);
    }
    int matches = 0;
    while (is_prefix(query, suffix(full_ref, sa[loc + matches]))) {
      matches++;
    }
    //if (matches > 1) std::cout << fr.Id() << " " << fr.Description() << " Loc: " << loc << " Matches: " << matches << std::endl;
    
  }
  return 0;
}
