#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stddef.h>
#include <stdbool.h>

#define TEST_ERROR(fmt, ...) \
    printf("[TEST ERROR] %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define TEST_INFO(fmt, ...) \
    printf("[TEST INFO] " fmt "\n", ##__VA_ARGS__)

#define TRACE_ERROR_POSITION() \
    printf("Error at %s:%d\n",__FILE__,__LINE__);

#define file_exists(filepath)              file_exists_impl(filepath)
#define test_file_exists(out)              test_file_exists_impl(out)
#define compute_test_file_name(out, size)  compute_test_file_name_impl(out, size, __FILE__)
#define files_counter(path)                files_counter_impl(path, __FILE__)

bool  file_exists_impl(const char *fullpath);
bool *test_file_exists_impl(const char *test_name);
char *get_executable_directory(char *out, size_t out_size);
char *compute_test_file_name_impl(char *out, size_t out_size, const char *calling_file);
int   files_counter_impl(const char* path, const char *calling_file);


#endif // TEST_UTILS_H


