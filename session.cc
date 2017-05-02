#include <memory>
#include <string>

#include "session.h"
#include "sys_pam.h"
#include "request.h"

namespace
{
class impl : public session_ifc {
    private:
        pam pam_;
    public:
    impl(const pam &pam) : pam_(pam) {}
    std::string user_name (const pam_request &request)  const {
        const char *user_name;
        pam_.get_user(request.handle(), &user_name);
        return user_name;
    }
};

}

session session::create(const pam &pam) {
    return session(std::make_shared<impl>(pam));
}
