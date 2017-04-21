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
#include <string>

#include "request.h"

std::vector<std::string> pam_request::arguments() const
{
    std::vector<std::string> rval;

    for (int i= 0; i < argc_; i++) {
        rval.push_back (argv_[i]);
    }

    return rval;
}

