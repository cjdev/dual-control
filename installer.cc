#include <string>
#include <vector>

#include "installer.h"
#include "token.h"
#include "sys_unistd.h"
#include "user.h"

namespace {

class impl : public installer_ifc {
    private:
        tokens tokens_;
        unistd unistd_;
        directory directory_;
        generator generator_;
    public:
        impl(const tokens &tokens, const unistd &unistd,
            const directory &directory, const installer_ifc::generator &generator) :
            tokens_(tokens), unistd_(unistd), directory_(directory), generator_(generator) {}
        std::string install_token() const override {
            const char *c_user_name = unistd_.getlogin();
            if (c_user_name == nullptr) {
                return "";
            }

            std::string user_name = c_user_name;

            auto found_user = directory_.find_user(user_name);
            if (found_user.empty()) {
                return "";
            }

            user user(found_user[0]);
            std::string token(generator_());
            tokens_.save(user, token);
            return "123456";
        }
};

}


installer installer::create(const tokens &tokens, const unistd &unistd,
        const directory &directory, const installer_ifc::generator &generator) {
    return installer(std::make_shared<impl>(tokens, unistd, directory, generator));
}

