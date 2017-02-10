#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "common.h"
#include "serial.h"

int main(int argc, char * argv[])
{
  if (argc != 2)
  {
    printf("Error in the program execution\n");
    return -1;
  }

  int matrixCase = atoi(argv[1]);
  int matrixBase = option(matrixCase);

  dotProductData * info = initDotProductData(matrixBase, matrixBase);
  if (!info)
  {
    printf("Error on initialization of dot product data structure\n");
    return -1;
  }

  clock_t begin, end;
  double elapsed = 0;

  begin = clock();
  matrixVectorSerial(info);
  end = clock();

  #ifdef _DEBUG_
  printf("Matrix\n");
  #endif
  writeMatrixToFile("matrix.csv", info->matrix, info->lines, info->columns);
  #ifdef _DEBUG_
  printf("Vector\n");
  #endif
  writeMatrixToFile("vector.csv", info->vector, 1, info->columns);
  #ifdef _DEBUG_
  printf("Result\n");
  #endif
  writeMatrixToFile("result.csv", info->finalVector, 1, info->columns);

  cleanDotProductData(info);

  elapsed = calculateElapsedTime(begin, end);
  printf("Elapsed time: %f (ms)\n", elapsed);

  FILE * elapsedTable = fopen("elapsedTable.txt", "a+");
  if (!elapsedTable)
  {
    printf("Error to save elapsed time\n");
    return -1;
  }

  char elapsedString[BUFFER];
  sprintf(elapsedString, "%f\n", elapsed);
  fputs(elapsedString, elapsedTable);
  fclose(elapsedTable);

  return 0;
}
