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
#include <vector>
#include <security/pam_modules.h>
#include <security/pam_appl.h>

#include "sys_pam.h"

namespace
{
class impl : public pam_ifc
{
public:
    int get_conv (pam_handle *handle, const pam_conv **out) const override
    {
        return ::pam_get_item (handle, PAM_CONV, (const void **)out);
    }
    int get_user (pam_handle *handle, const char **out) const override
    {
        return ::pam_get_item (handle, PAM_USER, (const void **)out);
    }
};
}

pam pam::create()
{
    static pam singleton (std::shared_ptr<pam_ifc> (new impl));
    return singleton;
}

