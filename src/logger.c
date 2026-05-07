#include "log_queue.h"
#include <string.h>


#include "logger.h"

muninn_t muninn_create(const char *path, size_t max_size)
{
    muninn_t retVal;

    size_t dim = strlen(path);
    memcpy(retVal.path,path,dim);
    retVal.rotate_size = max_size;

    return retVal;
}

void muninn_start(muninn_t muninn)
{
    pthread_t th;
    int exit_code = pthread_create(&th,NULL,muninn_thread_function,NULL);
    if ( exit_code )
    {
        muninn.thread = th;
    }

}

void muninn_log(muninn_t muninn,const char *msg)
{

}

void muninn_thread_function(void *arg)
{
    
}

