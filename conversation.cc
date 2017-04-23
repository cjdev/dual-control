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
conversation_result err = {"",""};
class impl : public conversation_ifc
{
private:
    pam pam_;
public:
    impl (pam &pam) : pam_ (pam) {}
    conversation_result initiate (const pam_request &request)
    {
        const pam_conv *conv;
        int get_conv_result = pam_.get_conv (request.handle(), &conv);

        if (get_conv_result != PAM_SUCCESS) {
            return err;
        }

        pam_message msg;
        msg.msg = const_cast<char *> ("Dual control token: ");
        msg.msg_style = PAM_PROMPT_ECHO_OFF;
        std::vector<const pam_message *> messages;
        messages.push_back (&msg);
        std::vector<pam_response *> responses (1);
        int conv_result = conv->conv (1, messages.data(), responses.data(),
                                      conv->appdata_ptr);

        if (conv_result != PAM_SUCCESS) {
            return err;
        }

        if (responses[0]->resp_retcode != 0) {
            return err;
        }

        std::string answer (responses[0]->resp);
        std::string::iterator delim = std::find (answer.begin(), answer.end(), ':');

        if (delim == answer.end()) {
            return err;
        }

        return {
            std::string (answer.begin(), delim),
            std::string (delim + 1, answer.end())
        };

    }
};
}

conversation create_conversation (pam &pam)
{
    return conversation (std::shared_ptr<conversation_ifc> (new impl (pam)));
}
