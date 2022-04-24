#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
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

  return 0;
}
