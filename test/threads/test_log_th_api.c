#include <internal/logger_th.h>
#include <unistd.h>

#include <string.h>

#define RB_SIZE 1000
#define TESTPATH "test_log_th_api.log"

int main()
{
    char buffer[RB_SIZE]; 
    ts_ring_buffer_t rb = {0};
    ts_rb_setup(&rb,buffer,RB_SIZE);


    logger_th_data data;
    data.ringbuffer = &rb;
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