#ifndef _PAM_H
#define _PAM_H
#include <string>
#include <vector>
#include <memory>
#include <tuple>
#include <security/pam_modules.h>

class pam_ifc
{
public:
    virtual std::tuple<int,std::vector<pam_response>> conv (pam_handle *handle,
            const std::vector<pam_message> &prompts);

};

class pam : public pam_ifc
{
    typedef std::shared_ptr<pam_ifc> delegate;
private:
    delegate delegate_;
public:
    pam (const delegate &delegate) : delegate_ (delegate) {}
    pam() : pam (delegate (new pam_ifc)) {}
    std::tuple<int,std::vector<pam_response>> conv (pam_handle *handle,
                                           const std::vector<pam_message> &prompts)
    {
        return delegate_-> conv (handle, prompts);
    }
};

pam system_pam();

#endif

