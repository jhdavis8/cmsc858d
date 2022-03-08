#include <cassert>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include "select_support.hpp"

void test_eq(uint64_t a, uint64_t e, const std::string& name, int& tests, int& passes) {
  assert(name.size() < 35);
  if (a != e) {
    printf("%-10sFAIL      expected %lu, got %lu\n", name.c_str(), e, a);
    tests++;
  } else {
    printf("%-10sPASS      got %lu\n", name.c_str(), a);
    passes++;
    tests++;
  }
}

void benchmark(uint64_t size, uint64_t ops) {
  bitvector b;
  uint64_t ones = 0;
  for (int i = 0; i < size; i++) {
    uint64_t val = rand() % 2;
    if (val == 1) ones++;
    b.push_back(val);
  }
  rank_support r(b);
  select_support s(r);
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  volatile uint64_t result;
  for (int i = 0; i < ops; i++) {
    result = s(rand() % (ones + 1));
  }
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  double ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  printf("Benchmark N = %10lu, ops = %10lu: Memory overhead = %10lu bits, runtime %10f ms.\n",
         size, ops, r.overhead(), ms);
}

uint64_t nsel(const bitvector& b, uint64_t ind) {
  uint64_t count = 0;
  for (uint64_t i = 0; i <= b.size(); i++) {
    if (b[i] == 1) count++;
    if (count == ind) return i;
  }
  return 0;
}

int main() {
  srand(time(NULL));
  std::string n("NAME");
  printf("%-10sRESULT    DETAILS\n", n.c_str());
  int passes = 0, tests = 0;

  bitvector b;
  b.assign({0,1,1,1,0,1,0,1,1,1,0,0,1,1,0,1,0,1,1,1,0,1,1,1,0,0,0,0,1,1,1,0,1,0,0,1,1,0,1,0,1,0,1,1,0,0,1,1,1,0,0,0,1,0,1,1,0,0,0,1,0,1,0});
  rank_support r(b);
  select_support s(r);
  test_eq(s(0), nsel(b,0), "0", tests, passes);
  test_eq(s(1), nsel(b,1), "1", tests, passes);
  test_eq(s(4), nsel(b,4), "4", tests, passes);
  test_eq(s(8), nsel(b,8), "8", tests, passes);
  test_eq(s(9), nsel(b,9), "9", tests, passes);
  test_eq(s(12), nsel(b,12), "12", tests, passes);
  test_eq(s(19), nsel(b,19), "19", tests, passes);
  test_eq(s(21), nsel(b,21), "21", tests, passes);
  test_eq(s(25), nsel(b,25), "25", tests, passes);
  test_eq(s(26), nsel(b,26), "26", tests, passes);
  test_eq(s(27), nsel(b,27), "27", tests, passes);
  test_eq(s(28), nsel(b,28), "28", tests, passes);
  test_eq(s(29), nsel(b,29), "29", tests, passes);
  test_eq(s(30), nsel(b,30), "30", tests, passes);
  test_eq(s(31), nsel(b,31), "31", tests, passes);
  test_eq(s(34), nsel(b,34), "34", tests, passes);

  s.save("select-save-test.dat");
  printf("Save done.\n");
  bitvector c;
  c.assign({1});
  rank_support r1(c);
  select_support s1(r1);
  s1.load("select-save-test.dat");
  printf("Load done.\n");
  
  test_eq(s(0), nsel(b,0), "0", tests, passes);
  test_eq(s(1), nsel(b,1), "1", tests, passes);
  test_eq(s(4), nsel(b,4), "4", tests, passes);
  test_eq(s(8), nsel(b,8), "8", tests, passes);
  test_eq(s(9), nsel(b,9), "9", tests, passes);
  test_eq(s(12), nsel(b,12), "12", tests, passes);
  test_eq(s(19), nsel(b,19), "19", tests, passes);
  test_eq(s(21), nsel(b,21), "21", tests, passes);
  test_eq(s(25), nsel(b,25), "25", tests, passes);
  test_eq(s(26), nsel(b,26), "26", tests, passes);
  test_eq(s(27), nsel(b,27), "27", tests, passes);
  test_eq(s(28), nsel(b,28), "28", tests, passes);
  test_eq(s(29), nsel(b,29), "29", tests, passes);
  test_eq(s(30), nsel(b,30), "30", tests, passes);
  test_eq(s(31), nsel(b,31), "31", tests, passes);
  test_eq(s(34), nsel(b,34), "34", tests, passes);
  
  printf("Test complete. %f%% pass rate, %d pass, %d fail, %d total.\n",
         (double(passes)/double(tests))*100, passes, tests - passes, tests);
  
  printf("Overhead: %lu bits\n", s.overhead());

  benchmark(100, 100000);
  benchmark(200, 100000);
  benchmark(400, 100000);
  benchmark(800, 100000);
  benchmark(1600, 100000);
  benchmark(3200, 100000);
  benchmark(6400, 100000);
  benchmark(12800, 100000);
  benchmark(25600, 100000);
  benchmark(51200, 100000);
  benchmark(102400, 100000);
  benchmark(204800, 100000);
  benchmark(409600, 100000);
  benchmark(819200, 100000);
  benchmark(1638400, 100000);
  benchmark(3276800, 100000);
  benchmark(6553600, 100000);
  benchmark(3276800, 100000);
  benchmark(13107200, 100000);
  
  return 0;
}
