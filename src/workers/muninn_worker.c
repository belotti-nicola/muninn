#include <internal/muninn_worker.h>
#include <stdio.h>

//this function takes care of the atomic_boolean
static void* mw_internal_runner(void *arg)
{
    muninn_worker_t *w = (muninn_worker_t *)arg;

    atomic_store(&w->running, true);
    void *ret = w->thread_loop(w->context);
    atomic_store(&w->running, false);

    return ret;
}


int mw_init(muninn_worker_t *muninn_worker, const char *name, 
    void* (*thread_loop)(void*),
    void* (*thread_stop)(void*),
    void* (*thread_perform)(void *arg, void *data),
    void *context)
{
    if(muninn_worker == NULL || name == NULL ) return 1;
    
    atomic_init(&muninn_worker->running,false);

    muninn_worker->name = name;
    muninn_worker->thread_loop = thread_loop;
    muninn_worker->thread_stop = thread_stop;
    muninn_worker->thread_post = thread_perform;
    muninn_worker->context = context;

    return 0;
}

int mw_start(muninn_worker_t *muninn_worker)
{
    if(muninn_worker == NULL) return 1;

    int rc = pthread_create(
        &muninn_worker->th,
        NULL,
        mw_internal_runner,
        muninn_worker
    );

    if(rc != 0)
    {
        fprintf(stderr,"Error starting thread '%s'\n",muninn_worker->name);
        return -1;
    }

    atomic_store(&muninn_worker->running,true);
    return 0;
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

bool mw_running(muninn_worker_t *muninn_worker)
{
    if(muninn_worker == NULL) return false;
    return atomic_load(&muninn_worker->running);
}