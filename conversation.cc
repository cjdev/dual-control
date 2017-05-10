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

        if (get_conv_result != PAM_SUCCESS) {
            return result;
        }

        pam_message token_message;
        token_message.msg = const_cast<char *> ("Dual control token: ");
        token_message.msg_style = PAM_PROMPT_ECHO_OFF;
        pam_message reason_message;
        reason_message.msg = const_cast<char *> ("Reason: ");
        reason_message.msg_style = PAM_PROMPT_ECHO_OFF;
        std::vector<const pam_message *> messages;
        messages.push_back (&token_message);
        messages.push_back (&reason_message);
        std::vector<pam_response *> responses (2);
        int conv_result = conv->conv (2, messages.data(), responses.data(),
                                      conv->appdata_ptr);

        if (conv_result != PAM_SUCCESS) {
            return result;
        }

        if (responses[1]->resp_retcode == 0) {
            result.reason = responses[1]->resp;
        }

        if (responses[0]->resp_retcode != 0) {
            return result;
        }

        std::string answer (responses[0]->resp);
        std::string::iterator delim = std::find (answer.begin(), answer.end(), ':');

        if (delim == answer.end()) {
            return result;
        }

        result.user_name = std::string (answer.begin(), delim);
        result.token = std::string (delim + 1, answer.end());
        return result;
    }
};
}

conversation conversation::create (pam &pam)
{
    return conversation (std::shared_ptr<conversation_ifc> (new impl (pam)));
}

