#ifndef _USER_H
#define _USER_H
#include <vector>
#include <string>
#include <memory>
#include <pwd.h>

class user {
    public:
        virtual ~user() {}
//        virtual std::string home_directory() = 0;
};


typedef std::shared_ptr<user> user_p;
class directory {
    public:
        virtual ~directory() {}
        virtual const user_p find_user(const std::string &user_name) = 0;
};

typedef std::shared_ptr<directory> directory_p;
#endif

