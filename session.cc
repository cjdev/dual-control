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

#include <memory>
#include <string>

#include "session.h"
#include "sys_pam.h"
#include "request.h"

namespace
{
class impl : public sessions_ifc
{
private:
    pam pam_;
public:
    impl (const pam &pam) : pam_ (pam) {}
    std::string user_name (const pam_request &request)  const
    {
        const char *user_name;
        pam_.get_user (request.handle(), &user_name);
        return user_name;
    }
};

}

sessions sessions::create (const pam &pam)
{
    return sessions (std::make_shared<impl> (pam));
}

