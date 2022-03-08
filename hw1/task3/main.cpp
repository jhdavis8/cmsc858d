#include <cassert>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include "sparse_array.hpp"

void benchmark(uint64_t size, uint64_t ops, uint64_t maxsize) {
  sparse_array sp(maxsize);
  uint64_t conflicts = 0;
  for (int i = 0; i < size; i++) {
    uint64_t ind = rand() % maxsize;
    if (!sp.append("elm", ind)) conflicts++;
  }
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  volatile bool result;
  std::string stres;
  for (int i = 0; i < ops; i++) {
    uint64_t ind = rand() % (size - 1 - conflicts);
    result = sp.get_at_rank(ind, stres);
  }
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  double ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  printf("Benchmark N = %10lu, ops = %10lu: Runtime %10f ms.\n",
         size, ops, ms);
}

int main() {
  srand(time(NULL));

  /*
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
  */
  
  benchmark(10, 100000, 1000);
  benchmark(100, 100000, 10000);
  benchmark(1000, 100000, 100000);
  benchmark(10000, 100000, 1000000);
  benchmark(50, 100000, 1000);
  benchmark(500, 100000, 10000);
  benchmark(5000, 100000, 100000);
  benchmark(50000, 100000, 1000000);
  benchmark(100, 100000, 1000);
  benchmark(1000, 100000, 10000);
  benchmark(10000, 100000, 100000);
  benchmark(100000, 100000, 1000000);
  
  return 0;
}
