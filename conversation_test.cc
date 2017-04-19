#include <vector>
#include <algorithm>
#include <string>
#include <security/pam_modules.h>

#include "conversation.h"
#include "test_util.h"
#include "pam.h"


class fake_pam_conversation : public pam_conversation
{
private:
    pam_response response_;
    std::string answer_;
public:
    fake_pam_conversation(const std::string &answer) : answer_(answer) {}
    int conv(const std::vector<const struct pam_message *> &prompts, std::vector<struct pam_response *> &answers)
    {
        if (prompts.size() != 1) {
            throw std::string("test only supports one prompt");
        }
        response_.resp_retcode = 0;
        response_.resp = const_cast<char *>(answer_.c_str());
        answers.resize(1);
        answers[0] = &response_;
        return 0;
    }
};

class fake_failing_conversation: public pam_conversation
{

public:
    int conv(const std::vector<const struct pam_message *> &prompts, std::vector<struct pam_response *> &answers)
    {
        return 1;
    }
};

class fake_failing_answer_conversation: public pam_conversation
{
private:
    pam_response response_;
    std::string answer_;
public:
    fake_failing_answer_conversation() : answer_("ok:1") {}
    int conv(const std::vector<const struct pam_message *> &prompts, std::vector<struct pam_response *> &answers)
    {
        if (prompts.size() != 1) {
            throw std::string("test only supports one prompt");
        }
        response_.resp_retcode = 13;
        response_.resp = const_cast<char *>(answer_.c_str());
        answers.resize(1);
        answers[0] = &response_;
        return 0;
    }
};

class match_prompt_text_conversation : public pam_conversation
{
private:
    pam_response response_;
    std::string answer_;
    std::string prompt_;
public:
    match_prompt_text_conversation(const std::string &prompt) : prompt_(prompt), answer_("ok:123") {}
    int conv(const std::vector<const struct pam_message *> &prompts, std::vector<struct pam_response *> &answers)
    {
        if (prompt_ != prompts[0]->msg) {
            throw std::string("prompt does not match");
        }
        response_.resp_retcode = 0;
        response_.resp = const_cast<char *>(answer_.c_str());
        answers.resize(1);
        answers[0] = &response_;
        return 0;
    }

};

class match_prompt_style_conversation : public pam_conversation
{
private:
    pam_response response_;
    std::string answer_;
    int style_;
public:
    match_prompt_style_conversation(int style) : style_(style), answer_("ok:123") {}
    int conv(const std::vector<const struct pam_message *> &prompts, std::vector<struct pam_response *> &answers)
    {
        if (style_ != prompts[0]->msg_style) {
            throw std::string("style does not match");
        }
        response_.resp_retcode = 0;
        response_.resp = const_cast<char *>(answer_.c_str());
        answers.resize(1);
        answers[0] = &response_;
        return 0;
    }

};

class fake_pam : public pam
{
private:
    std::shared_ptr<pam_conversation> conversation_;
public:
    fake_pam(std::shared_ptr<pam_conversation> conversation) : conversation_(conversation) {}
    fake_pam() {}
    int get_conversation(pam_handle_t *pamh, std::shared_ptr<pam_conversation> &conversation)
    {
        if (conversation_) {
            conversation = conversation_;
            return 0;
        }
        return 12;
    }
};


int returns_correct_token()
{
    //given
    pam_handle_t *pamh;
    pam_conversation_p fake_conversation = (pam_conversation_p) new fake_pam_conversation("user:code");
    pam_p pam = (pam_p)new fake_pam(fake_conversation);

    //when
    pam_token_conversation conversation(pamh, pam);

    //then
    check(conversation.token() == "code", "returned incorrect token");
    succeed();
}

int returns_correct_user_name()
{
    //given
    pam_handle_t *pamh;
    pam_conversation_p fake_conversation = (pam_conversation_p) new fake_pam_conversation("sally:token");
    pam_p pam = (pam_p)new fake_pam(fake_conversation);


    //when
    pam_token_conversation conversation(pamh, pam);

    //then
    check(conversation.user_name() == "sally", "returned incorrect user name");
    succeed();
}

int returns_empty_user_and_token_when_no_colon()
{
    //given
    pam_handle_t *pamh;
    pam_conversation_p fake_conversation = (pam_conversation_p) new fake_pam_conversation("sally");
    pam_p pam = (pam_p)new fake_pam(fake_conversation);

    //when
    pam_token_conversation conversation(pamh, pam);

    //then
    check(conversation.user_name() == "", "did not return empty user name");
    check(conversation.token() == "", "did not return empty token");
    succeed();
}

