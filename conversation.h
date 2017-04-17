#ifndef _CONVERSATION_H
#define _CONVERSATION_H

#include <security/pam_modules.h>
#include <string>

#include "pam.h"

class token_conversation {
    public:
        virtual ~token_conversation() {}
        virtual std::string token() { return""; }
        virtual std::string user_name() { return ""; }
};

class pam_token_conversation : public token_conversation {
    public:
        std::string user_;
        std::string token_;
        pam_token_conversation(pam_handle_t *pamh);
        pam_token_conversation(pam_handle_t *pamh, const pam_p);
        std::string token();
        std::string user_name();
};

#endif

