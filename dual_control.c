#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include "logging.h"
#include "token.h"
#include "conversation.h"

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    const char *returned_token = ask_for_token(pamh);
    int returned_validation = validate_token(returned_token);

    if (returned_validation) {
        log_success();
        return PAM_SUCCESS;
    } else {
        return PAM_AUTH_ERR;
    }
}

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}
