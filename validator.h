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

#ifndef _VALIDATOR_H
#define _VALIDATOR_H

#include <string>
#include <memory>

#include "user.h"
#include "token.h"
#include "generator.h"

class validator_ifc
{
public:
    virtual ~validator_ifc() {}
    virtual bool validate (const std::string &requester_user_name,
                           const std::string &authorizer_user_name,
                           const std::string &authorizer_token,
                           const std::string &reason)
    {
        return false;
    }
};

class validator
{
private:
    std::shared_ptr<validator_ifc> delegate_;
public:
    validator (const std::shared_ptr<validator_ifc> &delegate) : delegate_
        (delegate) {}
    validator() : validator (std::shared_ptr<validator_ifc>
                                 (new validator_ifc)) {}
    bool validate (const std::string &requester_user_name,
                   const std::string &authorizer_user_name,
                   const std::string &authorizer_token,
                   const std::string &reason)
    {
        return delegate_->validate (requester_user_name, authorizer_user_name,
                                    authorizer_token, reason);
    }
    static validator create (const directory &directory,
                             const tokens &token_supplier);
};

#endif

