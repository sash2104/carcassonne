#include <iostream>

#include "test_util.hpp"

static int passed_n;
static int failed_n;

void start_test() {
  passed_n = 0;
  failed_n = 0;
}

void test_assert(char* test_name, bool cond) {
  if (cond) {
    std::cout << "Passed: " << test_name << std::endl;
    passed_n++;
  } else {
    std::cout << "Failed: " << test_name << std::endl;
    failed_n++;
  }
}

int end_test() {
  std::cout << std::endl;
  std::cout << "Passed: " << passed_n << ", Failed: " << failed_n << std::endl;
  if (failed_n > 0) {
    std::cout << "TEST FAILED" << std::endl;
    return -1;
  } else {
    std::cout << "OK" << std::endl;
    return 0;
  }
}
