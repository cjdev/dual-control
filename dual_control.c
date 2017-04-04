#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <stdio.h>
#include "logging.h"

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    printf("Acct mgmt\n");
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    printf("Authentication\n");
    log_dual_control();

    struct pam_conv *conversation;
    int get_item_result = pam_get_item(pamh, PAM_CONV, (void *)&conversation);
    if (get_item_result != PAM_SUCCESS) {
        return get_item_result;
    }

    struct pam_message msg1;
    msg1.msg_style = PAM_PROMPT_ECHO_ON;
    msg1.msg = "Birthday: ";

    struct pam_message *messages[1];
    messages[0] = &msg1;


    struct pam_response *responses[1];


    int conversation_result  = conversation->conv(1, &messages, &responses, conversation->appdata_ptr);
    if (conversation_result != PAM_SUCCESS) {
        return conversation_result;
    }

   printf("code: %d   response: %s\n", responses[0]->resp_retcode, responses[0]->resp);
   return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    printf("Set cred\n");
    return PAM_SUCCESS;
}
