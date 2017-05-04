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

#ifndef SYS_CSTDLIB_H_
#define SYS_CSTDLIB_H_

#include <memory>

class stdlib_ifc
{
public:
    virtual void srand(unsigned int seed) const {
    }
    virtual int rand() const
    {
        return 0;
    }
};

class stdlib
{
public:
    using delegate = std::shared_ptr<stdlib_ifc>;
private:
    delegate delegate_;
public:
    stdlib (delegate delegate = std::make_shared<stdlib_ifc>()) : delegate_
        (delegate) {}
    void srand(unsigned int seed) const {
        return delegate_->srand(seed);
    }
    int rand() const
    {
        return delegate_->rand();
    }
    static const stdlib &get();
};

#endif

