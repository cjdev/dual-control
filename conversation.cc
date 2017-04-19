#include <security/pam_modules.h>
#include <algorithm>
#include <vector>
#include <string>

#include "conversation.h"
#include "pam.h"

pam_token_conversation::pam_token_conversation (pam_handle_t *pamh,
        const pam_p pam)
{
    pam_conversation_p pam_conversation;
    int get_conversation_result = pam->get_conversation (pamh,
                                  pam_conversation);

    if (get_conversation_result != 0) {
        return;
    }

    struct pam_message prompt;

    std::string message ("Dual control token: ");

    prompt.msg = const_cast<char *> (message.c_str());

    prompt.msg_style = PAM_PROMPT_ECHO_OFF;

    std::vector<const struct pam_message *> prompts (1);

    prompts[0] = &prompt;

    std::vector<struct pam_response *> answers (1);

    int conversation_result = pam_conversation->conv (prompts, answers);

    if (conversation_result) {
        return;
    }

    if (answers[0]->resp_retcode) {
        return;
    }

    std::string answer (answers[0]->resp);
    std::string::iterator delim = std::find (answer.begin(), answer.end(), ':');

    if (delim == answer.end()) {
        return;
    }

    std::string user_name (answer.begin(), delim);
    std::string token (delim + 1, answer.end());
    user_ = user_name;
    token_ = token;
}

std::string pam_token_conversation::token()
{
    return token_;
}

std::string pam_token_conversation::user_name()
{
    return user_;
}

