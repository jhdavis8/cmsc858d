#include <cassert>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include "sparse_array.hpp"

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

int main() {
  srand(time(NULL));

  sparse_array s(100);
  s.append("a", 1);
  s.append("b", 4);
  s.append("c", 6);
  s.append("d", 9);
  s.append("e", 12);
  s.append("f", 13);
  s.append("g", 19);
  s.append("h", 22);
  s.append("i", 23);
  s.append("j", 27);
  s.append("k", 45);
  s.append("l", 50);
  s.append("m", 63);
  s.append("n", 68);
  s.append("o", 69);
  s.append("p", 78);
  s.append("q", 84);
  s.append("r", 92);
  s.append("s", 99);

  s.ranksup->print_members();

  std::string str;
  s.get_at_rank(0, str);
  printf("%s\n", str.c_str());
  s.get_at_rank(5, str);
  printf("%s\n", str.c_str());
  s.get_at_rank(14, str);
  printf("%s\n", str.c_str());
  s.get_at_index(1, str);
  printf("%s\n", str.c_str());
  s.get_at_index(45, str);
  printf("%s\n", str.c_str());
  s.get_at_index(99, str);
  printf("%s\n", str.c_str());
  printf("%lu\n", s.num_elem_at(0));
  printf("%lu\n", s.num_elem_at(10));
  printf("%lu\n", s.num_elem_at(90));
  printf("%lu\n", s.size());
  printf("%lu\n", s.num_elem());
  
  s.save("sparse-save-test.dat");
  printf("Save done.\n");
  sparse_array s1(1);
  s1.load("sparse-save-test.dat");
  printf("Load done.\n");

  s.get_at_rank(0, str);
  printf("%s\n", str.c_str());
  s.get_at_rank(5, str);
  printf("%s\n", str.c_str());
  s.get_at_rank(14, str);
  printf("%s\n", str.c_str());
  s.get_at_index(1, str);
  printf("%s\n", str.c_str());
  s.get_at_index(45, str);
  printf("%s\n", str.c_str());
  s.get_at_index(99, str);
  printf("%s\n", str.c_str());
  printf("%lu\n", s.num_elem_at(0));
  printf("%lu\n", s.num_elem_at(10));
  printf("%lu\n", s.num_elem_at(90));
  printf("%lu\n", s.size());
  printf("%lu\n", s.num_elem());
  
  /*
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
  */
  
  return 0;
}
