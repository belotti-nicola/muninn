#ifndef GATEWAY_TH_H
#define GATEWAY_TH_H

#include <stdatomic.h>
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>

#include <muninn.h>

typedef struct gateway_th_data
{
    pthread_t          th;
    atomic_bool        running;
    muninn_t          *muninn;

} gateway_th_data;

void *gateway_loop_fn(void *arg);
void *gateway_stop_fn(void *arg);
void *gateway_join_fn(void *arg);


#endif