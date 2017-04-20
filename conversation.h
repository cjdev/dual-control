#ifndef _CONVERSATION_H
#define _CONVERSATION_H

#include <string>
#include <memory>

#include "request.h"
#include "pam.h"

struct conversation_result {
    std::string user_name;
    std::string token;
};

class conversation_ifc
{
public:
    virtual conversation_result initiate (const pam_request &request)
    {
        return { "", "" };
    }
};

class conversation: public conversation_ifc
{
private:
    std::shared_ptr<conversation_ifc> delegate_;
public:
    conversation (const std::shared_ptr<conversation_ifc> &delegate) :
        delegate_ (delegate) {}
    conversation() : conversation (std::shared_ptr<conversation_ifc>
                                       (new conversation_ifc)) {}
    conversation_result initiate (const pam_request &request)
    {
        return delegate_->initiate (request);
    }
};

inline conversation wrap (conversation_ifc *delegate)
{
    return conversation (std::shared_ptr<conversation_ifc> (delegate));
};

conversation create_conversation(pam &pam);

#endif

