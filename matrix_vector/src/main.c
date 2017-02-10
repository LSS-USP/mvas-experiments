#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "serial.h"
#include "parallelThread.h"

int main(int argc, char * argv[])
{
  if (argc != 2)
  {
    printf("Error in the program execution\n");
    return -1;
  }

  int matrixCase = atoi(argv[1]);
  int matrixBase = optionMatrixSize(matrixCase);

  dotProductData * info = initDotProductData(matrixBase, matrixBase);
  if (!info)
  {
    printf("Error on initialization of dot product data structure\n");
    return -1;
  }

  // Serial
  printf("SERIAL\n");
  dataTime dataTimeSerial;
  dataTimeSerial = matrixVectorSerial(info);
  calculateElapsedTime(&dataTimeSerial);
  dumpDotProductData(info, "serial");
  dumpElapsedTime(&dataTimeSerial, "serial");

  //Parallel
  printf("PARALLEL\n");
  dataTime dataTimeParallel;
  int totalThreads = optionTotalThreads(matrixCase);
  dataTimeParallel = matrixVectorThread(totalThreads, info);
  calculateElapsedTime(&dataTimeParallel);
  dumpDotProductData(info, "parallel");
  dumpElapsedTime(&dataTimeParallel, "parallel");
  cleanDotProductData(info);

  return 0;
}
