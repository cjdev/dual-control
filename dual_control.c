#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <stdio.h>
#include "logging.h"
#include "token.h"

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    printf("Authentication\n");

    int returned_validation = validate_token("str");

    if (returned_validation) {
        log_success();
        return PAM_SUCCESS;
    } else {
        return PAM_AUTH_ERR;
    }
}

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    printf("Set cred\n");
    return PAM_SUCCESS;
}
