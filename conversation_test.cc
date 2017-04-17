#include <vector>
#include <algorithm>
#include <string>
#include <security/pam_modules.h>

#include "conversation.h"
#include "test_util.h"
#include "pam.h"


class fake_pam_conversation : public pam_conversation {
    private:
        pam_response response_;
        std::string answer_;
    public:
        fake_pam_conversation(const std::string &answer) : answer_(answer) {}
        int conv(const std::vector<const struct pam_message *> &prompts, std::vector<struct pam_response *> &answers) {
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

class fake_pam : public pam {
    private:
        std::string answer_;
    public:
        fake_pam(const std::string &answer) : answer_(answer) {}
        int get_conversation(pam_handle_t *pamh, std::shared_ptr<pam_conversation> &conversation) {
           conversation.reset(new fake_pam_conversation(answer_));
           return 0;
        }
};

int returns_correct_token() {
    //given
    pam_handle_t *pamh;
    pam_p pam = (pam_p)new fake_pam("user:code");

    //when
    pam_token_conversation conversation(pamh, pam);

    //then
    check(conversation.token() == "code", "returned incorrect token");
    succeed();
}

int returns_correct_user_name() {
    //given
    pam_handle_t *pamh;
    pam_p pam = (pam_p)new fake_pam("sally:token");

    //when
    pam_token_conversation conversation(pamh, pam);

    //then
    check(conversation.user_name() == "sally", "returned incorrect user name");
    succeed();
}

int returns_empty_user_and_token_when_no_colon() {
    //given
    pam_handle_t *pamh;
    pam_p pam = (pam_p)new fake_pam("sally");

    //when
    pam_token_conversation conversation(pamh, pam);

    //then
    check(conversation.user_name() == "", "did not return empty user name");
    check(conversation.token() == "", "did not return empty token");
    succeed();
}

int returns_empty_user_and_token_when_empty_answer() {
    //given
    pam_handle_t *pamh;
    pam_p pam = (pam_p)new fake_pam("");

    //when
    pam_token_conversation conversation(pamh, pam);

    //then
    check(conversation.user_name() == "", "did not return empty user name");
    check(conversation.token() == "", "did not return empty token");
    succeed();
}

int returns_empty_token_when_colon_end() {
    //given
    pam_handle_t *pamh;
    pam_p pam = (pam_p)new fake_pam("sally:");

    //when
    pam_token_conversation conversation(pamh, pam);

    //then
    check(conversation.user_name() == "sally", "did not return empty user name");
    check(conversation.token() == "", "did not return empty token");
    succeed();
}

int returns_empty_user_when_colon_begin() {
    //given
    pam_handle_t *pamh;
    pam_p pam = (pam_p)new fake_pam(":token");

    //when
    pam_token_conversation conversation(pamh, pam);

    //then
    check(conversation.user_name() == "", "did not return empty user name");
    check(conversation.token() == "token", "did not return empty token");
    succeed();
}


RESET_VARS_START
RESET_VARS_END

int run_tests() {
    test(returns_correct_token);
    test(returns_correct_user_name);
    test(returns_empty_user_and_token_when_no_colon);
    test(returns_empty_token_when_colon_end);
    test(returns_empty_user_when_colon_begin);
    test(returns_empty_user_and_token_when_empty_answer);
    succeed();
}

int main(int argc, char *args[]) {
    return !run_tests();
}


