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
    message m1;
    char m1_buffer[M1_SIZE];
    setup_message(&m1,m1_buffer,M1_SIZE);
    queue q1;
    setup_queue(&q1,&m1,M1_SIZE);

    message m2;
    char m2_buffer[M2_SIZE];
    setup_message(&m2,m2_buffer,M2_SIZE); 
    queue q2;
    setup_queue(&q2,&m2,M2_SIZE);


    return 0;
}