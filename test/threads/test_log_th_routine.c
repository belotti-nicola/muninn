#include <internal/logger_th.h>
#include <unistd.h>
#include <string.h>


#define TESTPATH "test_log_th_routine.txt"

int main()
{
    logger_th_data data;
    ts_queue_t q;ts_queue_setup(&q);
    data.queue = &q;
    strcpy(data.path,TESTPATH);
    
    logger_th_start(&data);
    logger_th_perform(&data,"hello");
    logger_th_perform(&data,"world");
    logger_th_stop(&data);
    logger_th_join(&data);

    FILE *file = fopen(TESTPATH,"r");
    if(file == NULL)
    {
        printf("Error: could not check test file %s,\n",TESTPATH);
        return 1;
    }


    char buffer[128];
    if (fgets(buffer, sizeof(buffer), file) == NULL)
    {
        printf("Error: fgets failed at line %d.\n", __LINE__);
        return 1;
    }
    if(strncmp(buffer, "hello", 5) != 0)
    {
        printf("Error: string log(%s) differs from expected(%s) at line %d.\n",buffer,"hello",__LINE__);
        return 1;
    }

    fgets(buffer, sizeof(buffer), file);
    if(buffer == NULL)
    {
        printf("Error: char pointer is null at line %d.\n",__LINE__);
        return 1;
    }
    if(strncmp(buffer, "world", 5) != 0)
    {
       printf("Error: string log(%s) differs from expected(%s) at line %d.\n",buffer,"world",__LINE__);
        return 1;
    }

    if (fgets(buffer, sizeof(buffer), file) == NULL)
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