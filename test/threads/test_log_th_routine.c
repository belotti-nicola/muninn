#include <internal/flogger_th.h>
#include <unistd.h>
#include <string.h>

#include "test_utils.h"

#define RB_SIZE 100
#define PATH_SIZE 512

int main()
{
    char filePath[PATH_SIZE] = {0};
    if(compute_test_file_name(filePath,PATH_SIZE) == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Could not compute test file name with %d bytes!",PATH_SIZE);
        return 1;
    }

    setbuf(stderr, NULL);
    setbuf(stdout, NULL);
    remove(filePath);

    char buffer[RB_SIZE]; 
    ts_ring_buffer_t rb = {0};
    ts_rb_setup(&rb,(uint8_t *)buffer,RB_SIZE);

    logger_th_data data;
    data.compress_q = NULL; //unused in this test
    data.ringbuffer = &rb;
    strcpy(data.path,filePath);
   
    logger_th_start(&data);
    logger_th_perform(&data,LOG_INFO,"hello");
    logger_th_perform(&data,LOG_INFO,"world");
    
    logger_th_stop(&data);
    logger_th_join(&data);

    FILE *file = fopen(filePath,"r");
    if(file == NULL)
    {
        printf("Error: could not check test file %s,\n",filePath);
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