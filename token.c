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


int get_passwd(const char *user, struct passwd *passwd, buffer_t buffer) {
    struct passwd *found_passwd = 0;
    getpwnam_r(user, passwd, use_buffer(buffer), buffer_size(buffer), &found_passwd);
    return (found_passwd != 0);
}

int validate_token(const char *token) {

    char *filepath = 0;
    char *working_token = 0;
    buffer_t buffer = allocate_buffer();

    int ok = 0;

    int token_length = strlen(token);
    working_token = (char *) malloc((token_length + 1) * sizeof(char));
    strcpy(working_token, token);
    char *colon = strchr(working_token, ':');
    if (!colon) {
        goto finally;
    }

    *colon = 0;
    char *user = working_token;
    char *user_token = colon + 1;

    struct passwd passwd;
    int user_found = get_passwd(user, &passwd, buffer);

    // check if user is known
    if(!user_found) {
       goto finally;
    }

    const char *directory = passwd.pw_dir;

    int dir_len = strlen(directory);
    int fname_len = strlen(".dual_control");
    filepath = (char *)malloc((dir_len + 1 + fname_len + 1) * sizeof(char));

    strcpy(filepath, directory);
    strcat(filepath, "/");
    strcat(filepath, ".dual_control");

    struct stat file_stat;
    int check_file = stat(filepath, &file_stat);
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

    free(filepath);
    free(working_token);
    free_buffer(&buffer);

    return ok;
}


