#include <stdio.h>

#include "dataTime.h"
#include "common.h"
#include "serial.h"

dataTime estimatePiSerial(piData * pInfo)
{
  double factor = 1.0;
  double sum = 0.0;
  dataTime serialTime;
  if(!pInfo)
  {
    printf("Invalid parameter\n");
    return serialTime;
  }

  getBeginTime(&serialTime);
  for (int i = 0; i < pInfo->terms; i++, factor = -factor)
  {
    sum += factor/(2 * i + 1);
  }
  pInfo->pi = 4.0 * sum;
  getEndTime(&serialTime);

  return serialTime;
}
