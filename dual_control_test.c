
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

int main(int argc, char* argv[]) {
    if (pam_sm_acct_mgmt_returns_successs()) {
        printf("success!");
    } else {
        printf("fail");
    }
    return 0;
}

