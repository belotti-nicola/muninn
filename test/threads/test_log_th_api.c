#include <internal/logger_th.h>
#include <unistd.h>

#include <string.h>

#define MESSAGE_SIZE 100
#define QUEUE_SIZE 5
#define TESTPATH "test_log_th_api.log"

int main()
{
    int offset = 0;
    char buffer[MESSAGE_SIZE * QUEUE_SIZE]; 
    queue_message_t messages[QUEUE_SIZE] = {0};
    for(int i=0;i<QUEUE_SIZE;i++)
    {
        setup_queue_message(messages+i,buffer+offset,MESSAGE_SIZE);
        offset += MESSAGE_SIZE;
    }

    ts_queue_t tsq;
    ts_queue_setup(&tsq,messages,QUEUE_SIZE);


    logger_th_data data;
    data.queue = &tsq;
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