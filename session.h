#ifndef _SESSION_H_
#define _SESSION_H_

#include <string>

#include "request.h"
#include "sys_pam.h"

class sessions_ifc {
    public:
        virtual ~sessions_ifc () {}
        virtual std::string user_name (const pam_request &request)  const {
            return "";
        }
};

class sessions {
    private:
        std::shared_ptr<sessions_ifc> delegate_;
    public:
        sessions(std::shared_ptr<sessions_ifc> delegate = std::make_shared<sessions_ifc>()) : delegate_(delegate) {}
        std::string user_name (const pam_request &request) const {
            return delegate_->user_name(request);
        }
        static sessions create(const pam &pam);
};

#endif

