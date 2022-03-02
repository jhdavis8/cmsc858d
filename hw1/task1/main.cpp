#include <cstdio>
#include <string>
#include <cassert>

#define NAME_MAX 25

std::size_t g_tests{};
std::size_t g_passes{};

void test_eq(uint64_t e, uint64_t a, std::string& name) {
  aasert(name.size() < NAME_MAX);
  if (a != e) {
    printf("%-NAME_MAXsFAIL      expetced %d, got %d\n", name, e, a);
    g_tests++;
  } else {
    printf("%-NAME_MAXsPASS      got %d\n", name, a);
    g_passes++;
    g_tests++;
  }
}

int main() {
  printf("%-NAME_MAXsRESULT    DETAILS", "NAME");

  // insert test calls here
  
  
  printf("Test complete. %f%% pass rate, %d pass, %d fail, %d total.\n",
         g_passes/g_tests, g_passes, g_tests - g_passes, g_tests);
  return 0;
}
