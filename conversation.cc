
/*Copyright (C) CJ Affiliate
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
    pam_conv_result conversation (const std::string &msg, int msg_style,
                                  const pam_request &request)
    {
        const pam_conv *conv;
        int get_conv_result = pam_.get_conv (request.handle(), &conv);

        if (get_conv_result != PAM_SUCCESS) {
            return pam_conv_result {get_conv_result, PAM_CONV_ERR, std::vector<pam_response *>()};
        }

        pam_message message;
        message.msg = const_cast<char *> (msg.c_str());
        message.msg_style = msg_style;

        std::vector<const pam_message *> messages;
        messages.push_back (&message);
        std::vector<pam_response *> responses (1);

        int conv_result = conv->conv (1, messages.data(),
                                      responses.data(),
                                      conv->appdata_ptr);

        return pam_conv_result {get_conv_result, conv_result, responses};
    }

public:
    impl (pam &pam) : pam_ (pam) {}
    conversation_result initiate (const pam_request &request)
    {
        conversation_result result;

        pam_conv_result reason_result (conversation ("Reason: ", PAM_PROMPT_ECHO_ON,
                                       request));

        if (reason_result.get_conv_result != PAM_SUCCESS) {
            return result;
        }

        if (reason_result.conv_result == PAM_SUCCESS) {
            std::string reason (reason_result.responses[0]->resp);
            result.reason = reason;
        }

        pam_conv_result seconduser_username (conversation ("Dual Control User: ",
                                             PAM_PROMPT_ECHO_ON, request));


        pam_conv_result seconduser_token (conversation ("Dual Control User Token: ",
                                          PAM_PROMPT_ECHO_OFF, request));
        if (seconduser_username.conv_result!= PAM_SUCCESS) {
            return result;
        }


        if (seconduser_token.conv_result!= PAM_SUCCESS) {
            return result;
        }



        result.user_name = std::string (seconduser_username.responses[0]->resp);
        result.token = std::string (seconduser_token.responses[0]->resp);

        return result;
    }
};
}

conversation conversation::create (pam &pam)
{
    return conversation (std::shared_ptr<conversation_ifc> (new impl (pam)));
}
