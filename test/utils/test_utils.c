#include "test_utils.h"

#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <dirent.h>

#define PATH_LEN 1024

bool file_exists_impl(const char *path)
{
    struct stat buffer;
    return stat(path, &buffer) == 0 ? true : false;
}

char *get_executable_directory(char *out, size_t out_size)
{
    if (out == NULL || out_size == 0) return NULL;
    char raw_path[PATH_LEN] = {0};

    ssize_t len = readlink("/proc/self/exe", raw_path, sizeof(raw_path) - 1);
    if (len == -1) {
        printf("Error: could not readlink /proc/self/exe\n");
        return NULL;
    }

    raw_path[len] = '\0'; 
    char *dir = dirname(raw_path);

    int written = snprintf(out, out_size, "%s", dir);
    if (written < 0 || (size_t)written >= out_size) {
        printf("Error: out buffer too small for executable path\n");
        return NULL;
    }
    return out;
}

char *compute_test_file_name_impl(char *out, size_t out_size, const char *calling_file)
{
    if (out == NULL || out_size == 0) return NULL;

    if (get_executable_directory(out, out_size) == NULL) return NULL;

    size_t cwd_len = strlen(out);
    size_t remaining_space = out_size - cwd_len;

    // Copiamo calling_file in un buffer locale perché basename() può modificare la stringa!
    char file_copy[PATH_LEN];
    strncpy(file_copy, calling_file, sizeof(file_copy) - 1);
    file_copy[sizeof(file_copy) - 1] = '\0';

    char *file_basename = basename(file_copy); 
    char *dot = strrchr(file_basename, '.');
    int name_len = (dot != NULL) ? (dot - file_basename) : (int)strlen(file_basename);

    int written = snprintf(out + cwd_len, remaining_space, "/%.*s.log", name_len, file_basename);
    if (written < 0 || (size_t)written >= remaining_space) return NULL;

    return out;
}

int files_counter_impl(const char* path, const char *calling_file)
{
    char file_copy[PATH_LEN];
    strncpy(file_copy, calling_file, sizeof(file_copy) - 1);
    file_copy[sizeof(file_copy) - 1] = '\0';

    char *f_basename = basename(file_copy);
    char *dot = strrchr(f_basename, '.');
    int len_before_dot = (dot != NULL) ? (dot - f_basename) : (int)strlen(f_basename);

    DIR* dir = opendir(path);
    if (!dir) return -1;

    struct dirent* entry;
    int count = 0;

    while ((entry = readdir(dir)) != NULL) {
        const char* name = entry->d_name;
        size_t len = strlen(name);

        if(strncmp(name, f_basename, len_before_dot) != 0) continue;

        // Corretto il parametro size di strncmp!
        if(len > 4 && strncmp(name + len - 4, ".lz4", 4) == 0) {
            count++;
            continue;
        }

        if(len > 9 && strncmp(name + len - 9, ".rotating", 9) == 0) {
            count++;
            continue;
        }       
    }

    closedir(dir);
    return count;
}