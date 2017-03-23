#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "serial.h"
#include "parallelThread.h"
#include "mvasParallelThread.h"

int main(int argc, char * argv[])
{
  if (argc != 2)
  {
    printf("Error in the program execution\n");
    printf("\nParameters (Matrix Size [thread]):\n\t0 - 10x10 [5]\n\t"\
           "1 - 100x100 [20]\n\t2 - 1000x1000 [40]\n\t3 - 10000x10000 [80]\n");
    return -1;
  }

  // Argument defines matrix size and the total of threads. We hard coded those
  // options
  int matrixCase = atoi(argv[1]);
  int matrixBase = optionMatrixSize(matrixCase);

  // Info gona be the same for matrix, and vector for all tests. However, value
  // will be override everytime
  dotProductData * info = initDotProductData(matrixBase, matrixBase);
  if (!info)
  {
    printf("Error on initialization of dot product data structure\n");
    return -1;
  }

  printTestConfiguration(matrixCase);
  int totalThreads = optionTotalThreads(matrixCase);

  // Serial
  printf("\nSERIAL...\n");
  dataTime dataTimeSerial;
  dataTimeSerial = matrixVectorSerial(info);
  calculateElapsedTime(&dataTimeSerial);
  dumpDotProductData(info, "serial");
  dumpElapsedTime(&dataTimeSerial, "serial");

  // Parallel
  printf("\nPARALLEL...\n");
  dataTime dataTimeParallel;
  dataTimeParallel = matrixVectorThread(totalThreads, info);
  calculateElapsedTime(&dataTimeParallel);
  dumpDotProductData(info, "parallel");
  dumpElapsedTime(&dataTimeParallel, "parallel");

  // MVAS
  printf("\nPARALLEL WITH VAS...\n");
  dataTime mvasDataTimeParallel;
  mvasDataTimeParallel = mvasMatrixVectorThread(totalThreads, info);
  calculateElapsedTime(&mvasDataTimeParallel);
  dumpDotProductData(info, "mvas-parallel");
  dumpElapsedTime(&mvasDataTimeParallel, "mvas-parallel");

  cleanDotProductData(info);

  return 0;
}
