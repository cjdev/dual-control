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

#include "trace.h"

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
        const pam_conv *conv;
        int get_conv_result = pam_.get_conv (request.handle(), &conv);
            TRACE

        if (get_conv_result != PAM_SUCCESS) {
            return result;
        }
            TRACE

        pam_message token_message;
        token_message.msg = const_cast<char *> ("Dual control token: ");
        token_message.msg_style = PAM_PROMPT_ECHO_OFF;

        pam_message reason_message;
        reason_message.msg = const_cast<char *> ("Reason: ");
        reason_message.msg_style = PAM_PROMPT_ECHO_OFF;

        std::vector<const pam_message *> messages1;
        messages1.push_back (&token_message);

        std::vector<const pam_message *> messages2;
        messages2.push_back(&reason_message);


        std::vector<pam_response *> responses1 (1);
        std::vector<pam_response *> responses2 (1);


        int token_result = conv->conv (1, messages1.data(), responses1.data(),
                                      conv->appdata_ptr);
        int reason_result = conv->conv (1, messages2.data(), responses2.data(),
                                      conv->appdata_ptr);

        if (token_result != PAM_SUCCESS) {
            return result;
        }

        if (reason_result != PAM_SUCCESS) {
            return result;
        }
            TRACE

        std::string answer (responses1[0]->resp);
        std::string::iterator delim = std::find (answer.begin(), answer.end(), ':');

        if (delim == answer.end()) {
            return result;
        }

        std::string reason(responses2[0]->resp);

        result.user_name = std::string (answer.begin(), delim);
        result.token = std::string (delim + 1, answer.end());
        result.reason = reason;
        return result;
    }
};
}

conversation conversation::create (pam &pam)
{
    return conversation (std::shared_ptr<conversation_ifc> (new impl (pam)));
}

