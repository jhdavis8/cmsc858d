#include "rank_support.h"

void test_eq(uint64_t a, uint64_t e, const std::string& name, int& tests, int& passes) {
  assert(name.size() < 35);
  if (a != e) {
    printf("%-35sFAIL      expected %lu, got %lu\n", name.c_str(), e, a);
    tests++;
  } else {
    printf("%-35sPASS      got %lu\n", name.c_str(), a);
    passes++;
    tests++;
  }
}

int main() {
  std::string n("NAME");
  printf("%-35sRESULT    DETAILS\n", n.c_str());
  int passes = 0, tests = 0;

  

  printf("Test complete. %f%% pass rate, %d pass, %d fail, %d total.\n",
         (double) passes/tests, passes, tests - passes, tests);
  return 0;
}
