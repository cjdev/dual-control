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

#include "request.h"
#include "conversation.h"
#include "test_util.h"
#include "sys_pam.h"

struct conversation_data {
    std::vector<pam_message> expected_prompts;
    std::vector<pam_response> responses;
    int return_value;
};

const std::string token_prompt ("Dual control token (<user>:<token>): ");
const std::string reason_prompt ("Reason: ");

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
    std::vector<conversation_data> conversations_;
    int get_response_;
    mutable pam_conv token_conv_;
    mutable pam_conv reason_conv_;
    mutable int count_;
public:
    fake_pam (pam_handle *expected_handle,
              const std::vector<conversation_data> &conversations)
        : expected_handle_ (expected_handle),
          conversations_ (conversations),
          get_response_ (PAM_SUCCESS),
          count_ (0)
    {}
    fake_pam (int get_response) : get_response_ (get_response) {}
    int get_conv (pam_handle *handle, const pam_conv **out) const
    {
        if (get_response_ != PAM_SUCCESS) {
            return get_response_;
        }

        if (expected_handle_ != handle) {
            throw std::string ("unexpected handle");
        }

        pam_conv *conv_p;
        conv_p = count_ == 0 ? &token_conv_ : &reason_conv_;
        conv_p->appdata_ptr = (void *) (&conversations_[count_]);
        conv_p->conv = fake_conv;
        *out = conv_p;
        count_ += 1;

        return PAM_SUCCESS;
    }
};

template<class T>
std::shared_ptr<T> share (T *t)
{
    return std::shared_ptr<T> (t);
}

std::vector<pam_message> create_messages (const std::string &prompt)
{
    pam_message message;
    message.msg_style = prompt == token_prompt ? PAM_PROMPT_ECHO_OFF :
                        PAM_PROMPT_ECHO_ON;
    message.msg = const_cast<char *> (prompt.c_str());

    std::vector<pam_message> messages;
    messages.push_back (message);

    return messages;
}

std::vector<pam_response> create_responses (const std::string &answer,
        int retcode)
{
    pam_response response;
    response.resp_retcode = retcode;
    response.resp = const_cast<char *> (answer.c_str());

    std::vector<pam_response> responses;
    responses.push_back (response);

    return responses;
}

conversation successful_conversation (pam_handle *expected_handle,
                                      const std::string &token_answer,
                                      const std::string &reason_answer)
{
    std::vector<pam_message> token_messages (create_messages (
                token_prompt));
    std::vector<pam_response> token_responses (create_responses (
                token_answer,
                PAM_SUCCESS));
    std::vector<pam_message> reason_messages (create_messages (
                reason_prompt));
    std::vector<pam_response> reason_responses (create_responses (
                reason_answer,
                PAM_SUCCESS));

    conversation_data token_conv_data = {
        token_messages,
        token_responses,
        PAM_SUCCESS
    };
    conversation_data reason_conv_data = {
        reason_messages,
        reason_responses,
        PAM_SUCCESS
    };

    std::vector<conversation_data> conversations;
    conversations.push_back (reason_conv_data);
    conversations.push_back (token_conv_data);

    pam pam (share (new fake_pam (expected_handle, conversations)));
    return conversation::create (pam);
}

bool check_conversation_response (const std::string &token_answer,
                                  const std::string &expected_reason,
                                  const std::string &expected_user,
                                  const std::string &expected_token)
{
    // given
    pam_handle *handle = reinterpret_cast<pam_handle *> (29039);
    conversation conversation (successful_conversation (handle, token_answer,
                               expected_reason));
    pam_request request (handle, 0, 0, 0);

    // when
    conversation_result actual = conversation.initiate (request);

    // then
    check (actual.user_name == expected_user, "user name does not match");
    check (actual.token == expected_token, "token does not match");
    check (actual.reason == expected_reason, "reason does not match");

    succeed();
}

bool returns_user_token_and_reason()
{
    std::string user ("user");
    std::string token ("token");
    std::string reason ("test reason");
    return check_conversation_response (user + ":" + token, reason, user,
                                        token);
}

int returns_empty_user_and_token_when_no_colon()
{
    return check_conversation_response ("nocolon", "test reason", "", "");
}

