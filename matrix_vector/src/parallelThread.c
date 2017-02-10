#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#include "common.h"
#include "parallelThread.h"

// Matrix data
static int gTotalThreads = 0;
static dotProductData * infoThread = NULL;

pthread_t * initThreadData(int pTotalThreads)
{
  if (pTotalThreads < 1)
  {
    printf("It is required at least one thread\n");
    return NULL;
  }
  pthread_t * threadData = (pthread_t *)calloc(pTotalThreads,
                                               sizeof(pthread_t));
  if (!threadData)
  {
    printf("Problem to allocate pthread_t\n");
    return NULL;
  }
  gTotalThreads = pTotalThreads;
  return threadData;
}

void matrixVectorThread(int pTotalThreads, dotProductData * pInfo)
{
  if (!pInfo || pTotalThreads < 1)
  {
    printf("Invalid parameters\n");
    return;
  }
  pthread_t * threadData = initThreadData(pTotalThreads);
  if (!threadData)
  {
    printf("matrixVectorThread cannot allocate data structure\n");
    return;
  }

  int status = 0;
  infoThread = pInfo;
  for (int thread = 0; thread < gTotalThreads; thread++)
  {
    status = pthread_create(&threadData[thread], NULL,
                            matrixVectorKernel, (void*)&thread);
    if (status)
    {
      printf("Error when tried to create thread: %d\n", thread);
      continue;
    }
  }

  for (int thread = 0; thread < gTotalThreads; thread++)
  {
    status = pthread_join(threadData[thread], NULL);
    if (status)
    {
      printf("Error when tried to join thread: %d\n", thread);
      continue;
    }
  }

  free(threadData);
  return;
}

void * matrixVectorKernel(void * pId)
{
  long localId = *((long*)pId);
  int localToProcess = infoThread->lines / gTotalThreads;
  int startLine = localId * localToProcess;
  int endLine = (localId + 1) * localToProcess - 1;

  long tempElement = 0;
  for (int line = startLine; line <= endLine; line++)
  {
    infoThread->finalVector[line] = 0.0;
    for (int column = 0; column < infoThread->columns; column++)
    {
      tempElement = *((infoThread->matrix + line*infoThread->lines) + column);
      infoThread->finalVector[line] += tempElement * infoThread->vector[column];
    }
  }

  return NULL;
}
