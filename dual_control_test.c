#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <stdio.h>

#include "conversation.h"
#include "token.h"
#include "testutil.h"

const char *validated_token = "";
const char *token_to_return = "";
int validation_to_return = 0;
int log_success_invoked = 1;
int at_least_one_failed_test = 0;
pam_handle_t *passed_pam_handle = NULL;

#define reset_vars() \
  validated_token = ""; \
  validation_to_return = 1; \
  passed_pam_handle = NULL; \
  log_success_invoked = 0

const char *ask_for_token(pam_handle_t *pamh) {
    passed_pam_handle = pamh;
    return token_to_return;
}

int validate_token(const char *token) {
    validated_token = token;
    return validation_to_return;
}

void log_success() {
    log_success_invoked = 1;
}

int pam_sm_setcred_returns_success() {
    //given

    //when
    int result = pam_sm_setcred(NULL, 0, 0, NULL);

    //then
    checkint(PAM_SUCCESS, result, "function return");
    succeed();

}

int pam_sm_authenticate_validates_with_received_token() {
    // given
    token_to_return = "mytoken";
    pam_handle_t *handle = (pam_handle_t*)"";

    // when
    pam_sm_authenticate(handle, 0, 0, NULL);

    // then
    checkstr("mytoken",validated_token, "validated token");
    check(passed_pam_handle == handle, "incorrect handle");
    succeed();
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

int runtests() {
    test(pam_sm_authenticate_validates_with_received_token);
    test(pam_sm_setcred_returns_success);
    test(pam_sm_authenticate_success_invokes_log_success);
    test(succeeds_with_valid_token);
    test(fails_with_invalid_token);
    succeed();
}

int main(int argc, char* argv[]) {
   return !runtests();
}

