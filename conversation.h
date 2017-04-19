#ifndef _CONVERSATION_H
#define _CONVERSATION_H

#include <security/pam_modules.h>
#include <string>
#include <memory>

#include "pam.h"



class conversation_result
{
private:
    std::string token_;
    std::string user_name_;
public:
    conversation_result (std::string user_name, std::string token)
        : token_ (token),
          user_name_ (user_name) {}
    std::string token()
    {
        return token_;
    }
    std::string user_name()
    {
        return user_name_;
    }
};


class conversations_ifc
{
public:
    virtual ~conversations_ifc() {}
    virtual conversation_result initiate_conversation()
    {
        return conversation_result ("","");
    }
};

class conversations : public conversations_ifc
{
private:
    std::shared_ptr<conversations_ifc> delegate_;
public:
    conversations() : conversations (std::shared_ptr<conversations_ifc>
                                         (new conversations_ifc)) {}
    conversations (const std::shared_ptr<conversations_ifc> &delegate) :
        delegate_ (delegate) {}
    conversation_result initiate_conversation()
    {
        return delegate_->initiate_conversation();
    }
};

class token_conversation
{
public:
    virtual ~token_conversation() {}
    virtual std::string token()
    {
        return "";
    }
    virtual std::string user_name()
    {
        return "";
    }
};

class pam_token_conversation : public token_conversation
{
public:
    std::string user_;
    std::string token_;
    pam_token_conversation (pam_handle_t *pamh);
    pam_token_conversation (pam_handle_t *pamh, const pam_p);
    std::string token();
    std::string user_name();
};

#endif

