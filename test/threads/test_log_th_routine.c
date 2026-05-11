#include <internal/logger_th.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>


#define TESTPATH "test_log.txt"

int main()
{
    ts_queue_t q;ts_queue_setup(&q);
    logger_th_data data;
    
    data.queue = q;
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

    bool res;
    char buffer[256];
    res = fgets(buffer, sizeof(buffer), file);
    assert(res != NULL);    
    assert(strncmp(buffer, "hello", 5) == 0);

    res = fgets(buffer, sizeof(buffer), file);
    assert(res != NULL);    
    assert(strncmp(buffer, "world", 5) == 0);

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