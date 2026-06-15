#ifndef USAGE_FLAGS_H
#define USAGE_FLAGS_H

#include <stdint.h>

//used 
typedef uint32_t muninn_flags_t;//MAX 32


typedef enum muninn_flags_int_t 
{
    MU_FILE                  = (1 << 0),  // 0x01
    MU_CONSOLE               = (1 << 1),  // 0x02
    // MU_NETWORK_UDP
    // MU_SYSLOG

    // --- METADATA FORMATTING ---
    MU_COLORED_CONSOLE       = (1 << 2),     // 0x04

    //MU_WITH_TIMESTAMP   = (1 << 4),0x10
    //MU_WITH_THREAD_ID   = (1 << 5),0x20
    //MU_WITH_SOURCE_LOC  = (1 << 6),0x40
    //MU_FORMAT_JSON      = (1 << 7),0X80

    // --- BEHAVIOR & SAFETY (Bit 8-10) ---
    //MU_FORCE_SYNC       = (1 << 8),  // 0x0100 
    //MU_BLOCK_IF_FULL    = (1 << 9),  // 0x0200 
    //MU_DROP_IF_FULL     = (1 << 10), // 0x0400 

    //AT MOST typedef uint32_t muninn_flags_t;

} muninn_flags_int_t;





#endif

