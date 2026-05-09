#include "muninn.h"

#include <assert.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    muninn_t logger;

    muninn_setup("test.log", &logger);

    muninn_start(&logger);

    muninn_log(&logger, "hello");
    muninn_log(&logger, "world");

    sleep(1);

    muninn_join(&logger);


    FILE *test_file = fopen("test.log", "r");
    assert(test_file != NULL);

    char buffer[256];

    fgets(buffer, sizeof(buffer), test_file);
    assert(strncmp(buffer, "hello", 5) == 0);

    fgets(buffer, sizeof(buffer), test_file);
    assert(strncmp(buffer, "world", 5) == 0);

    fclose(test_file);

    return 0;
}