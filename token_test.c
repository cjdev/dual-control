#include <string.h>
#include "token.h"
#include "test_util.h"

const char *fake_user = "";
const char *fake_user_token = "";

struct passwd;

// all the fake system calls
int fake_getpwnam_r(const char *nam, struct passwd *pwd, char *buffer, size_t bufsize, struct passwd **result) {
  int ok = !strcmp(nam, fake_user);
  *result = ok ? (struct passwd *)"" : 0;
  return !ok;
}

RESET_VARS_START
fake_user = "";
fake_user_token = "";
RESET_VARS_END


int validate_compares_to_user_token() {

    // given
    fake_user = "msmith";
    fake_user_token = "123456";

    // when
    int valid = validate_token("msmith:123456");

    // then
    check(valid, "expected result to be valid");

    succeed();

}

int validates_from_the_right_user() {
    //given
    fake_user = "jbalcita";
    fake_user_token = "123456";

    //when
    int valid = validate_token("msmith:12346");

    //then
    check(!valid, "expected result to be invalid");
    succeed();
}

int runtests() {
    test(validate_compares_to_user_token);
    test(validates_from_the_right_user);
    succeed();
}

int main(int argc, char **argv) {
    return !runtests();
}


