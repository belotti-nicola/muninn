#include <internal/logger_th.h>
#include <unistd.h>
#include <string.h>

#define RB_SIZE 100
#define TESTPATH "test_log_th_routine.txt"

int main()
{
    setbuf(stderr, NULL);
    setbuf(stdout, NULL);
    remove(TESTPATH);

    char buffer[RB_SIZE]; 
    ts_ring_buffer_t rb = {0};
    ts_rb_setup(&rb,(uint8_t *)buffer,RB_SIZE);

    logger_th_data data;
    data.compress_q = NULL; //unused in this test
    data.ringbuffer = &rb;
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
        printf("Error at %s:%d (fgets failed)\n",__FILE__,__LINE__);
        return 1;
    }
    if(strstr(tmp, "hello") == NULL )
    {
        printf("Error: string log(%s) differs from expected(%s) at line %d.\n",tmp,"hello",__LINE__);
        return 1;
    }

    if (fgets(tmp, sizeof(tmp), file) == NULL)
    {
        printf("Error: fgets failed at line %d.\n", __LINE__);
        return 1;
    }

    if(strstr(tmp, "world") == NULL )
    {
        printf("Error: string log(%s) differs from expected(%s) at line %d.\n",tmp,"world",__LINE__);
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