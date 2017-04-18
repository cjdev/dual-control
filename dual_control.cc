
#include "dual_control.h"
#include "conversation.h"
#include "validator.h"

class impl : public dual_control_ifc {
    private:
       conversations conversations_;
       validator validator_;
    public:
        impl(const dual_control_configuration &configuration);
        int authenticate(pam_handle *handle, int flags, const std::vector<const std::string> &arguments );
        int setcred(pam_handle *handle, int flags, const std::vector<const std::string> &arguments);
};

impl::impl(const dual_control_configuration &configuration) :
    conversations_(configuration.conversations),
    validator_(configuration.validator) {}

int impl::setcred(pam_handle *handle, int flags, const std::vector<const std::string> &arguments) {
    return PAM_SUCCESS;
}

int impl::authenticate(pam_handle *handle, int flags, const std::vector<const std::string> &arguments) {

    conversation_result result = conversations_->initiate_conversation();
    std::string user = result.user_name();
    std::string token = result.token();

    if (validator_->validate(user, token)) {
        return PAM_SUCCESS;
    }
    return 1212;
}

dual_control create_dual_control(const dual_control_configuration &configuration) {
    return dual_control(new impl(configuration));
}



