#ifndef MUNINN_MESSAGE_MASK_H
#define MUNINN_MESSAGE_MASK_H

typedef enum muninn_message_mask
{
    MEDM_NONE              = 0,

    MEDM_FILE              = 1 << 0, //  1
    MEDM_THREAD            = 1 << 1, //  2
    MEDM_TIMESTAMP         = 1 << 2, //  4
    MEDM_SEVERITY          = 1 << 3, //  8
    MEDM_MESSAGE           = 1 << 4, // 16
    MEDM_LINE              = 1 << 5, // 32
    MEDM_FUNCTION          = 1 << 6, // 64
    MEDM_PID               = 1 << 7, //128

    MEDM_RUNTIMESTAMP      = 1 << 8, //256

    MEDM_ALL               = 0xFFFFFFFF 

} muninn_message_mask;

const char *mmm_edm_to_string(muninn_message_mask edm);
muninn_message_mask mmm_string_to_edm(char *edm);

#endif