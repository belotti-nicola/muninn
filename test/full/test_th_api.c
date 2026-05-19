#include "muninn.h"

#include <assert.h>
#include <string.h>
#include <unistd.h>

#define LOG "test_th_api.log"

int main(void)
{
    muninn_t muninn = {0};

    muninn_init(&muninn,LOG);
    muninn_shutdown(&muninn);
    
    return 0;
}