#include "internal/munin_int.h"
#include <string.h>

#include "muninn.h"

void muninn_setup(const char *path, muninn_t *m)
{

    strncpy(m->path, path, PATH_LEN - 1);
    m->path[PATH_LEN - 1] = '\0';

    m->file = NULL;
    m->q = ts_queue_setup();

    atomic_init(&m->running, false);
    atomic_init(&m->keep_looping, true);
}

void muninn_start(muninn_t *muninn)
{
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

void muninn_join(muninn_t *muninn)
{
    atomic_store(&muninn->keep_looping, false);

    //TODO WAKE MECHANISM TO IMPLEMENT
    muninn_log(muninn,"termination");

    int rc = pthread_join(muninn->thread, NULL);
    if( rc != 0 )
    {
        printf("Error! %d\n",rc);
        return;
    }
}

void *muninn_thread_function(void *arg)
{
    muninn_t *muninn = (muninn_t *)arg;

    muninn->file = fopen(muninn->path, "a");
    if (muninn->file == NULL)
    {
        perror("fopen");
        atomic_store(&muninn->running, false);
        return NULL;
    }

    atomic_store(&muninn->running, true);

    queue_message_t qm;
    while(atomic_load(&muninn->keep_looping))
    {
        bool ec = ts_queue_pop(&muninn->q,&qm);
        if(ec)
        {
            fprintf(muninn->file, "%s\n", qm.message);
            fflush(muninn->file);
        }
    }
    atomic_store(&muninn->running, false);

    fclose(muninn->file);
    muninn->file = NULL;
    ts_queue_release(&muninn->q);

    return NULL;
}