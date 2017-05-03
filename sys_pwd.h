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

#ifndef _SYS_PWD_H
#define _SYS_PWD_H

#include <memory>
#include <pwd.h>

class pwd_ifc
{
public:
    virtual ~pwd_ifc() {}
    virtual int getpwnam_r (const char *user_name, passwd *out, char *buffer,
                            size_t buffer_sz, passwd **result) const
    {
        *result = 0;
        return 0;
    };
};

class pwd
{
public:
    typedef std::shared_ptr<pwd_ifc> delegate;

private:
    delegate delegate_;

public:
    pwd (const delegate delegate) : delegate_ (delegate) {}
    pwd() : delegate_ (delegate (new pwd_ifc)) {}
    int getpwnam_r (const char *user_name, passwd *out, char *buffer,
                    size_t buffer_sz, passwd **result) const
    {
        return delegate_-> getpwnam_r (user_name, out, buffer, buffer_sz, result);
    }
    static pwd create();
};

#endif

