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
#include "become.h"
#include "sys_unistd.h"

namespace
{
class impl : public validator_ifc
{
private:
    directory directory_;
    tokens tokens_;
    unistd unistd_;
public:
    impl (const directory &directory,
          const tokens tokens,
          const unistd unistd) :
        directory_ (directory),
        tokens_ (tokens),
        unistd_ (unistd)
    {}
    bool validate (const std::string &requester_user_name,
                   const std::string &authorizer_user_name,
                   const std::string &token,
                   const std::string &reason) override
    {
        std::vector<user> found_user = directory_.find_user (authorizer_user_name);

        if (reason.empty()) {
            return false;
        }

        if (requester_user_name.empty()) {
            return false;
        }

        if (requester_user_name == authorizer_user_name) {
            return false;
        }

        if (found_user.empty()) {
            return false;
        }

        auto user_ = found_user[0];
        become become_(user_, unistd_);

        std::string user_token = tokens_.token (user_);
        return user_token == token;
    }
};
}

validator validator::create (const directory &directory,
                             const tokens &tokens,
                             const unistd &unistd)
{
    std::shared_ptr<validator_ifc> delegate (new impl (directory, tokens, unistd));
    return validator (delegate);
}
