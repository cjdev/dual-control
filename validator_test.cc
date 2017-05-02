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

#include <string>
#include <vector>

#include "validator.h"
#include "user.h"
#include "test_util.h"
#include "token.h"

class fake_directory : public directory_ifc
{
private:
    std::string user_name_;
public:
    fake_directory (const std::string &user_name) : user_name_ (user_name)
    {
    }
    fake_directory() : user_name_ ("_NOT_A_USER") {}

    std::vector<user> find_user (const std::string &user_name)
    {
        std::vector<user> result;

        if (user_name == user_name_) {
            result.push_back (user());
        }

        return result;
    }
};

class fake_user_token_supplier : public user_token_supplier_ifc
{
private:
    std::string token_;
public:
    fake_user_token_supplier (const std::string &token) : token_ (token) {}
    fake_user_token_supplier() : token_ ("_NOT_A_TOKEN") {}
    std::string token (user &user)
    {
        return token_;
    }
};

template<class T>
std::shared_ptr<T> share (T *t)
{
    return std::shared_ptr<T> (t);
}

bool validator_validates()
{

    // given
    std::string token = "token";
    user_token_supplier user_token_supplier (share (new
            fake_user_token_supplier (token)));
    std::string user_name = "msmith";
    directory directory (share (new fake_directory (user_name)));
    validator validator = validator::create (directory, user_token_supplier);

    // when
    bool actual = validator.validate ("requester", user_name, token);

    // then
    check (actual, "should be valid");
    succeed();
}

bool validator_fails_unknown_user()
{

    // given
    std::string token = "token";
    user_token_supplier user_token_supplier (share (new
            fake_user_token_supplier));
    directory directory (share (new fake_directory));
    validator validator = validator::create (directory, user_token_supplier);

    // when
    bool actual = validator.validate ("requester", "notuser", token);

    // then
    check (!actual, "should not be valid");
    succeed();
}

bool validator_fails_incorrect_token()
{

    // given
    user_token_supplier user_token_supplier (share (new
            fake_user_token_supplier));
    std::string user_name = "msmith";
    directory directory (share (new fake_directory (user_name)));
    validator validator = validator::create (directory, user_token_supplier);

    // when
    bool actual = validator.validate ("requester", user_name, "token");

    // then
    check (!actual, "should not be valid");
    succeed();
}

bool validator_fails_with_own_token()
{
    // given
    std::string requester_user_name ("requester");
    std::string authorizer_user_name (requester_user_name);
    std::string authorizer_token ("token");
    directory directory (share (new fake_directory (authorizer_user_name)));
    user_token_supplier user_token_supplier (share (new
            fake_user_token_supplier (authorizer_token)));
    validator validator = validator::create (directory, user_token_supplier);

    // when
    bool actual = validator.validate (requester_user_name, authorizer_user_name,
                                      authorizer_token);

    // then
    check (!actual, "should not be valid");
    succeed();

}

bool validator_fails_with_unknown_requester()
{
    // given
    std::string requester_user_name ("");
    std::string authorizer_user_name ("authorizer");
    std::string authorizer_token ("token");
    directory directory (share (new fake_directory (authorizer_user_name)));
    user_token_supplier user_token_supplier (share (new
            fake_user_token_supplier (authorizer_token)));
    validator validator = validator::create (directory, user_token_supplier);

    // when
    bool actual = validator.validate (requester_user_name, authorizer_user_name,
                                      authorizer_token);

    // then
    check (!actual, "should not be valid");
    succeed();

}

RESET_VARS_START
RESET_VARS_END

bool run_tests()
{
    test (validator_validates);
    test (validator_fails_unknown_user);
    test (validator_fails_incorrect_token);
    test (validator_fails_with_own_token);
    test (validator_fails_with_unknown_requester);
    succeed();
}

int main (int argc, char *argv[])
{
    return !run_tests();
}

