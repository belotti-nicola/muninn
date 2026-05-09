#include "muninn.h"

#include <assert.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    muninn_t muninn;

    muninn_init("test_full.log", &muninn);

    muninn_log(&muninn, "hello");
    muninn_log(&muninn, "world");

    muninn_shutdown(&muninn);

    FILE *test_file = fopen("test_full.log", "r");
    assert(test_file != NULL);

    char buffer[256],*res;

    res = fgets(buffer, sizeof(buffer), test_file);
    assert(res != NULL);    
    assert(strncmp(buffer, "hello", 5) == 0);

    res = fgets(buffer, sizeof(buffer), test_file);
    assert(res != NULL);    
    assert(strncmp(buffer, "world", 5) == 0);

    if (fgets(buffer, sizeof(buffer), test_file) == NULL)
    {
        if (feof(test_file))
        {
            
        }
        else if (ferror(test_file))
        {
            return 1;
        }
    }

    fclose(test_file);
    return 0;
}