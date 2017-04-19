#include <security/pam_modules.h>
#include <string>

#include "request.h"
#include "dual_control.h"
#include "validator.h"
#include "conversation.h"
#include "logger.h"
#include "test_util.h"

template<class T>
std::shared_ptr<T> share (T *t)
{
    return std::shared_ptr<T> (t);
}

void use_validator (dual_control_configuration &config,
                    validator_ifc *value)
{
    config.validator = validator (share (value));
}

void use_conversation (dual_control_configuration &config,
                       conversation_ifc *value)
{
    config.conversation = conversation (share (value));
}

void use_logger (dual_control_configuration &config, logger_ifc *value)
{
    config.logger = logger (share (value));
}

class mock_logger : public logger_ifc
{
private:
    int result_;
    std::string user_name_;
    std::string token_;
public:
    void log (int result, const std::string &user_name,
              const std::string &token)
    {
        result_ = result;
        user_name_ = user_name;
        token_ = token;
    }
    int logged_result()
    {
        return result_;
    }
    std::string logged_user_name()
    {
        return user_name_;
    }
    std::string logged_token()
    {
        return token_;
    }
};

class fake_conversation : public conversation_ifc
{
private:
    std::string user_name_;
    std::string token_;
public:
    fake_conversation (const std::string &user_name,
                       const std::string &token) : user_name_ (user_name), token_ (token) {}
    conversation_result initiate (const pam_request &request)
    {
        return {user_name_, token_};
    }
};

class fake_validator : public validator_ifc
{
private:
    std::string user_;
    std::string token_;
public:
    fake_validator (const std::string &user,
                    const std::string &token): user_ (user), token_ (token) {}
    bool validate (const std::string &user, const std::string &token)
    {
        return user_ == user && token_ == token;
    }
};

pam_request req()
{
    return pam_request (0, 0, 0, 0);
}

int setcred_returns_success()
{
    //given
    dual_control_configuration configuration;
    dual_control dc (create_dual_control (configuration));

    //when
    int result = dc.setcred (req());

    //then
    checkint (PAM_SUCCESS, result, "function return");
    succeed();

}

int authenticate_validates_with_received_token()
{
    // given
    dual_control_configuration configuration;
    std::string user ("user");
    std::string token ("token");
    use_validator (configuration, new fake_validator (user, token));
    use_conversation (configuration, new fake_conversation (user, token));
    dual_control dc (create_dual_control (configuration));
    pam_handle_t *handle (0);
    std::vector<const std::string> arguments;

    // when
    int actual = dc.authenticate (req());

    // then
    check (actual == PAM_SUCCESS, "should be success");
    succeed();
}

int authenticate_fails_with_wrong_user()
{
    // given
    dual_control_configuration configuration;
    std::string token ("token");
    use_validator (configuration, new fake_validator ("user", token));
    use_conversation (configuration, new fake_conversation ("wrong user",
                      token));
    dual_control dc (create_dual_control (configuration));

    // when
    int actual = dc.authenticate (req());

    // then
    check (actual == PAM_AUTH_ERR, "should be auth err");
    succeed();
}

int authenticate_fails_with_wrong_token()
{
    // given
    dual_control_configuration configuration;
    std::string user ("user");
    use_validator (configuration, new fake_validator (user, "token"));
    use_conversation (configuration, new fake_conversation (user,
                      "wrong token"));
    dual_control dc (create_dual_control (configuration));

    // when
    int actual = dc.authenticate (req());

    // then
    check (actual == PAM_AUTH_ERR, "should be auth err");
    succeed();
}

int logs_authentication()
{
    //given
    dual_control_configuration configuration;
    std::string user ("user");
    std::string token ("token");
    use_validator (configuration, new fake_validator (user, token));
    use_conversation (configuration, new fake_conversation (user, token));
    mock_logger *test_logger;
    use_logger (configuration, test_logger = new mock_logger);
    dual_control dc (create_dual_control (configuration));

    //when
    dc.authenticate (req());

    //then
    check (test_logger->logged_result() == PAM_SUCCESS,
           "logged result should be success");
    check (test_logger->logged_user_name() == user,
           "logged user name should be user");
    check (test_logger->logged_token() == token,
           "logged token should be token");
    succeed();
}

int logs_authentication_failure()
{
    //given
    dual_control_configuration configuration;
    std::string user ("user");
    std::string token ("token");
    use_validator (configuration, new fake_validator (user,
                   "not the received token"));
    use_conversation (configuration, new fake_conversation (user, token));
    mock_logger *test_logger;
    use_logger (configuration, test_logger = new mock_logger);
    dual_control dc (create_dual_control (configuration));

    //when
    dc.authenticate (req());

    //then
    check (test_logger->logged_result() == PAM_AUTH_ERR,
           "logged result should be success");
    check (test_logger->logged_user_name() == user,
           "logged user name should be user");
    check (test_logger->logged_token() == token,
           "logged token should be token");
    succeed();
}

RESET_VARS_START
RESET_VARS_END

int runtests()
{
    test (setcred_returns_success);
    test (authenticate_validates_with_received_token);
    test (authenticate_fails_with_wrong_user);
    test (authenticate_fails_with_wrong_token);
    test (logs_authentication);
    test (logs_authentication_failure);
    succeed();
}

int main (int argc, char *argv[])
{
    return !runtests();
}

