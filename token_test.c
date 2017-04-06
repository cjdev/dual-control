#include "token.h"
#include "testutil.h"

int validate_compares_to_user_token() {
    fail();
}

RESET_VARS_START
RESET_VARS_END

int runtests() {
    test(validate_compares_to_user_token);
    succeed();
}

int main(int argc, char **argv) {
    return !runtests();
}


