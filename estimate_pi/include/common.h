#ifndef __COMMON__
#define __COMMON__

#define BUFFER 4096

typedef struct _piData
{
  unsigned long terms;
  double pi;
} piData;

extern void dumpEstimatePi(const piData * pInfo, const char * pBaseName);

#endif
