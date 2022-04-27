// C++ standard lib includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>

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

int lcp_len(std::string a, int a_pos, std::string b, int curr_len) {
  int search_len = std::min(a.length() - (a_pos - 1), b.length());
  int result = curr_len;
  for (int i = curr_len; i < search_len; i++) {
    if (a[i + a_pos] == b[i]) result++;
    else break;
  }
  return result;
}

int opt_compare(std::string a, std::string b, int b_pos, int* mlh, int mode) {
  if (mode == 0) *mlh = 0;
  int a_len = a.length();
  int b_len = b.length() - (b_pos - 1);
  for (int i = *mlh; i < std::min(a_len, b_len); i++) {
    if (a[i] > b[i + b_pos]) {
      return 1;
    } else if (a[i] < b[i + b_pos]) {
      return -1;
    } else {
      if (mode == 1) (*mlh)++;
    }
  }
  return (a_len > b_len) - (a_len < b_len);
  //if (mlh == 0) return a.compare(b);
  //else return a.substr(mlh + 1, a.length() - (mlh + 1)).compare(b.substr(mlh + 1,
  //                                                                       b.length() - (mlh + 1)));
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
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  std::vector<std::string> results;
  for (int i = 0; i < queries.size(); i++) {
    std::string query = queries[i];
    int l, h;
    int loc = -1;
    if (pref_len > 0 && preftab.contains(query.substr(0, pref_len))) {
      std::array<int, 2> pair = preftab.at(query.substr(0, pref_len));
      l = pair[0];
      h = pair[1] - 1;
      /*
      if (i == 203) {
        std::cout << suffix(full_ref, sa[l - 1]).substr(0, query.length()) << std::endl;
        std::cout << suffix(full_ref, sa[l    ]).substr(0, query.length()) << std::endl;
        std::cout << suffix(full_ref, sa[h    ]).substr(0, query.length()) << std::endl;
        std::cout << suffix(full_ref, sa[h + 1]).substr(0, query.length()) << std::endl;
      }
      */
    } else {
      l = 0;
      h = sa.size() - 1;
    }
    if (pref_len > 0 && preftab.contains(query.substr(0, pref_len))) loc = 0; // skip search
    //std::cout << i << " " << l << " " << h << " " << query << std::endl;
    //std::cout << query.substr(0, pref_len) << std::endl;
    int c = floor((l + h)/2);
    int lcp_l = (mode == 1) ? lcp_len(full_ref, sa[l], query, lcp_l) : 0;
    int lcp_h = (mode == 1) ? lcp_len(full_ref, sa[h], query, lcp_h) : 0;
    int mlh = (mode == 1) ? std::min(lcp_l, lcp_h) : 0;
    int iters = 0;
    while (loc < 0) {
      //std::cout << query << " " << c << " " << l << " " << h << std::endl;
      int cmp;
      cmp = opt_compare(query, full_ref, sa[c], &mlh, mode);
      if (h - l < 2) {
        //std::cout << "Direct search" << std::endl;
        for (int j = l; j <= h; j++) {
          if (is_prefix(query, suffix(full_ref, sa[j]))) {
            loc = j;
            //std::cout << "Hit" << std::endl;
            break;
          }
        }
        break;
      } else {
        if (cmp > 0) {
          if (c == h - 1) {
            loc = h;
          }
          l = c;
          if (mode == 1) lcp_l = mlh;
        } else if (cmp < 0) {
          if (c == l + 1) {
            loc = l;
          }
          h = c;
          if (mode == 1) lcp_h = mlh;
        }
       }
      c = floor((l + h)/2);
      if (mode == 1) mlh = std::min(lcp_l, lcp_h);
      iters++;
    }
    //std::cout << iters << std::endl;
    int matches = 0;
    if (loc >= 0 && !is_prefix(query, suffix(full_ref, sa[loc]))) {
      loc++;
    }
    while (loc >= 0 && is_prefix(query, suffix(full_ref, sa[loc + matches]))) {
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
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::cout << "Query time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

  // Write results to file
  std::ofstream os(out_file);
  std::sort(results.begin(), results.end());
  for (std::string s : results) {
    os << s << std::endl;
  }
  os.close();

  return 0;
}
