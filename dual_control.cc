#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <cstring>
#include <cstdlib>

#include "logging.h"
#include "conversation.h"
#include "validator.h"

extern validator system_validator;

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {

    pam_token_conversation conversation(pamh);
    int returned_validation = system_validator.validate(conversation.user_name(), conversation.token());

    if (returned_validation) {
        log_success();
        return PAM_SUCCESS;
    } else {
        log_failure();
        return PAM_AUTH_ERR;
    }
}

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}

