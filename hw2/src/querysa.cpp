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

int lcp_len(std::string a, std::string b) {
  int search_len = std::min(a.length(), b.length());
  int result = 0;
  for (int i = 0; i < search_len; i++) {
    if (a[i] == b[i]) result++;
    else return result;
  }
  return result;
}

int opt_compare(std::string a, std::string b, int mlh) {
  if (mlh == 0) return a.compare(b);
  else return a.substr(mlh + 1, a.length() - (mlh + 1)).compare(b.substr(mlh + 1,
                                                                         b.length() - (mlh + 1)));
  /*
  else {
    int search_len = std::min(a.length(), b.length());
    for (int i = mlh; i < search_len; i++) {
      if (a[i] > b[i]) return 1;
      else if (a[i] < b[i]) return -1;
    }
    return (a.length() > b.length()) - (a.length() < b.length());
  }
  */
}

int main(int argc, char* argv[]) {
  // Process command line arguments
  if (argc < 5) {
    std::cerr << "Usage: " << argv[0] << " index query mode output" << std::endl;
    return 1;
  }
  std::string index_file = argv[1];
  std::string query_file = argv[2];
  std::string mode_s = argv[3];
  int mode = (mode_s == "simpaccel") ? 1 : 0;
  std::string out_file = argv[4];
  if (mode_s != "naive" && mode_s != "simpaccel") {
    std::cerr << "Invalid query mode: " << mode_s << std::endl;
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

  // Read queries from file
  std::vector<std::string> queries;
  std::vector<std::string> descriptions;
  FastaReader fr(query_file);
  while (fr.hasNextSequence()) {
    fr.readSequence();
    queries.push_back(fr.Sequence());
    std::string des = fr.Id();
    if (fr.Description().length() > 0 && fr.Description() != " ") {
      des.append(" ");
      des.append(fr.Description());
    }
    descriptions.push_back(des);
  }

  //std::cout << full_ref.length() << std::endl;
  
  // Search for all queries and record in vector
  std::vector<std::string> results;
  for (int i = 0; i < queries.size(); i++) {
    std::string query = queries[i];
    int l, h;
    if (pref_len > 0) {
      std::array<int, 2> pair = preftab.at(query.substr(0, pref_len));
      l = pair[0];
      h = pair[1] - 1;
    } else {
      l = 0;
      h = sa.size() - 1;
    }
    std::cout << l << " " << h << std::endl;
    int c = floor((l + h)/2);
    int loc = -1;
    int mlh = 0;
    int iters = 0;
    while (loc < 0) {
      //std::cout << query << " " << c << " " << l << " " << h << std::endl;
      int cmp = opt_compare(query, suffix(full_ref, sa[c]), mlh);
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
      if (mode == 1) { // enable simpaccel if requested
        mlh = std::min(lcp_len(suffix(full_ref, sa[l]), query),
                       lcp_len(suffix(full_ref, sa[h]), query));
      }
      iters++;
    }
    std::cout << iters << std::endl;
    int matches = 0;
    while (is_prefix(query, suffix(full_ref, sa[loc + matches]))) {
      matches++;
    }
    std::string to_write = descriptions[i];
    to_write.append("\t");
    to_write.append(std::to_string(matches));
    for (int j = 0; j < matches; j++) {
      to_write.append("\t");
      to_write.append(std::to_string(sa[loc + j]));
    }
    results.push_back(to_write);
  }

  // Write results to file
  std::ofstream os(out_file);
  std::sort(results.begin(), results.end());
  for (std::string s : results) {
    os << s << std::endl;
  }
  os.close();

  return 0;
}
