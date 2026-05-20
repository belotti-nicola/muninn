#ifndef MUNINN_INT_H
#define MUNINN_INT_H


// =================================================================================================================
// =================================================================================================================
// ================================================== ATTENCTION ===================================================
//      Structures are based in the stack. 
//      Allocating Q_SIZE * M_SIZE bytes in the stack for logger queue (compressor queue is a lot of smaller). 
//      THIS CAN LEAD TO POTENTIAL STACKOVERFLOW CRASHES
//
#define    LOG_QUEUE_SIZE  1024  //MAXIMUM QUEUE SIZE
#define  LOG_MESSAGE_SIZE   512  //MAXIMUM MESSAGE SIZE
#define   COMP_QUEUE_SIZE     8  //MAXIMUM QUEUE SIZE
#define COMP_MESSAGE_SIZE   128  //MAXIMUM MESSAGE LENGTH
//
//
// =================================================================================================================
// =================================================================================================================
// =================================================================================================================

#define P_SIZE  COMP_MESSAGE_SIZE  //MAXIMUM LOG PATH SIZE

#define F_MAX_SIZE 1024*1024 //MAXIMUM FILE SIZE

void *muninn_thread_function(void *arg);

#endif