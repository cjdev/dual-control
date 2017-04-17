#include <security/pam_modules.h>
#include <string>
#include <memory>
#include <vector>

#include "arguments.h"

std::vector<const std::string> convert_arguments(int argc, const char **argv);

extern dual_control dc;

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    std::vector<const std::string> arguments = convert_arguments(argc, argv);
    return dc.authenticate(pamh, flags, arguments);
}

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    std::vector<const std::string> arguments = convert_arguments(argc, argv);
    return dc.setcred(pamh, flags, arguments);
}

