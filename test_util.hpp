#ifndef __TEST_UTIL_HPP__
#define __TEST_UTIL_HPP__

#include <string>

void start_test();
void test_assert(const std::string& test_name, bool cond);
int end_test();

#endif
