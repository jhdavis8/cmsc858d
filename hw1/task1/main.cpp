#include <cassert>
#include "rank_support.hpp"

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

  bitvector b;
  b.assign({0,1,1,1,0,1,0,1,1,1,0,0,1,1,0,1,0,1,1,1,0,1,1,1,0,0,0,0,1,1,1,0,1,0,0,1,1,0,1,0,1,0,1,1,0,0,1,1,1,0,0,0,1,0,1,1,0,0,0,1,0,1,0});
  rank_support r(b);
  test_eq(r(0), 0, "0", tests, passes);
  test_eq(r(1), 1, "1", tests, passes);
  test_eq(r(4), 3, "4", tests, passes);
  test_eq(r(8), 6, "8", tests, passes);
  test_eq(r(19), 13, "19", tests, passes);
  test_eq(r(35), 21, "35", tests, passes);
  test_eq(r(36), 22, "36", tests, passes);
  test_eq(r(37), 22, "37", tests, passes);
  test_eq(r(43), 26, "43", tests, passes);

  printf("Test complete. %f%% pass rate, %d pass, %d fail, %d total.\n",
         (double) (passes/tests)*100, passes, tests - passes, tests);

  r.print_members();
  
  return 0;
}
