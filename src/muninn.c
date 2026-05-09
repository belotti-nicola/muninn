#include "internal/munin_int.h"
#include <string.h>

#include "muninn.h"

muninn_t muninn_create(const char *path)
{
    muninn_t m;
    m.running = false;
    *(m.keep_looping) = true;
    m.path[0] = '\0';
    m.file    = NULL;

    return m;
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
    
    bool b = muninn->running;
    b = true;
}

void muninn_log(muninn_t *muninn, const char *msg)
{
    queue_message_t qm;
    strncpy(qm.message, msg, sizeof(qm.message) - 1);
    qm.message[sizeof(qm.message) - 1] = '\0';

    ts_queue_push(muninn->q, &qm);
}

void muninn_join(muninn_t *muninn)
{

    int rc = pthread_join(muninn->thread,NULL);
    if( rc != 0 )
    {
        printf("Error! %d\n",rc);
        return;
    }
}

void *muninn_thread_function(void *arg)
{
    muninn_t *muninn = (muninn_t *)arg;

    queue_message_t qm;
    bool *keep_looping = muninn->keep_looping;
    while(*keep_looping)
    {
        bool ec = ts_queue_pop(muninn->q,&qm);
        if(ec)
        {
            printf("%s\n",qm.message);
        }
    }

    ts_queue_release(*muninn->q);
}