#include <internal/flogger_th.h>
#include <internal/queue_message.h>
#include <internal/ts_queue.h>
#include <unistd.h>
#include <muninn.h>

#include <string.h>

#define TESTPATH "test_log_th_api.log"
#define MESSAGE_SIZE 100
#define QUEUE_SIZE 5


int main()
{
    muninn_t muninn;
    muninn_init(&muninn,TESTPATH);
       
    flogger_th_stop(&muninn.flogger);
    flogger_th_join(&muninn.flogger);
    
    return 0;
}