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

#ifndef _SYS_UNISTD_H_
#define _SYS_UNISTD_H_

#include <memory>

#include <unistd.h>

class unistd_ifc
{
public:
    virtual long int sysconf (int name)
    {
        return -1;
    }
};

class unistd : public unistd_ifc
{
public:
    typedef std::shared_ptr<unistd_ifc> delegate;

private:
    delegate delegate_;

public:
    unistd (delegate delegate): delegate_ (delegate) {}
    unistd() : delegate_ (delegate (new unistd_ifc)) {}
    long int sysconf (int name)
    {
        return delegate_->sysconf (name);
    }
    static unistd system();
};

#endif

