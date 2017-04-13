#include <security/pam_modules.h>

#include "conversation.h"
#include "test_util.h"


int returns_correct_token() {
    //given
    pam_handle_t *pamh;

    //when
    pam_token_conversation conversation(pamh);

    //then
    check(conversation.token() == "token", "returned incorrect token");
    succeed();
}



RESET_VARS_START
RESET_VARS_END

int run_tests() {
    test(returns_correct_token);
    succeed();
}

int main(int argc, char *args[]) {
    return !run_tests();
}


