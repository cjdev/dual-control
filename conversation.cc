#include <security/pam_modules.h>

#include "conversation.h"


pam_token_conversation::pam_token_conversation(pam_handle_t *pamh) {}

std::string pam_token_conversation::token() {
    return "token";
}

