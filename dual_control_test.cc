#include <security/pam_modules.h>

#include "dual_control.h"
#include "validator.h"

#include "test_util.h"
#include "conversation.h"


class fake_conversations : public conversations_ifc {
    private:
        std::string user_name_;
        std::string token_;
    public:
        fake_conversations(const std::string &user_name, const std::string &token) : user_name_(user_name), token_(token) {}
        conversation_result initiate_conversation() {
            return conversation_result(user_name_, token_);
        }
};

class fake_validator : public validator_ifc {
    private:
        std::string user_;
        std::string token_;
    public:
        fake_validator(const std::string &user, const std::string &token): user_(user), token_(token) {}
        bool validate(const std::string &user, const std::string &token) {
            return user_ == user && token_ == token;
        }
};


int setcred_returns_success() {
    //given
    dual_control_configuration configuration;
    pam_handle *pamh(0);
    dual_control dc(create_dual_control(configuration));
    std::vector<const std::string> arguments;

    //when
    int result = dc->setcred(pamh, 0, arguments);

    //then
    checkint(PAM_SUCCESS, result, "function return");
    succeed();

}

int authenticate_validates_with_received_token() {
    // given
    dual_control_configuration configuration;
    std::string user("user");
    std::string token("token");
    configuration.validator = validator(new fake_validator(user, token));
    configuration.conversations = conversations(new fake_conversations(user, token));
    dual_control dc(create_dual_control(configuration));
    pam_handle_t *handle = (pam_handle_t*)"";
    std::vector<const std::string> arguments;

    // when
    int actual = dc->authenticate(handle, 0, arguments);

    // then
    check(actual == PAM_SUCCESS, "should be valid");
    succeed();
}


RESET_VARS_START
RESET_VARS_END

int runtests() {
    test(setcred_returns_success);
    test(authenticate_validates_with_received_token);
    succeed();
}

int main(int argc, char* argv[]) {
   return !runtests();
}



// DELETE BELOW HERE

/*
const char *validated_user = "";
const char *validated_token = "";
const char *token_to_return = "";
int validation_to_return = 0;
int log_success_invoked = 0;
int log_failure_invoked = 0;
int at_least_one_failed_test = 0;
pam_handle_t *passed_pam_handle = NULL;

RESET_VARS_START
validated_user = "";
validated_token = "";
validation_to_return = 1;
passed_pam_handle = NULL;
log_success_invoked = 0;
log_failure_invoked = 0;
RESET_VARS_END

const char *ask_for_token(pam_handle_t *pamh) {
    passed_pam_handle = pamh;
    return token_to_return;
}


int validate_token(const char *user, const char *token) {
    validated_user = user;
    validated_token = token;
    return validation_to_return;
}

void log_success() {
    log_success_invoked = 1;
}

void log_failure() {
    log_failure_invoked = 1;
}

int pam_sm_authenticate_validates_with_received_token() {
    // given
    token_to_return = "user:pin";
    pam_handle_t *handle = (pam_handle_t*)"";

    // when
    pam_sm_authenticate(handle, 0, 0, NULL);

    // then
    checkstr("pin",validated_token, "validated token");
    checkstr("user",validated_user, "validated user");
    check(passed_pam_handle == handle, "incorrect handle");
    succeed();
}

int pam_sm_authenticate_success_invokes_log_success() {
    // given
    validation_to_return = 1;

    //when
   pam_sm_authenticate(NULL, 0, 0, NULL);
   return log_success_invoked;
}

int pam_sm_authenticate_fail_invokes_log_failure() {
    // given
    validation_to_return = 0;

    //when
   pam_sm_authenticate(NULL, 0, 0, NULL);
   check(log_failure_invoked, "log failure should be invoked");
   succeed();
}

int succeeds_with_valid_token() {
    //given
    validation_to_return = 1;

    //when
    int result = pam_sm_authenticate(NULL, 0, 0, NULL);

    //then
    return result == PAM_SUCCESS;
}

int fails_with_invalid_token() {
    //given
    validation_to_return = 0;

    //when
    int result = pam_sm_authenticate(NULL, 0, 0, NULL);

    //then
    return result == PAM_AUTH_ERR;
}
*/


