#ifndef INSTALLER_H_
#define INSTALLER_H_

#include <string>
#include <memory>
#include <functional>

#include "token.h"
#include "sys_unistd.h"
#include "user.h"


class installer_ifc {
    public:
        using generator = std::function<std::string()>;
        virtual std::string install_token() const {
            return "123456";
        }
};

class installer {
    public:
        using delegate = std::shared_ptr<installer_ifc>;
    private:
        delegate delegate_;
    public:
        installer(const delegate &delegate = std::make_shared<installer_ifc>()) : delegate_(delegate) {}
        std::string install_token() const {
            return delegate_->install_token();
        }
        static installer create(const tokens &tokens, const unistd &unistd,
            const directory &directory, const installer_ifc::generator &generator);
};




#endif
