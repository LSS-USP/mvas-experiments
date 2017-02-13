#include <stdio.h>
#include <time.h>
#include <string.h>

#include "dataTime.h"

void getBeginTime(dataTime * pDataTime)
{
  clock_gettime(CLOCK_MONOTONIC_RAW, &pDataTime->beginMonotonicRaw);
  pDataTime->beginClock = clock();
}

void getEndTime(dataTime * pDataTime)
{
  clock_gettime(CLOCK_MONOTONIC_RAW, &pDataTime->endMonotonicRaw);
  pDataTime->endClock = clock();
}

void calculateElapsedTime(dataTime * pDataTime)
{
  pDataTime->elapsedClock = (((double)pDataTime->endClock -
                              pDataTime->beginClock) / (CLOCKS_PER_SEC)) *
                              SECONDS_TO_MILLISECONDS;
  pDataTime->elapsedMonotonicRaw =
          (pDataTime->endMonotonicRaw.tv_sec -
           pDataTime->beginMonotonicRaw.tv_sec ) +
          (pDataTime->endMonotonicRaw.tv_nsec -
          pDataTime->beginMonotonicRaw.tv_nsec)
          / BILLION;
}

void dumpElapsedTime(const dataTime * pDataTime, char * pPrefix)
{
  printf("Elapsed time (MONOTONIC): %lf (s)\n", pDataTime->elapsedMonotonicRaw);
  printf("Elapsed time (CLOCK): %lf (ms)\n", pDataTime->elapsedClock);
  char saveTo[DATA_TIME_BUFFER];
  char output[DATA_TIME_BUFFER];
  strcpy(saveTo, pPrefix);
  strcat(saveTo, "_elapsedTime.txt");

  FILE * elapsedTimeFile = fopen(saveTo, "a+");
  if (!elapsedTimeFile)
  {
    printf("Error to write information file\n");
    return;
  }
  sprintf(output, "Elapsed time (MONOTONIC): %lf (s)\n",
          pDataTime->elapsedMonotonicRaw);
  fputs(output, elapsedTimeFile);
  sprintf(output, "Elapsed time (CLOCK): %lf (ms)\n",
          pDataTime->elapsedClock);
  fputs(output, elapsedTimeFile);
  fclose(elapsedTimeFile);
}
