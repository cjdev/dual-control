#include "validator.h"
#include "user.h"
#include "test_util.h"

class fake_directory : public directory {
    private:
        std::string user_name_;
    public:
        fake_directory(const std::string &user_name) : user_name_(user_name) {}
        fake_directory() : user_name_("_NOT_A_USER") {}

        virtual const user_p find_user(const std::string &user_name) {
            user_p result;
            if (user_name == user_name_) {
                result.reset(new user);
            }

            return result;
        }
};


bool validator_validates() {

   // given
    std::string user_name = "msmith";
    directory_p directory(new fake_directory(user_name));
    validator validator(directory);


    // when
    bool actual = validator.validate(user_name, "token");


    // then
    check(actual, "should be valid");
    succeed();
}

bool validator_fails_unknown_user() {

   // given
   directory_p directory(new fake_directory);
   validator validator(directory);


   // when
   bool actual = validator.validate("notuser", "token");


   // then
   check(!actual, "should not be valid");
   succeed();
}

bool validator_fails_incorrect_token() {

   // given
    std::string user_name = "user";
    directory_p directory(new fake_directory(user_name));
    validator validator(directory);


   // when
   bool actual = validator.validate(user_name, "nottoken");


   // then
   check(!actual, "should not be valid");
   succeed();
}

RESET_VARS_START
RESET_VARS_END

bool run_tests() {
    test(validator_validates);
    test(validator_fails_unknown_user);
    test(validator_fails_incorrect_token);
    succeed();

}

int main(int argc, char *argv[]) {
    return !run_tests();
}

