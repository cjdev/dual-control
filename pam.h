#ifndef _PAM_H
#define _PAM_H
#include <string>
#include <vector>
#include <memory>
#include <security/pam_modules.h>

class pam_conversation
{
public:
    virtual int conv (const std::vector<const struct pam_message *> &prompts,
                      std::vector<struct pam_response *> &answers) = 0;
};
typedef std::shared_ptr<pam_conversation> pam_conversation_p;

class pam
{
public:
    virtual int get_conversation (pam_handle_t *pamh,
                                  std::shared_ptr<pam_conversation> &conversation) = 0;
};
typedef std::shared_ptr<pam> pam_p;

pam_p get_system_pam();

#endif

