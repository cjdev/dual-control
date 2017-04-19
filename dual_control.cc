#include "dual_control.h"
#include "conversation.h"
#include "validator.h"

class impl : public dual_control_ifc
{
private:
    conversations conversations_;
    validator validator_;
    logger logger_;
public:
    impl (const dual_control_configuration &configuration);
    int authenticate (pam_handle *handle, int flags,
                      const std::vector<const std::string> &arguments );
    int setcred (pam_handle *handle, int flags,
                 const std::vector<const std::string> &arguments);
};

impl::impl (const dual_control_configuration &configuration) :
    conversations_ (configuration.conversations),
    validator_ (configuration.validator),
    logger_ (configuration.logger) {}

int impl::setcred (pam_handle *handle, int flags,
                   const std::vector<const std::string> &arguments)
{
    return PAM_SUCCESS;
}

int impl::authenticate (pam_handle *handle, int flags,
                        const std::vector<const std::string> &arguments)
{

    conversation_result conversation = conversations_.initiate_conversation();
    std::string user_name = conversation.user_name();
    std::string token = conversation.token();

    int auth_result = validator_.validate (user_name,
                                           token) ? PAM_SUCCESS : PAM_AUTH_ERR;

    logger_.log (auth_result, user_name, token);
    return auth_result;
}

dual_control create_dual_control (const dual_control_configuration
                                  &configuration)
{
    return dual_control (new impl (configuration));
}

