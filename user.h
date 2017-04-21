#ifndef _USER_H
#define _USER_H
#include <vector>
#include <string>
#include <memory>
#include <pwd.h>

class user_ifc
{
public:
    virtual ~user_ifc() {}
};

class user : public user_ifc
{
private:
    std::shared_ptr<user_ifc> delegate_;
public:
    user (std::shared_ptr<user_ifc> delegate) : delegate_ (delegate) {}
    user() : user (std::shared_ptr<user_ifc> (new user_ifc)) {}
};

class directory_ifc
{
public:
    virtual ~directory_ifc() {}
    virtual std::vector<user> find_user (const std::string &user_name)
    {
        return std::vector<user>();
    }
};

class directory : public directory_ifc
{
private:
    std::shared_ptr<directory_ifc> delegate_;
public:
    directory (std::shared_ptr<directory_ifc> delegate) : delegate_
        (delegate) {}
    directory() : directory (std::shared_ptr<directory_ifc>
                                 (new directory_ifc)) {}
    std::vector<user> find_user (const std::string &user_name)
    {
        return delegate_->find_user (user_name);
    }

    static directory create();
};

#endif

