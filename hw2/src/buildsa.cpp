#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "FastaReader.hpp"
#include "sdsl/qsufsort.hpp"
#include "cereal/archives/binary.hpp"
#include <cereal/types/string.hpp>

int main(int argc, char* argv[]) {
  // Process command line arguments
  if (argc < 3 || argc > 5) {
    std::cerr << "Usage: " << argv[0] << " [--preftab k] reference output" << std::endl;
    return 1;
  }
  std::string ref;
  std::string outp;
  int preftab = -1;
  int aoff = 0;
  
  if (std::string(argv[1]) == "--preftab") {
    std::istringstream ss(argv[2]);
    if (!(ss >> preftab) || preftab < 0) {
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

  // Write suffix array to disk using cereal and sdsl built-in serialization
  std::ofstream os(outp, std::ios::binary);
  cereal::BinaryOutputArchive archive(os);
  archive(full_ref);
  sa.serialize(os);
  
  return 0;
}
