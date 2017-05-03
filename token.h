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

#ifndef _TOKEN_H
#define _TOKEN_H

#include <string>
#include <memory>

#include "user.h"
#include "sys_fstream.h"

class tokens_ifc
{
public:
    virtual ~tokens_ifc() {}
    virtual std::string token (user &user)
    {
        return "";
    }
};

class tokens
{
public:
    typedef std::shared_ptr<tokens_ifc> delegate;
private:
    delegate delegate_;
public:
    tokens (delegate delegate) :
        delegate_ (delegate) {}
    tokens() : tokens (
            delegate (new tokens_ifc)) {}
    std::string token (user &user)
    {
        return delegate_->token (user);
    }
    static tokens create (fstreams &fstreams);
};

#endif

