#include "user.h"

#include <vector>
#include <string>
#include <pwd.h>


user::user(struct passwd *sys_info) : info(sys_info) {
}

bool user::valid() {
    return info;
}

std::string user::home_directory() {
    return info->pw_dir;
}

