#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <cstring>
#include <cstdlib>

#include "logging.h"
#include "token.h"
#include "conversation.h"

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    const char *returned_token = ask_for_token(pamh);

    int returned_token_length = strlen(returned_token);
    char working_token[returned_token_length + 1];
    strcpy(working_token, returned_token);
    char *colon = strchr(working_token, ':');
    if(!colon) {
        return PAM_AUTH_ERR;
    }

    *colon = 0;
    char *user = working_token;
    char *token = colon + 1;

    int returned_validation = validate_token(user, token);

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

