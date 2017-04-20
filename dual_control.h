/* PAM handlers class interface. The methods on the dual control class
 * correspond directly to the specified PAM handler entrypoints.
 *
 * See "dual_control_integrate.cc"
 */

#ifndef _DUAL_CONTROL_H
#define _DUAL_CONTROL_H

#include <memory>

#include "request.h"
#include "validator.h"
#include "conversation.h"
#include "logger.h"

struct dual_control_configuration {
    validator validator;
    conversation conversation;
    logger logger;
};

class dual_control_ifc
{
public:
    virtual ~dual_control_ifc() {}
    virtual int authenticate (const pam_request &request);
    virtual int setcred (const pam_request &request);
};

class dual_control : public dual_control_ifc
{
private:
    std::shared_ptr<dual_control_ifc> delegate_;
public:
    dual_control (std::shared_ptr<dual_control_ifc> delegate) : delegate_
        (delegate) {}
    dual_control() : dual_control (std::shared_ptr<dual_control_ifc>
                                       (new dual_control_ifc)) {}
    int authenticate (const pam_request &request)
    {
        return delegate_->authenticate (request);
    }
    int setcred (const pam_request &request)
    {
        return delegate_->setcred (request);
    }

};

dual_control create_dual_control (const dual_control_configuration
                                  &configuration);

#endif

