#ifndef _USER_H
#define _USER_H
#include <vector>
#include <string>
#include <memory>
#include <pwd.h>

class user {
    private:
        struct passwd *info;

    public:
        user(struct passwd *sys_info);
        std::string home_directory();
};


const std::shared_ptr<user> create_user(const std::string &user_name);
typedef std::shared_ptr<user> user_p;

#endif

