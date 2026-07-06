#ifndef ENCODED_DATA_MASK_H
#define ENCODED_DATA_MASK_H

typedef enum ENCONDED_DATA_MASK
{
    MEDM_NONE       = 0,

    MEDM_FILE       = 1 << 0,
    MEDM_THREAD     = 1 << 1,
    MEDM_TIMESTAMP  = 1 << 2,
    MEDM_SEVERITY   = 1 << 3,
    MEDM_MESSAGE    = 1 << 4,
    MEDM_LINE       = 1 << 5,
    MEDM_FUNCTION   = 1 << 6,
    MEDM_PID        = 1 << 7,

    MEDM_RUNTS      = 1 << 8, 

    MEDM_ALL        = 0xFFFFFFFF 

} ENCONDED_DATA_MASK;


#endif