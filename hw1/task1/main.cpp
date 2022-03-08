#include <cassert>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include "rank_support.hpp"

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
  for (int i = 0; i < size; i++) {
    uint64_t val = rand() % 2;
    b.push_back(val);
  }
  rank_support r(b);
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  volatile uint64_t result;
  for (int i = 0; i < ops; i++) {
    result = r(rand() % size);
  }
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  double ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  printf("Benchmark N = %10lu, ops = %10lu: Memory overhead = %10lu bits, runtime %10f ms.\n",
         size, ops, r.overhead(), ms);
}

uint64_t scan(const bitvector& b, uint64_t ind) {
  uint64_t scan = 0;
  for (uint64_t i = 0; i <= ind; i++) scan += b[i];
  return scan;
}

int main() {
  srand(time(NULL));
  std::string n("NAME");
  printf("%-10sRESULT    DETAILS\n", n.c_str());
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

  r.save("rank-save-test.dat");
  printf("Save done.\n");
  bitvector c;
  c.assign({1});
  rank_support r1(c);
  r1.load("rank-save-test.dat");
  printf("Load done.\n");
  
  test_eq(r1(0), scan(b,0), "0", tests, passes);
  test_eq(r1(1), scan(b,1), "1", tests, passes);
  test_eq(r1(4), scan(b,4), "4", tests, passes);
  test_eq(r1(8), scan(b,8), "8", tests, passes);
  test_eq(r1(19), scan(b,19), "19", tests, passes);
  test_eq(r1(35), scan(b,35), "35", tests, passes);
  test_eq(r1(36), scan(b,36), "36", tests, passes);
  test_eq(r1(37), scan(b,37), "37", tests, passes);
  test_eq(r1(43), scan(b,43), "43", tests, passes);
  test_eq(r1(50), scan(b,50), "50", tests, passes);
  test_eq(r1(52), scan(b,52), "52", tests, passes);
  test_eq(r1(60), scan(b,60), "60", tests, passes);
  test_eq(r1(61), scan(b,61), "61", tests, passes);
  test_eq(r1(62), scan(b,62), "62", tests, passes);
  
  printf("Test complete. %f%% pass rate, %d pass, %d fail, %d total.\n",
         (double(passes)/double(tests))*100, passes, tests - passes, tests);
  
  printf("Overhead: %lu bits\n", r.overhead());
  //r.print_members();
  //r1.print_members();

  benchmark(100, 1000000);
  benchmark(200, 1000000);
  benchmark(400, 1000000);
  benchmark(800, 1000000);
  benchmark(1600, 1000000);
  benchmark(3200, 1000000);
  benchmark(6400, 1000000);
  benchmark(12800, 1000000);
  benchmark(25600, 1000000);
  benchmark(51200, 1000000);
  benchmark(102400, 1000000);
  benchmark(204800, 1000000);
  benchmark(409600, 1000000);
  benchmark(819200, 1000000);
  benchmark(1638400, 1000000);
  benchmark(3276800, 1000000);
  benchmark(6553600, 1000000);
  benchmark(3276800, 1000000);
  benchmark(13107200, 1000000);
  
  return 0;
}
