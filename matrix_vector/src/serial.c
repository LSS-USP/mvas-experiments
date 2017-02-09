#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "common.h"

int main(int argc, char * argv[])
{
  if (argc != 2)
  {
    printf("Error in the program execution\n");
    return -1;
  }

  int matrixCase = atoi(argv[1]);
  int matrixBase = option(matrixCase);

  long * matrix = matrixGenerate(matrixBase, matrixBase);
  if (!matrix)
  {
    printf("Error on matrix allocation\n");
    return -1;
  }

  long * vector = matrixGenerate(matrixBase, 1);
  if (!vector)
  {
    printf("Error on vector allocation\n");
    free(matrix);
    return -1;
  }
  long * finalVector = (long*)calloc(matrixBase, sizeof(long));
  if (!finalVector)
  {
    printf("Error on final vector allocation\n");
    free(matrix);
    free(vector);
    return -1;
  }

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

  #ifdef _DEBUG_
  printf("Matrix\n");
  #endif
  writeMatrixToFile("matrix.csv", matrix, lines, columns);
  #ifdef _DEBUG_
  printf("Vector\n");
  #endif
  writeMatrixToFile("vector.csv", vector, 1, columns);
  #ifdef _DEBUG_
  printf("Result\n");
  #endif
  writeMatrixToFile("result.csv", finalVector, 1, columns);

  free(matrix);
  free(vector);
  free(finalVector);

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
