#ifndef __DATA_TIME__
#define __DATA_TIME__

#include <time.h>

#define DATA_TIME_BUFFER 4096
#define SECONDS_TO_MILLISECONDS 1000
#define BILLION 1E9

typedef struct _dataTime
{
  struct timespec beginMonotonicRaw;
  struct timespec endMonotonicRaw;
  clock_t beginClock;
  clock_t endClock;
  double elapsedMonotonicRaw;
  double elapsedClock;
} dataTime;

/**
* Fill out informations about start measure in dataTime structure
* @param pDataTime Data time information
*/
extern void getBeginTime(dataTime * pDataTime);

/**
* With this function, all informations related to stop is saved.
* @param pDataTime data structure with time information
* @see dataTime
*/
extern void getEndTime(dataTime * pDataTime);

/**
* Show and save information collected on dataTime structure
* @param pDataTime Parameter with time information
* @param pPrefix
*/
extern void dumpElapsedTime(const dataTime * pDataTime, char * pPrefix);

/**
* Based on dataTime informations, calculate the elapsed time
* @param pDataTime data structure with all time information
* @see dataTime
*/
extern void calculateElapsedTime(dataTime * pDataTime);

#endif
