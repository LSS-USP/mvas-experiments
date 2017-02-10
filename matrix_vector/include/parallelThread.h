#ifndef __PARALLEL_THREADS__
#define __PARALLEL_THREADS__

#include <pthread.h>

#include "common.h"

/**
* Kernel function responsible to calculate just a small part of dot product
*/
extern void * matrixVectorKernel(void * pId);

/**
* Basically initialize threads data structure
* @param pTotalThreads Total of threads to be created
* @return Return a reference to array of pthread_t
*/
extern pthread_t * initThreadData(int pTotalThreads);

/**
* Start matrix multiplication
* @param pTotalThreads
* @param pInfo
*/
extern dataTime matrixVectorThread(int pTotalThreads, dotProductData * pInfo);

#endif
