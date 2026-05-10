#include <internal/zstd_wrapper.h>

#include <stdio.h>
#include <time.h>

void timestamp_now(char* buffer, size_t size)
{
    time_t now = time(NULL);

    struct tm tm_now;

    localtime_r(&now, &tm_now);

    strftime(
        buffer,
        size,
        "%Y-%m-%d_%H-%M-%S",
        &tm_now
    );
}

