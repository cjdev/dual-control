#include <cstdlib>
#include <cstring>
#include <security/pam_modules.h>
#include <pwd.h>
#include <unistd.h>
#include <cstdio>
#include <sys/stat.h>
#include <string>
#include <vector>
#include <memory>
#include <fstream>

#include "token.h"
#include "test_support.h"


typedef struct passwd passwd_t;
class user {
    private:
        std::vector<char> passwd_buffer;
        std::shared_ptr<passwd_t> sys_user_info;

    public:
        user(const std::string &user_name);
        bool valid();
        const std::string token();

};

const std::string user::token() {
    // compute the token file path
    std::string filepath = (std::string)sys_user_info->pw_dir + "/.dual_control";
    // does the file exist?
    std::ifstream token_file(filepath);
    if (!token_file.good()) {
        return "";
    }

    // read file
    int token_length = 6;
    std::vector<char> token_buffer(token_length + 1);
    token_file.read(token_buffer.data(), token_length);

    // return contents
    return token_buffer.data();
}

bool user::valid() {
    return sys_user_info;
}

user::user(const std::string &user_name) :
        passwd_buffer(sysconf(_SC_GETPW_R_SIZE_MAX)) {
    std::shared_ptr<passwd> temp_passwd(new passwd);
    struct passwd *found_passwd(0);
    getpwnam_r(user_name.c_str(), sys_user_info.get(), passwd_buffer.data(), passwd_buffer.size(), &found_passwd);

    if (found_passwd) {
        sys_user_info = temp_passwd;
    }
}

int validate_token(const char *c_user_name, const char *c_token) {

   user_p user(create_user(c_user_name));

   validator_p validator(create_validator(user));

   return validator->valid(c_token);


}


