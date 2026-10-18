#include "test_utils.h"

#define TEST_FILE_SIZE 100

int main()
{
    char test_file[TEST_FILE_SIZE] = {0};

    if ( get_test_file_data_path(test_file,TEST_FILE_SIZE,"/data/test_formatter_csv_data.csv") == NULL )
    {
        TRACE_ERROR_POSITION();
        TEST_INFO("Error: could not get test data file.");
        return 1;
    }

    return 0;
}