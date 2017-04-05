
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <stdio.h>

int log_success_invoked = 0;
void log_success() {
    log_success_invoked = 1;
}


int pam_sm_authenticate_returns_success() {
    //given

    //when
    int result = pam_sm_authenticate(NULL, 0, 0, NULL);

    //then
    return result == PAM_SUCCESS;
}

int pam_sm_setcred_returns_success() {
    //when
    int result = pam_sm_setcred(NULL, 0, 0, NULL);

    //then
    return result == PAM_SUCCESS;
}

int pam_sm_authenticate_invokes_log_success() {
    // given
    log_success_invoked = 0;

    //when
   pam_sm_authenticate(NULL, 0, 0, NULL);
   return log_success_invoked;
}


int main(int argc, char* argv[]) {
    int test2_result = pam_sm_authenticate_returns_success();
    if (!test2_result) {
        fprintf(stderr, "acct auth failed\n");
    }

    int test3_result = pam_sm_setcred_returns_success();
    if (!test3_result) {
        fprintf(stderr, "set cred failed\n");
    }

    int test4_result = pam_sm_authenticate_invokes_log_success();
    if (!test4_result) {
        fprintf(stderr, "authenticate invokes log_success failed\n");
    }

    if (test2_result && test3_result && test4_result) {
        fprintf(stderr, "success\n");
        return 0;
    } else {
        return 1;
    }
}




