#ifndef __MVAS_PARALLEL_THREADS__
#define __MVAS_PARALLEL_THREADS__

#include <pthread.h>

#include <mvas/vas.h>

#include "common.h"

typedef struct _vasInfo
{
  char * name;
  mode_t permissions;
  vasid_t id;
}vasInfo;

/**
* Kernel function responsible to calculate just a small part of dot product
*/
extern void * mvasMatrixVectorKernel(void * pId);

extern vasInfo * mvasPoolInit(const int pTotalOfVas);
extern void cleanMvas(vasInfo * pVasInfo, const int pTotalVas);

/**
* Basically initialize threads data structure
* @param pTotalThreads Total of threads to be created
* @return Return a reference to array of pthread_t
*/
extern pthread_t * initMvasThreadData(const int pTotalThreads);

/**
* Start matrix multiplication
* @param pTotalThreads
* @param pInfo
*/
extern dataTime mvasMatrixVectorThread(const int pTotalThreads, dotProductData * pInfo);

#endif
