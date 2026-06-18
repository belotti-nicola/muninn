#include <internal/muninn_worker_th.h>
#include <stdio.h>

int mw_init(muninn_worker_t *muninn_worker, const char *name, 
    void* (*thread_loop)(void*),
    void* (*thread_stop)(void*),
    void* (*thread_perform)(void *arg, void *data),
    void *context)
{
    if(muninn_worker == NULL || name == NULL ) return 1;
    
    atomic_init(&muninn_worker->running,false);

    muninn_worker->name = name;
    muninn_worker->context = context;
    muninn_worker->thread_loop = thread_loop;
    muninn_worker->thread_stop = thread_stop;
    muninn_worker->thread_post = thread_perform;

    return 0;
}

int mw_start(muninn_worker_t *muninn_worker)
{
    if(muninn_worker == NULL) return 1;

    int rc = pthread_create(
        &muninn_worker->th,
        NULL,
        muninn_worker->thread_loop,
        muninn_worker->context
    );

    if(rc != 0)
    {
        fprintf(stderr,"Error starting thread '%s'\n",muninn_worker->name);
    }
    return rc;
}

int mw_stop(muninn_worker_t *muninn_worker)
{
    if(muninn_worker == NULL) return -1;
    
    if(atomic_load(&muninn_worker->running)== false) return 1;

    muninn_worker->thread_stop(muninn_worker->context);
    return 0;
}

int mw_join(muninn_worker_t *muninn_worker)
{
    if(muninn_worker == NULL) return 1;
    
    if(atomic_load(&muninn_worker->running)== false) return 1;

    int rc = pthread_join(muninn_worker->th,NULL);
    if( rc != 0)
    {
        fprintf(stderr,"Error mw_join thread '%s'\n",muninn_worker->name);
    }
    return rc;
}

int mw_shutdown(muninn_worker_t *muninn_worker)
{
    if(muninn_worker == NULL) return 1;

    mw_stop(muninn_worker);
    mw_join(muninn_worker);

    return 0;
}

int mw_post(muninn_worker_t *muninn_worker, void *data)
{
    if(muninn_worker == NULL) return 1;

    muninn_worker->thread_post(muninn_worker->context,data);

    return 0;
}
