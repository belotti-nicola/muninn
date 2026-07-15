#ifndef ENCODED_DATA_MASK_H
#define ENCODED_DATA_MASK_H

typedef enum ENCODED_DATA_MASK
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

} ENCODED_DATA_MASK;

const char *edm_to_string(ENCODED_DATA_MASK edm);
ENCODED_DATA_MASK string_to_edm(char *edm);

#endif