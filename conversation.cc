/* Copyright (C) CJ Affiliate
 *
 * You may use, distribute and modify this code under  the
 * terms of the  GNU General Public License  version 2  or
 * later.
 *
 * You should have received a copy of the license with this
 * file. If not, you will find a copy in the "LICENSE" file
 * at https://github.com/cjdev/dual-control.
 */

#include <security/pam_modules.h>
#include <algorithm>
#include <vector>
#include <string>

#include "conversation.h"

namespace
{
class impl : public conversation_ifc
{
private:
    pam pam_;
public:
    impl (pam &pam) : pam_ (pam) {}
    conversation_result initiate (const pam_request &request)
    {
        conversation_result result;
        const pam_conv *token_conv;
        const pam_conv *reason_conv;
        int get_token_conv = pam_.get_conv (request.handle(), &token_conv);
        int get_reason_conv = pam_.get_conv (request.handle(), &reason_conv);

        if (get_token_conv != PAM_SUCCESS || get_reason_conv != PAM_SUCCESS) {
            return result;
        }

        pam_message token_message;
        token_message.msg = const_cast<char *> ("Dual control token: ");
        token_message.msg_style = PAM_PROMPT_ECHO_OFF;
        pam_message reason_message;
        reason_message.msg = const_cast<char *> ("Reason: ");
        reason_message.msg_style = PAM_PROMPT_ECHO_OFF;

        std::vector<const pam_message *> token_messages;
        token_messages.push_back (&token_message);
        std::vector<const pam_message *> reason_messages;
        reason_messages.push_back (&reason_message);

        std::vector<pam_response *> token_responses (1);
        std::vector<pam_response *> reason_responses (1);

        int token_result = token_conv->conv (1, token_messages.data(),
                                             token_responses.data(),
                                             token_conv->appdata_ptr);
        int reason_result = reason_conv->conv (1, reason_messages.data(),
                                               reason_responses.data(),
                                               reason_conv->appdata_ptr);

        if (reason_result == PAM_SUCCESS) {
            std::string reason (reason_responses[0]->resp);
            result.reason = reason;
        }

        if (token_result != PAM_SUCCESS) {
            return result;
        }

        std::string token_answer (token_responses[0]->resp);
        std::string::iterator delim = std::find (token_answer.begin(),
                                      token_answer.end(), ':');

        if (delim == token_answer.end()) {
            return result;
        }

        result.user_name = std::string (token_answer.begin(), delim);
        result.token = std::string (delim + 1, token_answer.end());
        return result;
    }
};
}

conversation conversation::create (pam &pam)
{
    return conversation (std::shared_ptr<conversation_ifc> (new impl (pam)));
}

