#include <stdio.h>
#include <time.h>

#include <internal/timestamp_gen.h>

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

uint64_t timestamp_u64()
{
    uint64_t current_ts = (uint64_t)time(NULL);
    return current_ts;
}

long long timestamp_now_ms()
{
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);

    long long ms_since_epoch = ((long long)now.tv_sec * 1000) + (now.tv_nsec / 1000000);

    return ms_since_epoch;
}

