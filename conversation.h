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

#ifndef _CONVERSATION_H
#define _CONVERSATION_H

#include <string>
#include <memory>

#include "request.h"
#include "sys_pam.h"

struct conversation_result {
    std::string user_name;
    std::string token;
};

class conversation_ifc
{
public:
    virtual ~conversation_ifc() {}
    virtual conversation_result initiate (const pam_request &request)
    {
        return { "", "" };
    }
};

class conversation: public conversation_ifc
{
private:
    std::shared_ptr<conversation_ifc> delegate_;
public:
    conversation (const std::shared_ptr<conversation_ifc> &delegate) :
        delegate_ (delegate) {}
    conversation() : conversation (std::shared_ptr<conversation_ifc>
                                       (new conversation_ifc)) {}
    conversation_result initiate (const pam_request &request)
    {
        return delegate_->initiate (request);
    }
    static conversation create (pam &pam);
};

inline conversation wrap (conversation_ifc *delegate)
{
    return conversation (std::shared_ptr<conversation_ifc> (delegate));
};

#endif

