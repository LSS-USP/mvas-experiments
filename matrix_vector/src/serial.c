#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "serial.h"

void matrixVectorSerial(dotProductData * pInfo)
{
  if (!pInfo)
  {
    printf("Invalid parameters\n");
    return;
  }

  long tmpElement = 0;
  for (int line = 0; line < pInfo->lines; line++)
  {
    pInfo->finalVector[line] = 0.0;
    for(int column = 0; column < pInfo->columns; column++)
    {
      tmpElement = *((pInfo->matrix + line * pInfo->lines) + column);
      pInfo->finalVector[line] +=  tmpElement * pInfo->vector[column];
    }
  }
}
