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

uint64_t scan(const bitvector& b, uint64_t ind) {
  uint64_t scan = 0;
  for (uint64_t i = 0; i <= ind; i++) scan += b[i];
  return scan;
}

int main() {
  std::string n("NAME");
  printf("%-35sRESULT    DETAILS\n", n.c_str());
  int passes = 0, tests = 0;

  bitvector b;
  b.assign({0,1,1,1,0,1,0,1,1,1,0,0,1,1,0,1,0,1,1,1,0,1,1,1,0,0,0,0,1,1,1,0,1,0,0,1,1,0,1,0,1,0,1,1,0,0,1,1,1,0,0,0,1,0,1,1,0,0,0,1,0,1,0});
  rank_support r(b);
  test_eq(r(0), scan(b,0), "0", tests, passes);
  test_eq(r(1), scan(b,1), "1", tests, passes);
  test_eq(r(4), scan(b,4), "4", tests, passes);
  test_eq(r(8), scan(b,8), "8", tests, passes);
  test_eq(r(19), scan(b,19), "19", tests, passes);
  test_eq(r(35), scan(b,35), "35", tests, passes);
  test_eq(r(36), scan(b,36), "36", tests, passes);
  test_eq(r(37), scan(b,37), "37", tests, passes);
  test_eq(r(43), scan(b,43), "43", tests, passes);
  test_eq(r(50), scan(b,50), "50", tests, passes);
  test_eq(r(52), scan(b,52), "52", tests, passes);
  test_eq(r(60), scan(b,60), "60", tests, passes);
  test_eq(r(61), scan(b,61), "61", tests, passes);
  test_eq(r(62), scan(b,62), "62", tests, passes);

  printf("Test complete. %f%% pass rate, %d pass, %d fail, %d total.\n",
         (double) (passes/tests)*100, passes, tests - passes, tests);

  r.print_members();
  
  return 0;
}
