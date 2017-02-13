#include <stdio.h>
#include <stdlib.h>

#include "dataTime.h"
#include "serial.h"
#include "common.h"

int main(int argc, char * argv[])
{
  if (argc < 2)
  {
    printf ("Invalid parameters, inform total terms\n");
    return -1;
  }

  unsigned long totalTerms = atol(argv[1]);
  if (totalTerms <= 0)
  {
    printf("Total terms should be >= 1\n");
    return -1;
  }

  piData data;
  // Serial
  data.terms = totalTerms;
  dataTime serialTime;
  serialTime = estimatePiSerial(&data);
  calculateElapsedTime(&serialTime);
  dumpElapsedTime(&serialTime, "serial");
  dumpEstimatePi(&data, "serial");
  // Parallel

  return 0;
}
