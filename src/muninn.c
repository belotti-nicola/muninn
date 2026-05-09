#include "internal/munin_int.h"
#include <string.h>

#include "muninn.h"

void muninn_init(const char *path, muninn_t *muninn)
{
    strncpy(muninn->path, path, P_SIZE - 1);
    muninn->path[P_SIZE - 1] = '\0';

    muninn->file = NULL;
    muninn->q = ts_queue_setup();

    atomic_init(&muninn->running, false);

    int rc = pthread_create(
        &(muninn->thread),NULL,muninn_thread_function,muninn);
    if( rc != 0 )
    {
        printf("Error! %d\n",rc);
        return;
    }
}

void muninn_log(muninn_t *muninn, const char *msg)
{
    queue_message_t qm;
    strncpy(qm.message, msg, sizeof(qm.message) - 1);
    qm.message[sizeof(qm.message) - 1] = '\0';

    ts_queue_push(&muninn->q, &qm);
}

void muninn_shutdown(muninn_t *muninn)
{
    ts_queue_stop(&muninn->q);
    int rc = pthread_join(muninn->thread, NULL);
    if( rc != 0 )
    {
        printf("Error! %d\n",rc);
        return;
    }

    ts_queue_release(&muninn->q);
}

void *muninn_thread_function(void *arg)
{
    muninn_t *muninn = (muninn_t *)arg;

    muninn->file = fopen(muninn->path, "a");
    if (!muninn->file)
        return NULL;

    atomic_store(&muninn->running, true);

    queue_message_t qm;

    while (ts_queue_pop(&muninn->q, &qm))
    {
        fprintf(muninn->file, "%s\n", qm.message);
        fflush(muninn->file);
    }

    atomic_store(&muninn->running, false);

    fclose(muninn->file);
    muninn->file = NULL;
    return NULL;
}