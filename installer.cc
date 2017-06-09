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

#include "installer.h"
#include "token.h"
#include "sys_unistd.h"
#include "user.h"
#include "generator.h"

namespace
{

class impl : public installer_ifc
{
private:
    tokens tokens_;
    unistd unistd_;
    directory directory_;
    totp_generator generator_;
public:
    impl (const tokens &tokens, const unistd &unistd,
          const directory &directory,
          const totp_generator generator) :
        tokens_ (tokens), unistd_ (unistd), directory_ (directory),
        generator_ (generator) {}
    std::string install_token() const override
    {
        const char *c_user_name = unistd_.getlogin();

        if (c_user_name == nullptr) {
            return "";
        }

        std::string user_name = c_user_name;

        auto found_user = directory_.find_user (user_name);

        if (found_user.empty()) {
            return "";
        }

        user user (found_user[0]);
        std::string token (generator_.generate_token("\x00"));
        tokens_.save (user, token);
        return token;
    }
};

}

installer installer::create (const tokens &tokens, const unistd &unistd,
                             const directory &directory, const totp_generator &generator)
{
    return installer (std::make_shared<impl> (tokens, unistd, directory, generator));
}
