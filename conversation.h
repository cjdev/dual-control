#ifndef _CONVERSATION_H
#define _CONVERSATION_H

#include <security/pam_modules.h>
#include <string>


class token_conversation {
    public:
        virtual ~token_conversation() {}
        virtual std::string token() { return""; }
        virtual std::string user_name() { return ""; }
};

class pam_token_conversation : public token_conversation {
    public:
        pam_token_conversation(pam_handle_t *pamh);
        std::string token();
        std::string user_name();
};

#endif

