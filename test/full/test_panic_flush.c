#include "muninn.h"
#include "internal/ts_ring_buffer.h"
#include "internal/ring_buffer.h"
#include <string.h>

#include "test_utils.h"

int main()
{
    muninn_t muninn = {0};
    atomic_store(&muninn.running,true);
    
    ring_buffer *rb = &muninn.gateway_rb.ring_buffer;

    const char tmp[] = "Hello World";
    rb_push(rb,tmp,strlen(tmp));

    muninn_panic_flush(&muninn);

    if(!file_exists(MUNINN_DUMP))
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("File %s was not created!\n",MUNINN_DUMP);
        return 1;
    }

    return 0;
}