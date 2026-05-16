#ifndef MACRO_BASED_QUEUE_H
#define MACRO_BASED_QUEUE_H

#include <string.h>
#include <stdlib.h>

#define GENERATE_QUEUE_MESSAGE(M_Name,M_Size) \
    typedef struct M_Name                  \
    {                                      \
        char message[M_Size];              \
                                           \
    } M_Name                               

#define GENERATE_QUEUE(Q_Name,Q_Size, M_Name, M_Size) \
    GENERATE_QUEUE_MESSAGE(M_Name,M_Size);                  \
    typedef struct Q_Name                                \
    {                                                    \
        M_Name items[Q_Size];                            \
        size_t  head;                                    \
    } Q_Name

   

#endif