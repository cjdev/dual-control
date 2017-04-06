#include <stdlib.h>
#include <string.h>
#include <security/pam_modules.h>
#include <pwd.h>
#include <unistd.h>

#include "token.h"

#include "test_support.h"

int user_is_known(const char *user) {
    struct passwd *passwd = (struct passwd *) malloc(sizeof(struct passwd));
    size_t bufsize = (size_t) sysconf(_SC_GETPW_R_SIZE_MAX);
    char * buffer = (char *) malloc(bufsize * sizeof(char));
    struct passwd *found_passwd = 0;
    getpwnam_r(user, passwd, buffer, bufsize, &found_passwd);
    int known = found_passwd != 0;

    free(buffer);
    free(passwd);

    return known;

}

int validate_token(const char *token) {
    int ok = 0;

    char *user = 0;

    // duplicate
    int token_length = strlen(token);
    user = (char *) malloc((token_length + 1) * sizeof(char));
    strcpy(user, token);

    // find the first colon
    char *colon = strchr(user, ':');
    if (!colon) {
        goto finally;
    }

    // poke a zero so dup is the username
    *colon = 0;

    // check if user is known
    if(!user_is_known(user)) {
       goto finally;
    }

    ok = 1;

    // determine if user is system user
    // fail if not
    finally:

    free(user);

    return ok;
}


