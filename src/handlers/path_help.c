// path_helper.c

#include "path_helper.h"
#include <libgen.h> // For dirname and basename functions
#include <limits.h> // For PATH_MAX
#include <unistd.h> // For readlink
#include <stdlib.h> // For malloc
#include <stdio.h>  // For snprintf

char* resource_path(const char* relative_path) {
    char exec_path[PATH_MAX] = {0};
    ssize_t len = readlink("/proc/self/exe", exec_path, PATH_MAX - 1);
    if (len == -1) {
        // Handle error appropriately
        perror("readlink");
        return NULL;
    }

    char* dir_path = dirname(exec_path);
    size_t needed = snprintf(NULL, 0, "%s/%s", dir_path, relative_path) + 1;
    char* path = malloc(needed);
    if (path) {
        sprintf(path, "%s/%s", dir_path, relative_path);
    }

    return path;
}
