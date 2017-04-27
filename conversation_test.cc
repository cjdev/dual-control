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

#include <vector>
#include <algorithm>
#include <string>
#include <algorithm>
#include <memory>
#include <cstring>
#include <security/pam_modules.h>

#include <iostream>

#include "request.h"
#include "conversation.h"
#include "test_util.h"
#include "pam.h"

struct conversation_data {
    std::vector<pam_message> expected_prompts;
    std::vector<pam_response> responses;
    int return_value;
};

bool same_prompts (const std::vector<pam_message> &expected,
                   int num_prompts, const pam_message **actual)
{
    if (expected.size() != num_prompts) {
        return false;
    }

    for (int i=0; i< num_prompts; ++i) {
        if (expected[i].msg_style != actual[i]->msg_style) {
            return false;
        }

        if (std::strcmp (expected[i].msg, actual[i]->msg)) {
            return false;
        }
    }

    return true;
}

template<class T>
T *address_of (T &t)
{
    return &t;
}

int fake_conv (int num_msg, const struct pam_message **msg,
               struct pam_response **resp, void *appdata_ptr)
{
    conversation_data *data = reinterpret_cast<conversation_data *>
                              (appdata_ptr);

    if (data->return_value != PAM_SUCCESS) {
        return data->return_value;
    }

    if (!same_prompts (data->expected_prompts, num_msg, msg)) {
        throw std::string ("unexpected prompts");
    }

    std::vector<pam_response> &responses = data->responses;
    std::transform (responses.begin(), responses.end(), resp,
                    address_of<pam_response>);
    return data->return_value;
}

class fake_pam : public pam_ifc
{
private:
    pam_handle *expected_handle_;
    conversation_data conversation_data_;
    int get_response_;
    pam_conv conv_;
public:
    fake_pam (pam_handle *expected_handle,
              const conversation_data &conversation_data)
        : expected_handle_ (expected_handle),
          conversation_data_ (conversation_data),
          get_response_ (PAM_SUCCESS)
    {}
    fake_pam (int get_response) : get_response_ (get_response) {}
    int get_conv (pam_handle *handle, const pam_conv **out)
    {
        if (get_response_ != PAM_SUCCESS) {
            return get_response_;
        }

        if (expected_handle_ != handle) {
            throw std::string ("unexpected handle");
        }

        conv_.appdata_ptr = reinterpret_cast<void *> (&conversation_data_);
        conv_.conv = fake_conv;
        *out = &conv_;
        return PAM_SUCCESS;
    }

};

template<class T>
std::shared_ptr<T> share (T *t)
{
    return std::shared_ptr<T> (t);
}

conversation make_conversation (pam_handle *expected_handle,
                                const std::string &answer)
{
    pam_message prompt;
    prompt.msg_style = PAM_PROMPT_ECHO_OFF;
    prompt.msg = const_cast<char *> ("Dual control token: ");
    pam_response response;
    response.resp_retcode = 0;
    std::string response_text (answer);
    response.resp = const_cast<char *> (response_text.c_str());
    conversation_data conversation_data = {
        std::vector<pam_message> (&prompt, &prompt + 1),
        std::vector<pam_response> (&response, &response + 1),
        PAM_SUCCESS
    };
    pam pam (share (new fake_pam (expected_handle, conversation_data)));
    return conversation::create (pam);

}

bool check_conversation_response (const std::string &answer,
                                  const std::string &expected_user, const std::string &expected_token)
{
    // given
    pam_handle *handle = reinterpret_cast<pam_handle *> (29039);
    conversation conversation (make_conversation (handle, answer));
    pam_request request (handle, 0, 0, 0);

    // when
    conversation_result actual = conversation.initiate (request);

    // then
    check (actual.user_name == expected_user, "user name does not match");
    check (actual.token == expected_token, "token does not match");

    succeed();
}

bool returns_user_and_token()
{
    std::string user ("user");
    std::string token ("token");
    return check_conversation_response (user + ":" + token, user, token);
}

int returns_empty_user_and_token_when_no_colon()
{
    return check_conversation_response ("nocolon", "", "");
}

int returns_empty_user_and_token_when_empty_answer()
{
    return check_conversation_response ("", "", "");
}

int returns_empty_token_when_colon_end()
{
    std::string user ("user");
    std::string token ("");
    return check_conversation_response (user + ":" + token, user, token);
}

int returns_empty_user_when_colon_start()
{
    std::string user ("");
    std::string token ("token");
    return check_conversation_response (user + ":" + token, user, token);
}

int returns_empty_user_and_token_when_pam_cant_create_conversation()
{
    // given
    pam pam (share (new fake_pam (PAM_SERVICE_ERR)));
    conversation conversation = conversation::create (pam);
    pam_request request (0, 0, 0, 0);

    // when
    conversation_result actual = conversation.initiate (request);

    // then
    check (actual.user_name == "", "user name does not match");
    check (actual.token == "", "token does not match");

    succeed();
}

int returns_empty_user_and_token_when_conversation_fails()
{
    pam_message prompt;
    prompt.msg_style = PAM_PROMPT_ECHO_OFF;
    prompt.msg = const_cast<char *> ("Dual control token: ");
    conversation_data conversation_data = {
        std::vector<pam_message> (&prompt, &prompt + 1),
        std::vector<pam_response> (),
        PAM_SERVICE_ERR
    };
    pam pam (share (new fake_pam (0, conversation_data)));
    conversation conversation = conversation::create (pam);
    pam_request request (0, 0, 0, 0);

    //when
    conversation_result actual = conversation.initiate (request);

    //then
    check (actual.user_name == "", "user name does not match");
    check (actual.token == "", "token does not match");
    succeed();
}

int returns_empty_user_and_token_when_conversation_answer_fails()
{
    pam_message prompt;
    prompt.msg_style = PAM_PROMPT_ECHO_OFF;
    prompt.msg = const_cast<char *> ("Dual control token: ");
    pam_response response;
    response.resp_retcode = 1212;
    std::string response_text ("user:token");
    response.resp = const_cast<char *> (response_text.c_str());
    conversation_data conversation_data = {
        std::vector<pam_message> (&prompt, &prompt + 1),
        std::vector<pam_response> (&response, &response + 1),
        PAM_SUCCESS
    };
    pam pam (share (new fake_pam (0, conversation_data)));
    conversation conversation = conversation::create (pam);
    pam_request request (0, 0, 0, 0);

    //when
    conversation_result actual = conversation.initiate (request);

    //then
    check (actual.user_name == "", "user name does not match");
    check (actual.token == "", "token does not match");
    succeed();
}

RESET_VARS_START
RESET_VARS_END

int run_tests()
{
    test (returns_user_and_token);
    test (returns_empty_user_and_token_when_no_colon);
    test (returns_empty_user_and_token_when_empty_answer);
    test (returns_empty_token_when_colon_end);
    test (returns_empty_user_when_colon_start);
    test (returns_empty_user_and_token_when_pam_cant_create_conversation);
    test (returns_empty_user_and_token_when_conversation_fails);
    test (returns_empty_user_and_token_when_conversation_answer_fails);
    succeed();
}

int main (int argc, char **argv)
{
    return !run_tests();
}

