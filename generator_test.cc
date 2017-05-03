#include <algorithm>

#include "generator.h"
#include "test_util.h"

int six_digits()  {
   // given
   generator generator = make_generator();

  // when
  auto actual = generator();


  // then
  check(actual.size() == 6, "size is wrong");
  check(std::all_of(actual.begin(), actual.end(), [](char c){ return c >= '0' && c <= '9';}), "not just digits");
  succeed();
}


int run_tests() {
    test(six_digits);
    succeed();
}

int main(int argc, char *argv[]) {
    return !run_tests();
}

