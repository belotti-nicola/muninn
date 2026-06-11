#include "timestamp.h"
#include <time.h>

uint64_t timestamp_generation()
{
    uint64_t current_ts = (uint64_t)time(NULL);
    return current_ts;
}