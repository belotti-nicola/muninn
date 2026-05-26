#include "muninn.h"

#include <assert.h>
#include <string.h>
#include <unistd.h>

#define LOG "test_simple.log"

int main(void)
{
    
    muninn_t muninn = {0};

    muninn_init(&muninn,LOG);

    muninn_log_info(&muninn, "hello");
    muninn_log_info(&muninn, "world");

    muninn_shutdown(&muninn);

    FILE *test_file = fopen(LOG, "r");
    if(test_file == NULL)
    {
        printf("Error: char pointer is null at line %d.\n",__LINE__);
        return 1;
    }

    char buffer[256];
    char *res;

    res = fgets(buffer, sizeof(buffer), test_file);
    if(res == NULL)
    {
        printf("Error: char pointer is null at line %d.\n",__LINE__);
        return 1;
    }
    if(strstr(buffer, "hello") == NULL)
    {
        printf("Error: buffer(%s) differs from expected(%s) at line %d.\n",buffer,"hello",__LINE__);
        return 1;
    }

    res = fgets(buffer, sizeof(buffer), test_file);
    if(res == NULL)
    {
        printf("Error: char pointer is null at line %d.\n",__LINE__);
        return 1;
    }
    if(strstr(buffer, "world") == NULL)
    {
        printf("Error: buffer(%s) differs from expected(%s) at line %d.\n",buffer,"world",__LINE__);
        return 1;
    }

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