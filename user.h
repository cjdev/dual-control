#ifndef _USER_H
#define _USER_H
#include <vector>
#include <string>
#include <pwd.h>

class user {
    private:
        struct passwd *info;

    public:
        user(struct passwd *sys_info);
        bool valid();
        std::string home_directory();
};

class concrete_user : public user {
    private:
        std::vector<char> buffer;
        std::shared_ptr<struct passwd> store;
    public:
        concrete_user(const std::string &user_name);
};


#endif

