#include "validator.h"
#include "test_util.h"

bool validator_validates() {

   // given
   validator v;


   // when
   bool actual = v.validate("user", "token");


   // then
   check(actual, "should be valid");
   succeed();
}

bool validator_fails_unknown_user() {

   // given
   validator v;


   // when
   bool actual = v.validate("notuser", "token");


   // then
   check(!actual, "should not be valid");
   succeed();
}

bool validator_fails_incorrect_token() {

   // given
   validator v;


   // when
   bool actual = v.validate("user", "nottoken");


   // then
   check(!actual, "should not be valid");
   succeed();
}

RESET_VARS_START
RESET_VARS_END

bool run_tests() {
    test(validator_validates);
    test(validator_fails_unknown_user);
    test(validator_fails_incorrect_token);
    succeed();

}

int main(int argc, char *argv[]) {
    return !run_tests();
}

