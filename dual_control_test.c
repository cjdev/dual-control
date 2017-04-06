#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <stdio.h>

#include "conversation.h"
#include "token.h"

const char *token_to_return = "";
pam_handle_t *passed_pam_handle = NULL;
const char *ask_for_token(pam_handle_t *pamh) {
    passed_pam_handle = pamh;
    return token_to_return;
}

int validation_to_return = 0;
int validate_token(const char *token) {
    return validation_to_return;
}


int log_success_invoked = 0;
void log_success() {
    log_success_invoked = 1;
}


int pam_sm_setcred_returns_success() {
    //given

    //when
    int result = pam_sm_setcred(NULL, 0, 0, NULL);

    //then
    return result == PAM_SUCCESS;
}

int pam_sm_authenticate_success_invokes_log_success() {
    // given
    validation_to_return = 1;
    log_success_invoked = 0;

    //when
   pam_sm_authenticate(NULL, 0, 0, NULL);
   return log_success_invoked;
}

int succeeds_with_valid_token() {
    //given
    validation_to_return = 1;

    //when
    int result = pam_sm_authenticate(NULL, 0, 0, NULL);

    //then
    return result == PAM_SUCCESS;
}

int fails_with_invalid_token() {
    //given
    validation_to_return = 0;

    //when
    int result = pam_sm_authenticate(NULL, 0, 0, NULL);

    //then
    return result == PAM_AUTH_ERR;
}

int main(int argc, char* argv[]) {
    int test1_result = succeeds_with_valid_token();
    if (!test1_result) {
        fprintf(stderr, "succeds with valid token failed\n");
    }

    int test3_result = pam_sm_setcred_returns_success();
    if (!test3_result) {
        fprintf(stderr, "set cred failed\n");
    }

    int test4_result = pam_sm_authenticate_success_invokes_log_success();
    if (!test4_result) {
        fprintf(stderr, "authenticate invokes log_success failed\n");
    }

    int test5_result = fails_with_invalid_token();
    if (!test5_result) {
        fprintf(stderr, "fails with invalid token failed\n");
    }



    if (test1_result && test3_result && test4_result && test5_result) {
        fprintf(stderr, "success\n");
        return 0;
    } else {
        return 1;
    }
}


