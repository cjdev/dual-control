#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "installer.h"
#include "user.h"
#include "token.h"
#include "test_util.h"

class mock_tokens : public tokens_ifc {
    public:
        mutable std::string captured_token;
        void save (const user &user, const std::string &token) const override {
            captured_token = token;
        }
};

class fake_unistd : public unistd_ifc {
    private:
        std::string user_name_;
    public:
        fake_unistd(const std::string &user_name) : user_name_(user_name) {}
        const char *getlogin() const override {
            return user_name_.c_str();
        }
};

class fake_directory : public directory_ifc {
    private:
        std::string expected_user_name_;
    public:
        fake_directory(const std::string &expected_user_name) : expected_user_name_(expected_user_name) {
        }
        std::vector<user> find_user (const std::string &user_name) const override {
            std::vector<user> rval;
            if (user_name == expected_user_name_) {
                rval.push_back(user());
            }
            return rval;
        }

};

int installs_token() {
    //given
    user user;
    std::string user_name("user");
    std::string token("token");
    auto  test_tokens = std::make_shared<mock_tokens>();
    tokens tokens{test_tokens};
    unistd unistd(std::make_shared<fake_unistd>(user_name));
    directory directory(std::make_shared<fake_directory>(user_name));
    installer_ifc::generator generator = [&] { return token; };

    installer installer = installer::create (tokens, unistd, directory, generator);

    //when
    installer.install_token();

    //then
    check(test_tokens->captured_token == token, "installed wrong token");
    succeed();
}



int run_tests() {
    test(installs_token);
    succeed();
}

int main(int argc, char *argv[]) {
    return !run_tests();
}

