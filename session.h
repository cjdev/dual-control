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

#ifndef _SESSION_H_
#define _SESSION_H_

#include <string>

#include "request.h"
#include "sys_pam.h"

class sessions_ifc
{
public:
    virtual ~sessions_ifc () {}
    virtual std::string user_name (const pam_request &request)  const
    {
        return "";
    }
};

class sessions
{
    public:
        typedef std::shared_ptr<sessions_ifc> delegate;
private:
    delegate delegate_;
public:
    sessions (delegate delegate =
                  std::make_shared<sessions_ifc>()) : delegate_ (delegate) {}
    std::string user_name (const pam_request &request) const
    {
        return delegate_->user_name (request);
    }
    static sessions create (const pam &pam);
};

#endif

