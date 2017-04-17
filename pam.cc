#include <string>
#include <vector>
#include <memory>
#include <security/pam_modules.h>
#include <security/pam_appl.h>

#include "pam.h"

class pam_conversation_impl : public pam_conversation {
    private:
        struct pam_conv *pam_conv_;
    public:
        pam_conversation_impl(pam_conv *conv) : pam_conv_(conv) {}
        int conv(const std::vector<const struct pam_message *> &prompts, std::vector<struct pam_response *> &answers);
};

class pam_impl : public pam {
    public:
        int get_conversation(pam_handle_t *pamh, std::shared_ptr<pam_conversation> &conversation);
};

int pam_impl::get_conversation(pam_handle_t *pamh, std::shared_ptr<pam_conversation> &conversation) {
    struct pam_conv *pam_conv;
    int result = pam_get_item(pamh, PAM_CONV, (const void **)&pam_conv);
    conversation.reset(new pam_conversation_impl(pam_conv));
    return result;
}

int pam_conversation_impl::conv(const std::vector<const struct pam_message *> &prompts, std::vector<struct pam_response *> &answers) {
    return pam_conv_->conv(prompts.size(), const_cast<const struct pam_message **>(prompts.data()),
            answers.data(), pam_conv_->appdata_ptr);
}

pam_p get_system_pam() {
    return (pam_p)new pam_impl;
}