int returns_empty_user_and_token_when_empty_token_answer()
{
    return check_conversation_response ("", "test reason", "", "");
}

int returns_empty_token_when_colon_end()
{
    std::string user ("user");
    std::string reason ("test reason");
    return check_conversation_response (user + ":", reason, user, "");
}

int returns_empty_user_when_colon_start()
{
    std::string token ("token");
    std::string reason ("test reason");
    return check_conversation_response (":" + token, reason, "", token);
}

int returns_empty_conversation_result_when_pam_cant_create_conversation()
{
    // given
    pam pam (share (new fake_pam (PAM_SERVICE_ERR)));
    conversation conversation = conversation::create (pam);
    pam_request request (0, 0, 0, 0);

    // when
    conversation_result actual = conversation.initiate (request);

    // then
    check (actual.user_name == "", "user name should be empty");
    check (actual.token == "", "token should be empty");
    check (actual.reason == "", "reason should be empty");
    succeed();
}

int returns_empty_user_and_token_when_token_answer_fails()
{
    //given
    const std::string user ("user");
    const std::string token ("token");
    const std::string token_answer (user + ":" + token);
    const std::string reason ("test reason");
    int token_retcode = PAM_CONV_ERR;
    int reason_retcode = PAM_SUCCESS;

    std::vector<pam_message> token_messages (create_messages (
                token_prompt));
    std::vector<pam_response> token_responses (create_responses (
                user + ":" + token,
                token_retcode));
    std::vector<pam_message> reason_messages (create_messages (
                reason_prompt));
    std::vector<pam_response> reason_responses (create_responses (reason,
            reason_retcode));

    conversation_data token_conversation_data = {
        token_messages,
        token_responses,
        token_retcode
    };
    conversation_data reason_conversation_data = {
        reason_messages,
        reason_responses,
        reason_retcode
    };
    std::vector<conversation_data> conversations;
    conversations.push_back (reason_conversation_data);
    conversations.push_back (token_conversation_data);
    pam pam (share (new fake_pam (0, conversations)));
    conversation conversation = conversation::create (pam);
    pam_request request (0, 0, 0, 0);

    // when
    conversation_result actual = conversation.initiate (request);

    // then
    check (actual.user_name == "", "user name should be empty");
    check (actual.token == "", "token should be empty");
    check (actual.reason == "test reason", "reason does not match");
    succeed();
}

int returns_empty_reason_when_reason_answer_fails()
{
    const std::string user ("user");
    const std::string token ("token");
    const std::string token_answer (user + ":" + token);
    const std::string reason ("test reason");
    int token_retcode = PAM_SUCCESS;
    int reason_retcode = PAM_CONV_ERR;

    std::vector<pam_message> token_messages (create_messages (
                token_prompt));
    std::vector<pam_response> token_responses (create_responses (
                token_answer,
                token_retcode));
    std::vector<pam_message> reason_messages (create_messages (
                reason_prompt));
    std::vector<pam_response> reason_responses (create_responses (reason,
            reason_retcode));

    conversation_data token_conversation_data = {
        token_messages,
        token_responses,
        token_retcode
    };
    conversation_data reason_conversation_data = {
        reason_messages,
        reason_responses,
        reason_retcode
    };
    std::vector<conversation_data> conversations;
    conversations.push_back (reason_conversation_data);
    conversations.push_back (token_conversation_data);
    pam pam (share (new fake_pam (0, conversations)));
    conversation conversation = conversation::create (pam);
    pam_request request (0, 0, 0, 0);

    // when
    conversation_result actual = conversation.initiate (request);

    // then
    check (actual.user_name == user, "user name does not match");
    check (actual.token == token, "token does not match");
    check (actual.reason == "", "reason should be empty");
    succeed();
}

int run_tests()
{
    test (returns_user_token_and_reason);
    test (returns_empty_user_and_token_when_no_colon);
    test (returns_empty_user_and_token_when_empty_token_answer);
    test (returns_empty_token_when_colon_end);
    test (returns_empty_user_when_colon_start);
    test (returns_empty_conversation_result_when_pam_cant_create_conversation);
    test (returns_empty_user_and_token_when_token_answer_fails);
    test (returns_empty_reason_when_reason_answer_fails);
    succeed();
}

int main (int argc, char **argv)
{
    return !run_tests();
}
