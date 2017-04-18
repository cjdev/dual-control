#ifndef _DUAL_CONTROL_H
#define _DUAL_CONTROL_H

#include <string>
#include <memory>
#include <vector>
#include <security/pam_modules.h>

#include "validator.h"
#include "conversation.h"


class dual_control_ifc {
    public:
        virtual ~dual_control_ifc() {}
        virtual int authenticate(pam_handle_t *handle, int flags, const std::vector<const std::string> &arguments ) = 0;
        virtual int setcred(pam_handle_t *handle, int flags, const std::vector<const std::string> &arguments) = 0;
};

typedef std::shared_ptr<dual_control_ifc> dual_control;

struct dual_control_configuration {
    validator validator;
    conversations conversations;
};

dual_control create_dual_control(const dual_control_configuration &configuration);



#endif

