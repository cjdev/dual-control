#ifndef _SESSION_H_
#define _SESSION_H_

#include <string>

#include "request.h"
#include "sys_pam.h"

class session_ifc {
    public:
        virtual ~session_ifc () {}
        virtual std::string user_name (const pam_request &request)  const {
            return "";
        }
};

class session {
    private:
        std::shared_ptr<session_ifc> delegate_;
    public:
        session(std::shared_ptr<session_ifc> delegate = std::make_shared<session_ifc>()) : delegate_(delegate) {}
        std::string user_name (const pam_request &request) const {
            return delegate_->user_name(request);
        }
        static session create(const pam &pam);
};

#endif

