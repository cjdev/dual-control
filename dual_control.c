#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <stdio.h>

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    printf("Acct mgmt\n");
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    printf("Authentication\n");
    return PAM_SUCCESS;
}
