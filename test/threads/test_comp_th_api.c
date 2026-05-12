#include <internal/compressor_th.h>
#include <unistd.h>

int main()
{
    compressor_th_data data;
    ts_queue_t q;
    data.tasks = &q;
    
    int rc;
    rc = compressor_th_start(&data);
    if ( rc != 0)
    {
        printf("Error: compressor_th_start");
        return 1;
    }
    
    sleep(1);
    
    rc = compressor_th_stop(&data);
    if ( rc != 0)
    {
        printf("Error: compressor_th_stop");
        return 1;
    }

    rc = compressor_th_join(&data);
    if ( rc != 0)
    {
        printf("Error: compressor_th_stop");
        return 1;
    }
    return 0;
}