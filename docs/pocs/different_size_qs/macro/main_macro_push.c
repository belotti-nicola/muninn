#include "macro_based_q.h"

#define GENERATE_QUEUE_PUSH(Q_Name, Q_Size, M_Name, M_Size)        \
    void push_##Q_Name(Q_Name *q, const char *s) {               \
        if (q->head < Q_Size) {                                  \
            strncpy(q->items[q->head].data, s, M_Size - 1);   \
            q->items[q->head].data[M_Size - 1] = '\0';        \
            q->head++;                                           \
        }                                                        \
    }


#define GENERATE_QUEUE_WITH_PUSH(Q_Name, Q_Size, M_Name, M_Size)            \
    GENERATE_QUEUE(Q_Name, Q_Size, M_Name, M_Size);         \
    GENERATE_QUEUE_PUSH(Q_Name, Q_Size, M_Name, M_Size);         

GENERATE_QUEUE_WITH_PUSH(Q1,1024,M1,96)
GENERATE_QUEUE_WITH_PUSH(Q2,128,M2,15)

int main()
{
    Q1 q1;Q2 q2;

    push_Q1(&q1,"Hello");
    push_Q2(&q2,"World");

    return 0;
}