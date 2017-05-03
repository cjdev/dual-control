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
#include <string>

#include "request.h"
#include "dual_control.h"
#include "validator.h"
#include "conversation.h"
#include "logger.h"
#include "test_util.h"
#include "session.h"

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
    config.logger = logger (logger::delegate (value));
}

void use_sessions (dual_control_configuration &config, sessions_ifc *value)
{
    config.sessions = sessions (sessions::delegate (value));
}

class fake_sessions : public sessions_ifc {
    private:
        std::string user_;
    public:
        fake_sessions(const std::string &user) : user_(user) {}
    std::string user_name (const pam_request &request)  const {
        return user_;
    }

};

class mock_logger : public logger_ifc
{
private:
    int result_;
    std::string requester_user_name_;
    std::string authorizer_user_name_;
    std::string token_;
public:
    void log (int result, const std::string &requester_user_name, const std::string &authorizer_user_name,
              const std::string &token)
    {
        result_ = result;
        requester_user_name_ = requester_user_name;
        authorizer_user_name_ = authorizer_user_name;
        token_ = token;
    }
    int logged_result()
    {
        return result_;
    }
    std::string logged_requester()
    {
        return requester_user_name_;
    }
    std::string logged_authorizer()
    {
        return authorizer_user_name_;
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
    std::string requester_;
    std::string authorizer_;
    std::string token_;
public:
    fake_validator (const std::string &requester, const std::string &authorizer,
                    const std::string &token): requester_(requester), authorizer_(authorizer), token_ (token) {}
    bool validate (const std::string &requester, const std::string &authorizer,
                   const std::string &token)
    {
        return requester_ == requester && authorizer_ == authorizer && token_ == token;
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
    dual_control dc (dual_control::create (configuration));

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
    std::string requester("requester");
    std::string authorizer("authorizer");
    std::string token("token");
    use_validator (configuration, new fake_validator (requester, authorizer, token));
    use_conversation (configuration, new fake_conversation (authorizer, token));
    use_sessions(configuration, new fake_sessions(requester));
    dual_control dc (dual_control::create (configuration));
    pam_handle_t *handle (0);
    std::vector<std::string> arguments;

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
    use_validator (configuration, new fake_validator ("requester", "user", token));
    use_conversation (configuration, new fake_conversation ("wrong user",
                      token));
    dual_control dc (dual_control::create (configuration));

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
    std::string requester("requester");
    std::string authorizer("authorizer");
    use_validator (configuration, new fake_validator (requester, authorizer, "token"));
    use_conversation (configuration, new fake_conversation (authorizer,
                      "wrong token"));
    dual_control dc (dual_control::create (configuration));

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
    std::string requester("requester");
    std::string authorizer("authorizer");
    std::string token ("token");
    use_validator (configuration, new fake_validator (requester, authorizer, token));
    use_conversation (configuration, new fake_conversation (authorizer, token));
    use_sessions(configuration, new fake_sessions(requester));
    mock_logger *test_logger;
    use_logger (configuration, test_logger = new mock_logger);
    dual_control dc (dual_control::create (configuration));

    //when
    dc.authenticate (req());

    //then
    check (test_logger->logged_result() == PAM_SUCCESS,
           "logged result should be success");
    check (test_logger->logged_requester() == requester,
           "logged user name should be user");
    check (test_logger->logged_authorizer() == authorizer,
           "logged user name should be user");
    check (test_logger->logged_token() == token,
           "logged token should be token");
    succeed();
}

int logs_authentication_failure()
{
    //given
    dual_control_configuration configuration;
    std::string requester("requester");
    std::string authorizer("authorizer");
    std::string token("token");
    use_validator (configuration, new fake_validator (requester, authorizer,
                   "not the received token"));
    use_conversation (configuration, new fake_conversation (authorizer, token));
    use_sessions(configuration, new fake_sessions(requester));
    mock_logger *test_logger;
    use_logger (configuration, test_logger = new mock_logger);
    dual_control dc (dual_control::create (configuration));

    //when
    dc.authenticate (req());

    //then
    check (test_logger->logged_result() == PAM_AUTH_ERR,
           "logged result should be success");
    check (test_logger->logged_requester() == requester,
           "logged user name should be user");
    check (test_logger->logged_authorizer() == authorizer,
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

