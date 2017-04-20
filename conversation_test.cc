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
    pam_conv conv_;
public:
    fake_pam (pam_handle *expected_handle,
              const conversation_data &conversation_data)
        : expected_handle_ (expected_handle),
          conversation_data_ (conversation_data)
    {}
    int get_conv (pam_handle *handle, const pam_conv **out)
    {
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
    return create_conversation (pam);

}

bool returns_user_and_token()
{

    // given
    pam_handle *handle = reinterpret_cast<pam_handle *> (29039);
    std::string user ("user");
    std::string token ("token");
    conversation conversation (make_conversation (handle, user + ":" + token));
    pam_request request (handle, 0, 0, 0);

    // when
    conversation_result actual = conversation.initiate (request);

    // then
    check (actual.user_name == user, "user name does not match");
    check (actual.token == token, "token does not match");

    succeed();
}

int returns_empty_user_and_token_when_no_colon()
{

    // given
    pam_handle *handle = reinterpret_cast<pam_handle *> (29039);
    conversation conversation (make_conversation (handle, "nocolon"));
    pam_request request (handle, 0, 0, 0);

    // when
    conversation_result actual = conversation.initiate (request);

    // then
    check (actual.user_name == "", "user name does not match");
    check (actual.token == "", "token does not match");
    succeed();
}

int returns_empty_user_and_token_when_empty_answer()
{
    // given
    pam_handle *handle = reinterpret_cast<pam_handle *> (29039);
    conversation conversation (make_conversation (handle, ""));
    pam_request request (handle, 0, 0, 0);

    // when
    conversation_result actual = conversation.initiate (request);

    // then
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
    succeed();
}

int main (int argc, char **argv)
{
    return !run_tests();
}

/*
int returns_empty_user_and_token_when_empty_answer()
int returns_empty_token_when_colon_end()
int returns_empty_user_when_colon_begin()
int returns_empty_user_and_token_when_pam_cant_create_conversation()
int prompts_user_with_correct_text()
int prompts_user_with_correct_style()
int returns_empty_user_and_token_when_conversation_fails()
int returns_empty_user_and_token_when_conversation_answer_fails()
*/

