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
#include <functional>

#include "user.h"
#include "sys_fstream.h"

class tokens_ifc
{
public:
    using token_generator = std::function<std::string()>;
    virtual ~tokens_ifc() {}
    virtual std::string token (const user &user) const
    {
        return "";
    }
    virtual void save (const user &user, const std::string &token) const {}
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
    std::string token (const user &user) const
    {
        return delegate_->token (user);
    }
    void save (const user &user, const std::string &token) const
    {
        return delegate_->save (user, token);
    }
    static tokens create (const fstreams &fstreams,
                          const tokens_ifc::token_generator &generate_token);
};

#endif

