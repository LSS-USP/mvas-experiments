#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "common.h"

long * matrixGenerate(int pLines, int pColumns)
{
  int totalSize = pLines * pColumns;
  long * matrix = (long *) calloc(totalSize, sizeof(long));
  srand((unsigned int)clock());
  for (int i = 0; i < totalSize; i++)
  {
    matrix[i] = rand() % 500;
  }
  return matrix;
}

double calculateElapsedTime(clock_t pBegin, clock_t pEnd)
{
  double elapsed = (((double)pEnd - pBegin) / (CLOCKS_PER_SEC));
  return elapsed * 1000;
}

void writeMatrixToFile(char * pFileName, long * pMatrix, int pLines, int pColumns)
{
  FILE * saveTo = fopen(pFileName, "w+");
  if (!saveTo)
  {
    printf("Error to open file\n");
    return;
  }
  char number[1024];

  for (int i = 0; i < pLines; i++)
  {
    for (int j = 0; j < pColumns; j++)
    {
      sprintf(number, "%ld", *(pMatrix + i*pLines + j));
      printf("%s", number);
      fputs(number, saveTo);
      if (j < pColumns - 1)
      {
        printf(",");
        fputc(',', saveTo);
      }
    }
    fputc('\n', saveTo);
    printf("\n");
  }

  fclose(saveTo);
}
