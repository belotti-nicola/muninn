#include <internal/logger_th.h>
#include <unistd.h>
#include <string.h>

#define MESSAGE_SIZE 100
#define QUEUE_SIZE 5
#define TESTPATH "test_log_th_routine.txt"

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

    ts_queue_t tsq  = {0};
    ts_queue_setup(&tsq,messages,QUEUE_SIZE);


    logger_th_data data = {0};
    data.queue = &tsq;
    strcpy(data.path,TESTPATH);
    
    logger_th_start(&data);
    logger_th_perform(&data,LOG_INFO,"hello");
    logger_th_perform(&data,LOG_INFO,"world");
    logger_th_stop(&data);
    logger_th_join(&data);

    FILE *file = fopen(TESTPATH,"r");
    if(file == NULL)
    {
        printf("Error: could not check test file %s,\n",TESTPATH);
        return 1;
    }


    char tmp[128];
    if (fgets(tmp, sizeof(tmp), file) == NULL)
    {
        printf("Error: fgets failed at line %d.\n", __LINE__);
        return 1;
    }
    if(strncmp(tmp, "hello", 5) != 0)
    {
        printf("Error: string log(%s) differs from expected(%s) at line %d.\n",tmp,"hello",__LINE__);
        return 1;
    }

    if (fgets(tmp, sizeof(tmp), file) == NULL)
    {
        printf("Error: fgets failed at line %d.\n", __LINE__);
        return 1;
    }
    if(buffer == NULL)
    {
        printf("Error: char pointer is null at line %d.\n",__LINE__);
        return 1;
    }
    if(strncmp(tmp, "world", 5) != 0)
    {
       printf("Error: string log(%s) differs from expected(%s) at line %d.\n",buffer,"world",__LINE__);
        return 1;
    }

    if (fgets(tmp, sizeof(tmp), file) == NULL)
    {
        if (feof(file))
        {
            
        }
        else if (ferror(file))
        {
            return 1;
        }
    }

    fclose(file);
    return 0;
}