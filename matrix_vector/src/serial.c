#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "common.h"

int main(int argc, char * argv[])
{
  if (argc > 2)
  {
    printf("Error in the program execution\n");
    return -1;
  }

  int matrixCase = atoi(argv[1]);
  int matrixBase = 10;
  switch(matrixCase)
  {
    case CASE_10_5:
      matrixBase = 10;
    break;
    case CASE_100_20:
      matrixBase = 100;
    break;
    case CASE_1000_40:
      matrixBase = 1000;
    break;
    case CASE_10000_80:
      matrixBase = 10000;
    default:
      printf("Invalid option\n");
      return -1;
    break;
  }

  long * matrix = matrixGenerate(matrixBase, matrixBase);
  long * vector = matrixGenerate(matrixBase, 1);
  long * finalVector = (long*)calloc(matrixBase, sizeof(long));
  int lines = matrixBase;
  int columns = matrixBase;

  clock_t begin, end;
  double elapsed = 0;
  begin = clock();

  for (int line = 0; line < lines; line++)
  {
    finalVector[line] = 0.0;
    for(int column = 0; column < columns; column++)
    {
      finalVector[line] += *((matrix + line*lines) + column) * vector[column];
    }
  }
  end = clock();

  printf("Matrix\n");
  writeMatrixToFile("matrix.csv", matrix, lines, columns);
  printf("Vector\n");
  writeMatrixToFile("vector.csv", vector, 1, columns);
  printf("Result\n");
  writeMatrixToFile("result.csv", finalVector, 1, columns);

  free(matrix);
  free(vector);
  free(finalVector);

  elapsed = calculateElapsedTime(begin, end);
  printf("Total time elapsed: %f (ms)\n", elapsed);

  return 0;
}
