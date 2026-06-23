#include "muninn.h"

#include <assert.h>
#include <string.h>
#include <unistd.h>

#define LOG "test_th_api.log"

int main(void)
{
    CONFIG conf = {0};
    
    muninn_t muninn = {0};

    muninn_init(&muninn,conf);
    muninn_shutdown(&muninn);
    
    return 0;
}