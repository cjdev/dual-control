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

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "installer.h"
#include "user.h"
#include "token.h"
#include "test_util.h"
#include "generator.h"

class mock_tokens : public tokens_ifc
{
public:
    mutable std::string captured_token;
    void save (const user &user, const std::string &token) const override
    {
        captured_token = token;
    }
};

class fake_unistd : public unistd_ifc
{
private:
    std::string user_name_;
public:
    fake_unistd (const std::string &user_name) : user_name_ (user_name) {}
    const char *getlogin() const override
    {
        return user_name_.c_str();
    }
};

class fail_unistd : public unistd_ifc
{
public:
    const char *getlogin() const override
    {
        return nullptr;
    }
};

class fake_totp_generator : public token_generator_ifc
{
private:
    std::string expected_token;
public:
    fake_totp_generator (const std::string expected_token = "000000"):
        expected_token (expected_token)
    {}

    std::string generate_token() const override
    {
        return expected_token;
    }
};

class fake_directory : public directory_ifc
{
private:
    std::string expected_user_name_;
public:
    fake_directory (const std::string &expected_user_name) :
        expected_user_name_ (expected_user_name)
    {
    }
    std::vector<user> find_user (const std::string &user_name) const override
    {
        std::vector<user> rval;

        if (user_name == expected_user_name_) {
            rval.push_back (user());
        }

        return rval;
    }

};

int installs_token()
{
    //given
    std::string user_name ("user");
    std::string token ("token");
    auto  test_tokens = std::make_shared<mock_tokens>();
    tokens tokens{test_tokens};
    unistd unistd (std::make_shared<fake_unistd> (user_name));
    directory directory (std::make_shared<fake_directory> (user_name));
    std::shared_ptr<fake_totp_generator> fake_generator =
        std::make_shared<fake_totp_generator> (token);
    totp_generator generator (fake_generator);

    installer installer = installer::create (tokens, unistd, directory,
                          generator);

    //when
    std::string result = installer.install_token();

    //then
    check (test_tokens->captured_token == token, "installed wrong token");
    check (result == token, "installer returned wrong token");
    succeed();
}

int unistd_does_not_find_user_name_nullptr_case()
{
    //given
    std::string user_name ("user");
    std::string token ("token");
    auto  test_tokens = std::make_shared<mock_tokens>();
    tokens tokens{test_tokens};
    unistd unistd (std::make_shared<fail_unistd>());
    directory directory (std::make_shared<fake_directory> (user_name));
    totp_generator generator (std::make_shared<fake_totp_generator>());

    installer installer = installer::create (tokens, unistd, directory,
                          generator);

    //when
    auto returned = installer.install_token();

    //then
    check (test_tokens->captured_token == "",
           "should not have installed a token");
    check (returned == "", "did not return empty token");
    succeed();
}

int unistd_does_not_find_user_name_empty_string_case()
{
    //given
    std::string user_name ("user");
    std::string token ("token");
    auto  test_tokens = std::make_shared<mock_tokens>();
    tokens tokens{test_tokens};
    unistd unistd (std::make_shared<fake_unistd> (""));
    directory directory (std::make_shared<fake_directory> (user_name));
    totp_generator generator (totp_generator (std::make_shared<fake_totp_generator>()));

    installer installer = installer::create (tokens, unistd, directory,
                          generator);

    //when
    auto returned = installer.install_token();

    //then
    check (test_tokens->captured_token == "",
           "should not have installed a token");
    check (returned == "", "did not return empty token");
    succeed();
}

int directory_finds_no_user_info()
{
    std::string user_name ("user");
    std::string token ("token");
    auto  test_tokens = std::make_shared<mock_tokens>();
    tokens tokens{test_tokens};
    unistd unistd (std::make_shared<fake_unistd> (user_name));
    directory directory (std::make_shared<fake_directory> ("not the user"));
    totp_generator generator (std::make_shared<fake_totp_generator>());

    installer installer = installer::create (tokens, unistd, directory,
                          generator);

    //when
    auto returned = installer.install_token();

    //then
    check (test_tokens->captured_token == "", "installed wrong token");
    check (returned == "", "did not return empty token");
    succeed();
}

int run_tests()
{
    test (installs_token);
    test (unistd_does_not_find_user_name_nullptr_case);
    test (unistd_does_not_find_user_name_empty_string_case);
    test (directory_finds_no_user_info);
    succeed();
}

int main (int argc, char *argv[])
{
    return !run_tests();
}
