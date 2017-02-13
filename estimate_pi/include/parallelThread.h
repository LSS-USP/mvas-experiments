#ifndef __PARALLEL__
#define __PARALLEL__

#include <pthread.h>

#include "common.h"

/**
* Initialize threads and start computing pi value
* @param pTotalThreads Total of thread to be created
* @param pInfo data structure with required informations to calculate PI value
* @return Return dataTime with start and finish time
**/
extern dataTime estimatePiThread(int pTotalThreads, piData * pInfo);

/**
* Kernel function used on pthread_create
* @param pId Thread id
*/
extern void * estimatePiKernel(void * pId);

/**
* Initialized thread data
* @param pTotalThreads Total of threads that should be created
* @return Return an array of pthread_t
*/
extern pthread_t * initThreadData(int pTotalThreads);

#endif
