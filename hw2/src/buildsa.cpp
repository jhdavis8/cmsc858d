#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "FastaReader.hpp"
#include "sdsl/qsufsort.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/unordered_map.hpp"
#include "cereal/types/array.hpp"

int main(int argc, char* argv[]) {
  // Process command line arguments
  if (argc < 3 || argc > 5) {
    std::cerr << "Usage: " << argv[0] << " [--preftab k] reference output" << std::endl;
    return 1;
  }
  std::string ref;
  std::string outp;
  int pref_len = -1;
  int aoff = 0;
  
  if (std::string(argv[1]) == "--preftab") {
    std::istringstream ss(argv[2]);
    if (!(ss >> pref_len) || pref_len < 0) {
      std::cerr << "Invalid number: " << argv[2] << std::endl;
      return 1;
    }
    aoff = 2;
  }
  ref = argv[1 + aoff];
  outp = argv[2 + aoff];

  // Read in reference FASTA file using FastaReader
  std::string full_ref;
  FastaReader fr(ref);
  while (fr.hasNextSequence()) {
    fr.readSequence();
    std::string seq = fr.Sequence();
    full_ref.append(seq);
  }

  // Construct suffix array using SDSL qsufsort::construct_sa
  sdsl::int_vector<64> sa;
  std::vector<char> c_ref(full_ref.begin(), full_ref.end());
  c_ref.push_back('\0');
  sdsl::qsufsort::construct_sa(sa, c_ref);

  // Construct prefix lookup table if needed
  std::unordered_map<std::string, std::array<int, 2>> preftab;
  if (pref_len > 0) {
    std::string curr_pref = full_ref.substr(sa[0], pref_len);
    int curr_itv_start = 0;
    //std::cout << "Full text length: " << full_ref.length() << std::endl;
    //std::cout << "First suffix pos: " << sa[0] << std::endl;
    for (int i = 0; i < sa.size(); i++) {
      std::string chk = full_ref.substr(sa[i], pref_len);
      if (chk != curr_pref || i == sa.size() - 1) {
        //std::cout << curr_pref << " " << curr_itv_start << " " << i << " " << chk << ":" << sa[i] << std::endl;
        preftab.insert({curr_pref, {curr_itv_start, i}});
        curr_pref = chk;
        curr_itv_start = i;
      }
    }
  }

  /*
  std::cout << "Prefix table:" << std::endl;
  for (const auto& [key, value] : preftab) {
    std::cout << key << " " << value[0] << " " << value[1] << std::endl;
  }
  */

  // Write suffix array to disk using cereal and sdsl built-in serialization
  std::ofstream os(outp, std::ios::binary);
  cereal::BinaryOutputArchive archive(os);
  archive(full_ref);
  sa.serialize(os);
  archive(pref_len);
  if (pref_len > 0) {
    archive(preftab);
  }
  
  return 0;
}
