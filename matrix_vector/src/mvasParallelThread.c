#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>

#include <pthread.h>

#include <mvas/vas.h>

#include "common.h"
#include "mvasParallelThread.h"

// Matrix data
static int gTotalThreads = 0;
static dotProductData * infoThread = NULL;
static pid_t parentId = -1;

static int createVas(vasInfo * pVas)
{
  vasid_t vasId = 0;
  vasId = vas_find(pVas->name);
  if (vasId < 0)
  {
    vasId = vas_create(pVas->name, pVas->permissions);
    if (vasId < 0)
    {
      printf("Error on vas creation: %s\n", strerror(errno));
      return -1;
    }
  }
  pVas->id = vasId;
  return 0;
}

pthread_t * initMvasThreadData(int pTotalThreads)
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
  parentId = getpid();
  gTotalThreads = pTotalThreads;
  return threadData;
}

vasInfo * mvasPoolInit(const int pTotalOfVas)
{
  char name[64];

  // Notice parentId have to be set before
  if (pTotalOfVas < 0 || parentId < 0)
  {
    printf("Bad parameter\n");
    return NULL;
  }

  vasInfo * vasPool = malloc(sizeof(vasInfo) * pTotalOfVas);
  if (!vasPool)
  {
    printf("Cannot allocate memory for vas info.\n");
    return NULL;
  }

  for (int vasId = 0; vasId < pTotalOfVas; vasId++)
  {
    sprintf(name, "VAS%d", vasId);
    vasPool[vasId].permissions = S_IRWXU | S_IRWXG;
    vasPool[vasId].name = strdup(name);
    bzero(name, sizeof(name));

    if (createVas(&vasPool[vasId]) < 0)
    {
      printf("error\n");
      return NULL;
    }

  }
  //TODO: In the future, put attach to process here. See thread kernel with
  // the explanation of the current workaround running
  return vasPool;
}

void cleanMvas(vasInfo * pVasInfo, const int pTotalVas)
{
  int status = 0;
  for (int vas = 0; vas < pTotalVas; vas++)
  {
    status = vas_detach(parentId, pVasInfo[vas].id);
    if (status < 0)
    {
      printf("Error on detach VAS %d. %s\n", pVasInfo[vas].id, strerror(errno));
      continue;
    }
    status = vas_delete(pVasInfo[vas].id);
    if (status < 0)
    {
      printf("Error to delete VAS %d. %s\n", pVasInfo[vas].id, strerror(errno));
      continue;
    }
  }
  free(pVasInfo);
}

dataTime mvasMatrixVectorThread(const int pTotalThreads, dotProductData * pInfo)
{
  dataTime threadTime;
  int status = 0;
  vasInfo * poolOfVas = NULL;

  if (!pInfo || pTotalThreads < 1)
  {
    printf("Invalid parameters\n");
    return threadTime;
  }
  pthread_t * threadsData = initMvasThreadData(pTotalThreads);
  if (!threadsData)
  {
    printf("matrixVectorThread cannot allocate data structure\n");
    return threadTime;
  }

  poolOfVas = mvasPoolInit(pTotalThreads);
  if (!poolOfVas)
  {
    printf("Initialized MVAS pool");
    return threadTime;
  }

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
                            mvasMatrixVectorKernel, argument);
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
  cleanMvas(poolOfVas, gTotalThreads);

  free(threadsData);
  return threadTime;
}

void * mvasMatrixVectorKernel(void * pId)
{
  long localId = *((long*)pId);
  int vasId = (int)localId + 1;

  int status = 0;
  free(pId);
  // XXX: workaround
  // We should not need to sleep, in the future this gonna be correct.
  unsigned int timeThread = localId;
  long timeSleep = rand_r(&timeThread) % 999999999;
  const struct timespec timeInterval = {1, timeSleep};
  if (nanosleep(&timeInterval, NULL) < 0)
  {
    printf("Problem when try to sleep: %s\n", strerror(errno));
  }

  status = vas_attach(parentId, vasId, S_IRWXU | S_IRWXG);
  if (status < 0)
  {
    printf("Error on attach: %s\n", strerror(errno));
    return NULL;
  }
  // XXX: workaround

  status = vas_switch(vasId);
  if (status < 0)
  {
    printf("Thread kernel (%ld, %d): vas_switch %s\n", localId, vasId,
           strerror(errno));
    return NULL;
  }

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

  status = vas_switch(0);
  if (status < 0)
  {
    printf("Error with swith at the end. %s\n", strerror(errno));
    return NULL;
  }

  return NULL;
}
