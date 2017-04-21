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

#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <string>
#include <vector>
#include <security/pam_modules.h>

class pam_request
{
    int argc_;
    const char **argv_;
    int flags_;
    pam_handle *handle_;
public:
    pam_request (pam_handle *handle, int flags, int argc, const char **argv)
        : handle_ (handle),
          flags_ (flags),
          argc_ (argc),
          argv_ (argv) {}
    std::vector<std::string> arguments() const;
    int flags() const
    {
        return flags_;
    }
    pam_handle *handle() const
    {
        return handle_;
    }
};

#endif

