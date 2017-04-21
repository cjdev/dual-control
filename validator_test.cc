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

class fake_user_token_supplier : public user_token_supplier
{
private:
    std::string token_;
public:
    fake_user_token_supplier (const std::string &token) : token_ (token) {}
    fake_user_token_supplier() : token_ ("_NOT_A_TOKEN") {}
    virtual std::string token (const user &user)
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
    validator validator = create_validator (directory, user_token_supplier);

    // when
    bool actual = validator.validate (user_name, token);

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
    validator validator = create_validator (directory, user_token_supplier);

    // when
    bool actual = validator.validate ("notuser", token);

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
    validator validator = create_validator (directory, user_token_supplier);

    // when
    bool actual = validator.validate (user_name, "token");

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
    succeed();
}

int main (int argc, char *argv[])
{
    return !run_tests();
}

