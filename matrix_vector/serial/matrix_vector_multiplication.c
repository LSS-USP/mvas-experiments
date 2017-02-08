#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum
{
  CASE_10_5,
  CASE_100_20,
  CASE_1000_40,
  CASE_10000_80
};

int * matrixGenerate(int pLines, int pColumn)
{
  int totalSize = pLines * pColumn;
  int * matrix = (int *) calloc(pLines * pColumn, sizeof(int));
  srand(time(NULL));
  for (int i = 0; i < totalSize; i++)
  {
    matrix[i] = rand() % 500;
  }
  return matrix;
}

double calculateElapsedTime(clock_t pBegin, clock_t pEnd)
{
  double elapsed = (((double)pEnd - pBegin) / (CLOCKS_PER_SEC));
  printf("elapsed: %f\n", elapsed);
  return elapsed * 1000;
}

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
      matrixBase = 100;
    break;
  }

  int * matrix = matrixGenerate(matrixBase, matrixBase);
  int * vector = matrixGenerate(matrixBase, 1);
  int * finalVector = (int*)calloc(matrixBase, sizeof(int));
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
  free(matrix);
  free(vector);
  free(finalVector);

  elapsed = calculateElapsedTime(begin, end);
  printf("begin: %ld\n", (long)begin);
  printf("end: %ld\n", (long)end);
  printf("Total time elapsed: %f (ms)\n", elapsed);

  return 0;
}
