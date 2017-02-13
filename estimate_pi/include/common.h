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

/**
* Write final result of pi on STDOUT and file
* @param pInfo Data structure with information about pi
* @param pBaseName Base name to add in the final file output
*/
extern void dumpEstimatePi(const piData * pInfo, const char * pBaseName);

#endif
