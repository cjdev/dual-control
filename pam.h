#ifndef _PAM_H
#define _PAM_H
#include <memory>
#include <vector>
#include <security/pam_modules.h>

class pam_ifc
{
public:
    virtual int get_conv (pam_handle *handle, const pam_conv **out)
    {
        return PAM_SERVICE_ERR;
    }
};

class pam : public pam_ifc
{
    typedef std::shared_ptr<pam_ifc> delegate;
private:
    delegate delegate_;
public:
    pam (const delegate &delegate) : delegate_ (delegate) {}
    pam() : pam (delegate (new pam_ifc)) {}
    int get_conv (pam_handle *handle, const pam_conv **out)
    {
        return delegate_->get_conv (handle, out);
    }
};

pam system_pam();

#endif

