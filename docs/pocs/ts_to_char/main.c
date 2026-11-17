#include <stdio.h>
#include <time.h>

#include <stdbool.h>

#define BUFFER_SIZE 20

bool timestamp_now(char* buffer, size_t size)
{
    time_t now = time(NULL);

    struct tm tm_now;

    localtime_r(&now, &tm_now);

    // care: these function writes size bytes.
    // but tries to write each token -> if bytes arent enough: it is not written
    
    size_t written = strftime(
        buffer,
        size,
        "%Y-%m-%d %H:%M:%S",
        &tm_now
    );

    return written > 0;
}

int main()
{
    char buffer[BUFFER_SIZE] = {0};
    bool rc = timestamp_now(buffer,BUFFER_SIZE);

    printf("%s -> '%s'\n",rc?"OK":"KO",buffer);

    return 0;
}