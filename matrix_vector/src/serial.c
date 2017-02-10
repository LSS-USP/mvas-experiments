#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "serial.h"

dataTime matrixVectorSerial(dotProductData * pInfo)
{
  dataTime serialTime;
  if (!pInfo)
  {
    printf("Invalid parameters\n");
    return serialTime;
  }

  long tmpElement = 0;

  getBeginTime(&serialTime);
  for (int line = 0; line < pInfo->lines; line++)
  {
    pInfo->finalVector[line] = 0.0;
    for(int column = 0; column < pInfo->columns; column++)
    {
      tmpElement = *((pInfo->matrix + line * pInfo->lines) + column);
      pInfo->finalVector[line] +=  tmpElement * pInfo->vector[column];
    }
  }
  getEndTime(&serialTime);
  return serialTime;
}
