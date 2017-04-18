
#include "dual_control.h"

class impl : public dual_control_ifc {
    public:
        int authenticate(pam_handle *handle, int flags, const std::vector<const std::string> &arguments );
        int setcred(pam_handle *handle, int flags, const std::vector<const std::string> &arguments);
};

int impl::setcred(pam_handle *handle, int flags, const std::vector<const std::string> &arguments) {
    return PAM_SUCCESS;
}

int impl::authenticate(pam_handle *handle, int flags, const std::vector<const std::string> &arguments) {
    return -1209342;
}

dual_control create_dual_control() {
    return dual_control(new impl);
}



