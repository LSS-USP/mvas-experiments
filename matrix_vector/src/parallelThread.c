#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

dataTime matrixVectorThread(int pTotalThreads, dotProductData * pInfo)
{
  dataTime threadTime;
  if (!pInfo || pTotalThreads < 1)
  {
    printf("Invalid parameters\n");
    return threadTime;
  }
  pthread_t * threadsData = initThreadData(pTotalThreads);
  if (!threadsData)
  {
    printf("matrixVectorThread cannot allocate data structure\n");
    return threadTime;
  }

  int status = 0;
  infoThread = pInfo;
  getBeginTime(&threadTime);
  for (int thread = 0; thread < gTotalThreads; thread++)
  {
    long * argument = malloc(sizeof(*argument));
    if (!argument)
    {
      printf("Error to allocate argument\n");
    }
    *argument = thread;
    status = pthread_create(&threadsData[thread], NULL,
                            matrixVectorKernel, argument);
    if (status)
    {
      printf("Error when tried to create thread: %d\n", thread);
      continue;
    }
  }


  for (int thread = 0; thread < gTotalThreads; thread++)
  {
    status = pthread_join(threadsData[thread], NULL);
    if (status)
    {
      printf("Error when tried to join thread: %d\n", thread);
      continue;
    }
  }
  getEndTime(&threadTime);

  free(threadsData);
  return threadTime;
}

void * matrixVectorKernel(void * pId)
{
  long localId = *((long*)pId);
  free(pId);
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
