#include <internal/logger_th.h>
#include <unistd.h>

#include <string.h>

#define TESTPATH "test_log_th_api.log"

int main()
{
    logger_th_data data;
    ts_queue_t q;ts_queue_setup(&q);
    data.queue = &q;
    strcpy(data.path,TESTPATH);
    
    int rc;
    rc = logger_th_start(&data);
    if ( rc != 0)
    {
        printf("Error: logger_th_start");
        return 1;
    }
        
    rc = logger_th_stop(&data);
    if ( rc != 0)
    {
        printf("Error: logger_th_stop");
        return 1;
    }

    rc = logger_th_join(&data);
    if ( rc != 0)
    {
        printf("Error: logger_th_stop");
        return 1;
    }
    return 0;
}