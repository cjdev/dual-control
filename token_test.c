#include "token.h"
#include "test_util.h"

const char *fake_user = "";
const char *fake_user_token = "";


// all the fake system calls


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


int runtests() {
    test(validate_compares_to_user_token);
    succeed();
}

int main(int argc, char **argv) {
    return !runtests();
}


