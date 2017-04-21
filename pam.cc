/* Copyright (C) CJ Affiliate
 *
 * You may use, distribute and modify this code under  the
 * terms of the GNU Public License.
 *
 * You should have received a copy of the license with this
 * file. If not, you will find a copy in the "LICENSE" file
 * at https://github.com/cjdev/dual-control.
 */

#include <vector>
#include <security/pam_modules.h>
#include <security/pam_appl.h>

#include "pam.h"

class syspam : public pam_ifc
{
public:
    int get_conv (pam_handle *handle, const pam_conv **pout);
};

int syspam::get_conv (pam_handle *handle,
                      std::shared_ptr<pam_conv_ifc> &out)
{
    return pam_get_item (handle, PAM_CONV, (const void **)pout);
}

