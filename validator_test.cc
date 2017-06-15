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
#include "sys_unistd.h"

template<class T>
std::shared_ptr<T> share (T *t)
{
    return std::shared_ptr<T> (t);
}

class mock_unistd : public unistd_ifc
{
private:
    uid_t expected_euid_;
public:
    mutable std::vector<uid_t> actual_uids;
    mock_unistd (uid_t expected_euid)
        : expected_euid_ (expected_euid)
    {}

    int seteuid (uid_t euid) const override
    {
        actual_uids.push_back(euid);
        return (euid == expected_euid_ || euid == 0) ? 0 : -1;
    }
};

class fake_user : public user_ifc
{
private:
    uid_t uid_;
public:
    fake_user (uid_t uid) : uid_ (uid)
    {}
    uid_t uid() const override
    {
        return uid_;
    }
};

class fake_directory : public directory_ifc
{
private:
    std::string user_name_;
    uid_t uid_;
public:
    fake_directory (const std::string &user_name, const uid_t uid = -1) : user_name_ (user_name), uid_ (uid)
    {
    }
    fake_directory() : user_name_ ("_NOT_A_USER") {}

    std::vector<user> find_user (const std::string &user_name) const override
    {
        std::vector<user> result;

        if (user_name == user_name_) {
            result.push_back (user(share (new fake_user (uid_))));
        }

        return result;
    }
};

class fake_tokens : public tokens_ifc
{
private:
    std::string token_;
public:
    fake_tokens (const std::string &token) : token_ (token) {}
    fake_tokens() : token_ ("_NOT_A_TOKEN") {}
    std::string token (const user &user) const override
    {
        return token_;
    }
};

bool validator_validates()
{

    // given
    std::string token = "token";
    tokens tokens (share (new
                          fake_tokens (token)));
    std::string user_name = "msmith";

    uid_t expected_uid = 1000;
    directory directory (share (new fake_directory (user_name, expected_uid)));
    std::shared_ptr<mock_unistd> mock_unistd = share (new class mock_unistd(expected_uid));
    class unistd unistd (mock_unistd);
    validator validator = validator::create (directory, tokens, unistd);
    std::vector<uid_t> expected_uids {expected_uid, 0};

    // when
    bool actual = validator.validate ("requester", user_name, token, "reason");

    // then
    check (actual, "should be valid");
    check (mock_unistd->actual_uids == expected_uids, "should drop permissions");
    succeed();
}

bool validator_fails_unknown_user()
{

    // given
    std::string token = "token";
    tokens tokens (share (new
                          fake_tokens));
    directory directory (share (new fake_directory));
    class unistd unistd (share (new unistd_ifc()));
    validator validator = validator::create (directory, tokens, unistd);

    // when
    bool actual = validator.validate ("requester", "notuser", token, "reason");

    // then
    check (!actual, "should not be valid");
    succeed();
}

bool validator_fails_incorrect_token()
{

    // given
    tokens tokens (share (new
                          fake_tokens));
    std::string user_name = "msmith";
    directory directory (share (new fake_directory (user_name)));
    class unistd unistd (share (new unistd_ifc()));
    validator validator = validator::create (directory, tokens, unistd);

    // when
    bool actual = validator.validate ("requester", user_name, "token",
                                      "reason");

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
    tokens tokens (share (new
                          fake_tokens (authorizer_token)));
    class unistd unistd (share (new unistd_ifc()));
    validator validator = validator::create (directory, tokens, unistd);

    // when
    bool actual = validator.validate (requester_user_name, authorizer_user_name,
                                      authorizer_token, "reason");

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
    tokens tokens (share (new
                          fake_tokens (authorizer_token)));
    class unistd unistd (share (new unistd_ifc()));
    validator validator = validator::create (directory, tokens, unistd);

    // when
    bool actual = validator.validate (requester_user_name, authorizer_user_name,
                                      authorizer_token, "reason");

    // then
    check (!actual, "should not be valid");
    succeed();
}

bool validator_fails_on_empty_reason()
{
    //given
    std::string requester_user_name ("");
    std::string authorizer_user_name ("authorizer");
    std::string authorizer_token ("token");
    std::string reason;
    directory directory (share (new fake_directory (authorizer_user_name)));
    tokens tokens (share (new
                          fake_tokens (authorizer_token)));
    class unistd unistd (share (new unistd_ifc()));
    validator validator = validator::create (directory, tokens, unistd);

    //when
    bool actual = validator.validate (requester_user_name, authorizer_user_name,
                                      authorizer_token, reason);

    //then
    check (!actual, "should not be valid");
    succeed();
}

bool run_tests()
{
    test (validator_validates);
    test (validator_fails_unknown_user);
    test (validator_fails_incorrect_token);
    test (validator_fails_with_own_token);
    test (validator_fails_with_unknown_requester);
    test (validator_fails_on_empty_reason);
    succeed();
}

int main (int argc, char *argv[])
{
    return !run_tests();
}
