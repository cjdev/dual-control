#ifndef _CONVERSATION_H
#define _CONVERSATION_H

#include <security/pam_modules.h>

class token_conversation {
    public:
        virtual ~token_conversation() {}
        virtual std::string token() = 0;
        virtual std::string user_name() = 0;
}

class pam_token_conversation : token_conversation {
    public:
        pam_token_conversation(pam_handle_t *pamh);
}

char const *ask_for_token(pam_handle_t *pamh);

#endif

