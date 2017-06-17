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
#include <utility>

#include "installer.h"
#include "token.h"
#include "sys_unistd.h"
#include "user.h"
#include "generator.h"
#include "base32.h"

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
    std::pair<std::string, std::string> install_key() const override
    {
        auto found_user = directory_.get_current_user ();

        if (found_user.empty()) {
            return {"", ""};
        }

        user user (found_user[0]);

        std::string key_string = tokens_.ensure_key (user);
        std::vector<uint8_t> key = base32().decode (key_string);
        std::string token = generator_.generate_token (key);

        return {key_string, token};
    }
};

}

installer installer::create (const tokens &tokens, const unistd &unistd,
                             const directory &directory, const totp_generator &generator)
{
    return installer (std::make_shared<impl> (tokens, unistd, directory,
                      generator));
}

