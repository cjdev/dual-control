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

namespace
{
class impl : public validator_ifc
{
private:
    directory directory_;
    user_token_supplier user_token_supplier_;
public:
    impl (const directory &directory,
          const user_token_supplier user_token_supplier) :
        directory_ (directory),
        user_token_supplier_ (user_token_supplier) {}
    bool validate (const std::string &user_name,
                   const std::string &token)
    {
        std::vector<user> found_user = directory_.find_user (user_name);

        if (found_user.empty()) {
            return false;
        }

        std::string user_token = user_token_supplier_.token (found_user[0]);
        return user_token == token;
    }
};
}

validator validator::create (const directory &directory,
                             const user_token_supplier &user_token_supplier)
{
    std::shared_ptr<validator_ifc> delegate (new impl (directory,
            user_token_supplier));
    return validator (delegate);
}

