#include "macro_based_q.h"
#include <stdio.h>

GENERATE_QUEUE(Q1,1024,M1,96); 
GENERATE_QUEUE(Q2,128,M2,15);
  

void set_m1(M1 *m1, const char *s)
{
    // code here
}

void push_q1(Q1 *q1, const char *s)
{
   // code here
}

/*
 *  THIS APPROACH GENERATES QUEUES WITH MACROS
 */


int main()
{
    Q1 q1;Q2 q2;

    push_q1(&q1,"asdas");

    /*
     * UNCOMMENT THIS FOR SYNTAX ERROR!
    push_q2(&q2,"asdas"); 
     *
    */
    printf("Size q1 %ld, size q2 %ld\n",sizeof(q1),sizeof(q2));
    return 0;
}