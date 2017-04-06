#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <stdio.h>

#include "conversation.h"
#include "token.h"


const char *token_to_return = "";
int validation_to_return = 0;
int log_success_invoked = 1;
int at_least_one_failed_test = 0;
pam_handle_t *passed_pam_handle = NULL;

#define reset_vars() \
  validation_to_return = 1; \
  passed_pam_handle = NULL; \
  log_success_invoked = 0;

#define check_result(result, message) \
  if(!(result)) { \
    fprintf(stderr, message); \
    at_least_one_failed_test = 1; \
  }

const char *ask_for_token(pam_handle_t *pamh) {
    passed_pam_handle = pamh;
    return token_to_return;
}

int validate_token(const char *token) {
    return validation_to_return;
}

void log_success() {
    log_success_invoked = 1;
}

int pam_sm_setcred_returns_success() {
    //given
    reset_vars()

    //when
    int result = pam_sm_setcred(NULL, 0, 0, NULL);

    //then
    return result == PAM_SUCCESS;
}

int pam_sm_authenticate_success_invokes_log_success() {
    // given
    reset_vars()
    log_success_invoked = 0;

    //when
   pam_sm_authenticate(NULL, 0, 0, NULL);
   return log_success_invoked;
}

int succeeds_with_valid_token() {
    //given
    reset_vars()

    //when
    int result = pam_sm_authenticate(NULL, 0, 0, NULL);

    //then
    return result == PAM_SUCCESS;
}

int fails_with_invalid_token() {
    //given
    reset_vars()
    validation_to_return = 0;

    //when
    int result = pam_sm_authenticate(NULL, 0, 0, NULL);

    //then
    return result == PAM_AUTH_ERR;
}

int main(int argc, char* argv[]) {
    check_result(pam_sm_setcred_returns_success(), "set cred failed\n");
    check_result(pam_sm_authenticate_success_invokes_log_success(), "authenticate invokes log_success failed\n");
    check_result(succeeds_with_valid_token(), "succeeds with valid token failed\n");
    check_result(fails_with_invalid_token(), "fails with invalid token failed\n")

    if(at_least_one_failed_test) {
      return 1;
    } else {
      fprintf(stderr, "dual_control_test: success!\n");
      return 0;
    }
}
