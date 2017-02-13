#include <stdio.h>
#include <stdlib.h>

#include "dataTime.h"
#include "common.h"
#include "parallelThread.h"

static int gTotalThreads = 0;
static piData * infoThread = NULL;

dataTime estimatePiThread(int pTotalThreads, piData * pInfo)
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
    return threadTime;
  }
  int status = 0;
  infoThread = pInfo;
  pInfo->sum = 0.0;
  if (pthread_mutex_init(&pInfo->mutex, NULL))
  {
    printf("Error on mutex init\n");
    free(threadsData);
    return threadTime;
  }

  getBeginTime(&threadTime);
  for (int thread = 0; thread < gTotalThreads; thread++)
  {
    long * argument = malloc(sizeof(*argument));
    if (!argument)
    {
      printf("Error to allocate argument\n");
    }
    *argument = thread;
    status = pthread_create(&threadsData[thread], NULL, estimatePiKernel,
                            argument);
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
  pthread_mutex_destroy(&pInfo->mutex);
  return threadTime;
}

pthread_t * initThreadData(int pTotalThreads)
{
  if (pTotalThreads < 1)
  {
    printf("It is required at least one thread\n");
    return NULL;
  }

  pthread_t * threadData = (pthread_t *) calloc(pTotalThreads,
                                                sizeof(pthread_t));
  if (!threadData)
  {
    printf("Problem to allocate pthread_t\n");
    return NULL;
  }
  gTotalThreads = pTotalThreads;
  return threadData;
}

void * estimatePiKernel(void * pId)
{
  long localId = *((long*)pId);
  free(pId);
  double factor = 0.0;
  long long i = 0;
  long long localToProcess = infoThread->terms / gTotalThreads;
  long long start = localToProcess * localId;
  long long last = start + localToProcess;
  double localSum = 0.0;

  if (start % 2 == 0)
  {
    factor = 1.0;
  }
  else
  {
    factor = -1.0;
  }

  for (i = start; i < last; i++, factor = -factor)
  {
    localSum += factor / (2 * i + 1);
  }

  pthread_mutex_lock(&infoThread->mutex);
  infoThread->sum += localSum;
  pthread_mutex_unlock(&infoThread->mutex);

  return NULL;
}
