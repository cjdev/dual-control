#include "validator.h"
#include "test_util.h"

bool validator_validates() {

   // given
   validator v;


   // when
   bool actual = v.validate("user", "code");


   // then
   check(actual, "user and code should be valid");
   succeed();
}

RESET_VARS_START
RESET_VARS_END

bool run_tests() {
    test(validator_validates);
    succeed();

}

int main(int argc, char *argv[]) {
    return !run_tests();
}

