#ifndef _DUAL_CONTROL_H
#define _DUAL_CONTROL_H

#include <string>
#include <memory>
#include <vector>
#include <security/pam_modules.h>

class dual_control_ifc {
    public:
        virtual int authenticate(pam_handle_t *handle, int flags, const std::vector<const std::string> &arguments ) = 0;
        virtual int setcred(pam_handle_t *handle, int flags, const std::vector<const std::string> &arguments) = 0;
};

class dual_control : public dual_control_ifc {
    private:
        std::shared_ptr<dual_control_ifc> delegate_;
    public:
        int authenticate(pam_handle_t *handle, int flags, const std::vector<const std::string> &arguments ) {
            return delegate_->authenticate(handle, flags, arguments);
        }
        int setcred(pam_handle_t *handle, int flags, const std::vector<const std::string> &arguments) {
            return delegate_->setcred(handle, flags, arguments);
        }
};

#endif