int returns_empty_user_and_token_when_empty_answer()
{
    //given
    pam_handle_t *pamh;
    pam_conversation_p fake_conversation = (pam_conversation_p) new fake_pam_conversation("");
    pam_p pam = (pam_p)new fake_pam(fake_conversation);

    //when
    pam_token_conversation conversation(pamh, pam);

    //then
    check(conversation.user_name() == "", "did not return empty user name");
    check(conversation.token() == "", "did not return empty token");
    succeed();
}

int returns_empty_token_when_colon_end()
{
    //given
    pam_handle_t *pamh;
    pam_conversation_p fake_conversation = (pam_conversation_p) new fake_pam_conversation("sally:");
    pam_p pam = (pam_p)new fake_pam(fake_conversation);

    //when
    pam_token_conversation conversation(pamh, pam);

    //then
    check(conversation.user_name() == "sally", "did not return empty user name");
    check(conversation.token() == "", "did not return empty token");
    succeed();
}

int returns_empty_user_when_colon_begin()
{
    //given
    pam_handle_t *pamh;
    pam_conversation_p fake_conversation = (pam_conversation_p) new fake_pam_conversation(":token");
    pam_p pam = (pam_p)new fake_pam(fake_conversation);

    //when
    pam_token_conversation conversation(pamh, pam);

    //then
    check(conversation.user_name() == "", "did not return empty user name");
    check(conversation.token() == "token", "did not return empty token");
    succeed();
}

int returns_empty_user_and_token_when_pam_cant_create_conversation()
{
    // given
    pam_handle_t *pamh;
    pam_p pam = (pam_p)new fake_pam;

    //when
    pam_token_conversation conversation(pamh, pam);

    //then
    check(conversation.user_name() == "", "did not return empty user name");
    check(conversation.token() == "", "did not return empty token");
    succeed();

}

int prompts_user_with_correct_text()
{
    // given
    pam_handle_t *pamh;
    pam_conversation_p match_conversation = (pam_conversation_p) new match_prompt_text_conversation("Dual control token: ");
    pam_p pam = (pam_p)new fake_pam(match_conversation);


    // when / then
    try {
        pam_token_conversation conversation(pamh, pam);
        succeed();
    } catch (const std::string &x) {
        fail();
    }

}

int prompts_user_with_correct_style()
{
    // given
    pam_handle_t *pamh;
    pam_conversation_p match_conversation = (pam_conversation_p) new match_prompt_style_conversation(PAM_PROMPT_ECHO_OFF);
    pam_p pam = (pam_p)new fake_pam(match_conversation);


    // when / then
    try {
        pam_token_conversation conversation(pamh, pam);
        succeed();
    } catch (const std::string &x) {
        fail();
    }
}

int returns_empty_user_and_token_when_conversation_fails()
{
    //given
    pam_handle_t *pamh;
    pam_conversation_p fake_conversation = (pam_conversation_p) new fake_failing_conversation;
    pam_p pam = (pam_p) new fake_pam(fake_conversation);

    //when
    pam_token_conversation conversation(pamh, pam);

    //then
    check(conversation.user_name() == "", "did not return empty user name");
    check(conversation.token() == "", "did not return empty token");
    succeed();
}

int returns_empty_user_and_token_when_conversation_answer_fails()
{
    //given
    pam_handle_t *pamh;
    pam_conversation_p fake_conversation = (pam_conversation_p) new fake_failing_answer_conversation;
    pam_p pam = (pam_p) new fake_pam(fake_conversation);

    //when
    pam_token_conversation conversation(pamh, pam);

    //then
    check(conversation.user_name() == "", "did not return empty user name");
    check(conversation.token() == "", "did not return empty token");
    succeed();
}

RESET_VARS_START
RESET_VARS_END

int run_tests()
{
    test(returns_correct_token);
    test(returns_correct_user_name);
    test(returns_empty_user_and_token_when_no_colon);
    test(returns_empty_token_when_colon_end);
    test(returns_empty_user_when_colon_begin);
    test(returns_empty_user_and_token_when_empty_answer);
    test(returns_empty_user_and_token_when_pam_cant_create_conversation);
    test(prompts_user_with_correct_text);
    test(prompts_user_with_correct_style);
    test(returns_empty_user_and_token_when_conversation_fails);
    test(returns_empty_user_and_token_when_conversation_answer_fails);
    succeed();
}

int main(int argc, char *args[])
{
    return !run_tests();
}


