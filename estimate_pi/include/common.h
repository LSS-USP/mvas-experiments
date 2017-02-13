#ifndef __COMMON__
#define __COMMON__

#include <pthread.h>

#define BUFFER 4096

typedef struct _piData
{
  unsigned long terms;
  double pi;
  double sum;
  pthread_mutex_t mutex;
} piData;

extern void dumpEstimatePi(const piData * pInfo, const char * pBaseName);

#endif
