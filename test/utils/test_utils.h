#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#define TEST_ERROR(fmt, ...) \
    printf("[TEST ERROR] " fmt "\n", ##__VA_ARGS__)

#define TEST_INFO(fmt, ...) \
    printf("[TEST INFO ]  " fmt "\n", ##__VA_ARGS__)

#define TRACE_ERROR_POSITION(fmt, ...) \
    do { \
        printf("[TEST ERROR] %s:%d\n", __FILE__, __LINE__); \
        printf("[TEST ERROR] " fmt "\n", ##__VA_ARGS__); \
    } while(0)

#define file_exists(filepath)                         file_exists_impl(filepath)
#define test_file_exists(out)                         test_file_exists_impl(out)
#define compute_test_file_name(out, size)             compute_test_file_name_impl(out, size, __FILE__)
#define compute_test_current_dir(out,size)            compute_test_current_dir_impl(out,size)
#define files_counter(path)                           files_counter_impl(path, __FILE__)
#define sleep_ms(value_ms)                            sleep_ms_impl(value_ms)
#define count_rows_across_files(dir_path,prefix)      count_rows_across_files_impl(dir_path,prefix)
#define count_rows(filename)                          count_rows_impl(filename)
#define STRING_TO_INT(value)                          STRING_TO_INT_impl(value)
#define STRING_TO_UINT64(value)                       STRING_TO_UINT64_impl(value)
#define MEMMEM_SIMPLE(h,hl,n,nl)                      MEMMEM_SIMPLE_impl(h,hl,n,nl)
#define get_test_file_data_path(path,size,app)        get_test_file_data_path_impl(path,size,app)


bool  file_exists_impl(const char *fullpath);
bool *test_file_exists_impl(const char *test_name);
char *get_executable_directory(char *out, size_t out_size);
char *compute_test_file_name_impl(char *out, size_t out_size, const char *calling_file);
int   files_counter_impl(const char* path, const char *calling_file);
char  compute_test_current_dir_impl(char* out, size_t out_size);
void  sleep_ms_impl(int value_ms);
long  count_rows_across_files_impl(const char *dir_path, const char *prefix);
long  count_rows_impl(const char *filename);
int   STRING_TO_INT_impl(char *value);
uint64_t STRING_TO_UINT64_impl(char *value);
const void *MEMMEM_SIMPLE_impl(const void *haystack, size_t haystack_len,const void *needle, size_t needle_len);
char *get_test_file_data_path_impl(char *path, size_t path_max_size, char *append);

int count_occurencies(const char *string, size_t string_size, char c);


#endif // TEST_UTILS_H


