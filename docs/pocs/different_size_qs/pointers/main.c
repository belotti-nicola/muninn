#include "queue_based_on_pointers.h"
#include <stdio.h>


/*  FIRST QUEUE */
#define Q1_SIZE 32
#define M1_SIZE 96

/* SECOND QUEUE */
#define Q2_SIZE 10
#define M2_SIZE 24

int main()
{
    int offset = 0;
    char m1_buffer[M1_SIZE * Q1_SIZE];
    char m2_buffer[M2_SIZE * Q2_SIZE];

    message m1[Q1_SIZE];
    offset = 0;    
    for(int i=0;i<Q1_SIZE;i++)
    {
        setup_message(&m1[i],m1_buffer+offset,M1_SIZE);
        offset += M1_SIZE;
    }
    queue q1;
    setup_queue(&q1,m1,Q1_SIZE);

    message m2[Q2_SIZE];
    offset = 0;
    for(int i=0;i<Q2_SIZE;i++)
    {
        setup_message(&m2[i],m2_buffer+offset,M2_SIZE);
        offset += M2_SIZE;
    }
    queue q2;
    setup_queue(&q2,m2,Q2_SIZE);


    return 0;
}