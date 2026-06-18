#ifndef GATEWAY_TH_H
#define GATEWAY_TH_H

#include <stdatomic.h>
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>

#include <internal/log_types.h>

typedef struct muninn_t muninn_t;

typedef struct gateway_th_data
{
    muninn_t          *muninn;

} gateway_th_data;

void *gateway_loop_fn(void *arg);
void *gateway_stop_fn(void *arg);
void *gateway_join_fn(void *arg);

void *gateway_th_perform(void *context, void *data);


#endif