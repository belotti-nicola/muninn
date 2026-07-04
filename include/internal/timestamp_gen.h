#ifndef TIMESTAMP_GEN_H
#define TIMESTAMP_GEN_H

#include <stdio.h>
#include <stdint.h>

void     timestamp_now(char* buffer, size_t size);
uint64_t timestamp_u64();
long long timestamp_now_ms();



#endif