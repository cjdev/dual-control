#include <stdlib.h>
#include <string.h>
#include <security/pam_modules.h>
#include <pwd.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>

#include "token.h"
#include "test_support.h"

typedef struct buffer {
    size_t size;
    char *mem;
} buffer_t;

buffer_t allocate_buffer() {
    buffer_t buffer;

    buffer.size = (size_t) sysconf(_SC_GETPW_R_SIZE_MAX);
    buffer.mem = (char *) malloc(buffer.size * sizeof(char));

    return buffer;

}

void free_buffer(buffer_t *buffer) {
    free(buffer->mem);
    buffer->mem = 0;
    buffer->size = 0;
}

char *use_buffer(buffer_t buffer) {
    return buffer.mem;
}

size_t buffer_size(buffer_t buffer) {
    return buffer.size;
}


int user_is_known(const char *user, buffer_t buffer) {
    struct passwd *passwd = 0;
    passwd = (struct passwd *) malloc(sizeof(struct passwd));
    struct passwd *found_passwd = 0;
    getpwnam_r(user, passwd, use_buffer(buffer), buffer_size(buffer), &found_passwd);
    int known = found_passwd != 0;

    free(passwd);

    return known;

}

char *get_home_directory(const char *user, buffer_t buffer) {
    struct passwd *passwd = 0;
    passwd = (struct passwd *) malloc(sizeof(struct passwd));
    struct passwd *found_passwd = 0;
    getpwnam_r(user, passwd, use_buffer(buffer), buffer_size(buffer), &found_passwd);

    free(passwd);

    if (found_passwd) {
        return found_passwd->pw_dir;
    }

    return 0;
}

int validate_token(const char *token) {

    char *user = 0;
    char *dup_token = 0;
    struct stat *file_stat = 0;
    char *filepath = 0;


    int ok = 0;


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

    dup_token = (char *)malloc((token_length + 1) * sizeof(char));
    strcpy(dup_token, token);
    char *token_colon = strchr(dup_token, ':');
    if (!token_colon) {
       goto finally;
    }

     char *user_token = token_colon + 1;


    // check if user is known
    buffer_t user_buffer = allocate_buffer();
    if(!user_is_known(user, user_buffer)) {
       goto finally;
    }

    // find the token for found user
    buffer_t directory_buffer = allocate_buffer();
    char *directory = get_home_directory(user, directory_buffer);
    if (!directory) {
        goto finally;
    }

    // stat

    file_stat = (struct stat *) malloc(sizeof(struct stat));
    int dir_len = strlen(directory);
    int fname_len = strlen(".dual_control");
    filepath = (char *)malloc((dir_len + 1 + fname_len + 1) * sizeof(char));

    strcpy(filepath, directory);
    strcat(filepath, "/");
    strcat(filepath, ".dual_control");

    int check_file = stat(filepath, file_stat);
    if (check_file) {
        goto finally;
    }


    // read the file and grab token
    char fetched_token[7];
    FILE *fp = 0;
    fp = fopen(filepath, "r");
    fgets(fetched_token, 7, fp);
    fclose(fp);

    // check if token matches
    if(strcmp(user_token, fetched_token)) {
        goto finally;
    }

    ok = 1;

    finally:

    free(user);
    free_buffer(&user_buffer);
    free_buffer(&directory_buffer);
    free(file_stat);
    free(filepath);
    return ok;
}


