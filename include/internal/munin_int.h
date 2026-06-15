#ifndef MUNINN_INT_H
#define MUNINN_INT_H


#define LOG_RB_SIZE            1024*1024            // DATA AVAILABLE IN LOGGER RINGBUFFEER
#define COMP_QUEUE_SIZE        8                    // MAXIMUM QUEUE SIZE (ELEMENTS IN THE Q)
#define COMP_MESSAGE_SIZE      256                  // MAXIMUM MESSAGE LENGTH (EACH ELEMENT IS LONG AT MOST)
#define P_SIZE                 COMP_MESSAGE_SIZE    // MAXIMUM LOG PATH SIZE

#define F_MAX_SIZE             1024*1024            // MAXIMUM FILE SIZE BEFORE COMPRESSION

#define MUNINN_DUMP  "muninn_panic.dump"


typedef enum MUNINN_COLORS
{
    COLOR_BLACK,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_MAGENTA,
    COLOR_CYAN,
    COLOR_WHITE,

    COLOR_RESET,
    __COLOR_MAX__
    
} MUNINN_COLORS;

#endif