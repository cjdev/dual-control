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
    virtual ~unistd_ifc() {}
    virtual long int sysconf (int name) const
    {
        return -1;
    }
    virtual const char *getlogin() const
    {
        return "";
    }
    virtual int seteuid (uid_t euid) const
    {
        return -1;
    };

};

class unistd
{
public:
    typedef std::shared_ptr<unistd_ifc> delegate;

private:
    delegate delegate_;

public:
    unistd (delegate delegate): delegate_ (delegate) {}
    unistd() : delegate_ (delegate (new unistd_ifc)) {}
    long int sysconf (int name) const
    {
        return delegate_->sysconf (name);
    }
    const char *getlogin() const
    {
        return delegate_->getlogin();
    }
    int seteuid (uid_t euid) const
    {
        return delegate_->seteuid (euid);
    };
    static unistd create();
};

template class std::shared_ptr<unistd_ifc>;

#endif

