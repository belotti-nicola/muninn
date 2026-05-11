#include <internal/compressor_th.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>


#define TESTPATH "test.txt"

int zst_files_counter()
{
    DIR* dir = opendir(".");
    if (!dir) return -1;

    struct dirent* entry;
    int count = 0;

    while ((entry = readdir(dir)) != NULL) {
        const char* name = entry->d_name;
        size_t len = strlen(name);

        if (len > 4 && strcmp(name + len - 4, ".zst") == 0) {
            count++;
        }
    }

    closedir(dir);
    return count;
}

int main()
{
    ts_queue_t q;ts_queue_setup(&q);
    compressor_th_data data;data.tasks = q;
    
    int rc;
    compressor_th_start(&data);
    

    FILE* f = fopen(TESTPATH,"w");
    if(f == NULL)
    {
        printf("Error: cannot open file %s\n",TESTPATH);
        return 1;
    }
    fclose(f);
    
    compressor_th_perform(&data,TESTPATH);

    compressor_th_stop(&data);
    compressor_th_join(&data);

    int counter = zst_files_counter();
    if( counter != 1)
    {
        printf("Error: file counter(%d) differs from expected(%d)\n",counter,1);
        return 1;
    }
    
    return 0;
}