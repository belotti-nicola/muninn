#include <internal/logger_th.h>
#include <unistd.h>

int main()
{
    logger_th_data data;
    ts_queue_t q;
    data.queue = q;
    
    int rc;
    rc = logger_th_start(&data);
    if ( rc != 0)
    {
        printf("Error: logger_th_start");
        return 1;
    }
    
    sleep(1);
    
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