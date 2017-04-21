#include <vector>
#include <security/pam_modules.h>
#include <security/pam_appl.h>

#include "pam.h"

class syspam : public pam_ifc
{
public:
    int get_conv (pam_handle *handle, const pam_conv **pout);
};

int syspam::get_conv (pam_handle *handle,
                      std::shared_ptr<pam_conv_ifc> &out)
{
    return pam_get_item (handle, PAM_CONV, (const void **)pout);
}

