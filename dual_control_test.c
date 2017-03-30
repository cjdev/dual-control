
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <stdio.h>

int pam_sm_acct_mgmt_returns_successs() {
    // given

    // when
    int result = pam_sm_acct_mgmt(NULL, 0, 0, NULL);

    // then
    return result == PAM_SUCCESS;
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

int main(int argc, char* argv[]) {
    int test1_result = pam_sm_acct_mgmt_returns_successs();
    if (!test1_result) {
        printf("acct management failed\n");
    }

    int test2_result = pam_sm_authenticate_returns_success();
    if (!test2_result) {
        printf("acct auth failed\n");
    }

    int test3_result = pam_sm_setcred_returns_success();
    if (!test3_result) {
        printf("set cred failed\n");
    }


    if (test1_result && test2_result && test3_result) {
        printf("success!\n");
        return 0;
    } else {
        printf("fail\n");
        return 1;
    }
}


