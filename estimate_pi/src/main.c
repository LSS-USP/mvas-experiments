#include <stdio.h>
#include <stdlib.h>

#include "dataTime.h"
#include "serial.h"
#include "parallelThread.h"
#include "common.h"

int main(int argc, char * argv[])
{
  if (argc < 2)
  {
    printf ("Invalid parameters, inform total terms\n");
    return -1;
  }

  unsigned long totalTerms = atol(argv[1]);
  int totalThreads = atoi(argv[2]);
  if (totalTerms <= 0)
  {
    printf("Total terms should be >= 1\n");
    return -1;
  }

  piData data;
  data.terms = totalTerms;
  // Serial
  dataTime serialTime;
  printf("SERIAL\n");
  serialTime = estimatePiSerial(&data);
  calculateElapsedTime(&serialTime);
  dumpElapsedTime(&serialTime, "serial");
  dumpEstimatePi(&data, "serial");
  // Parallel
  printf("Parallel\n");
  dataTime parallelTime;
  parallelTime = estimatePiThread(totalThreads, &data);
  calculateElapsedTime(&parallelTime);
  dumpElapsedTime(&parallelTime, "parallel");
  dumpEstimatePi(&data, "parallel");

  return 0;
}
