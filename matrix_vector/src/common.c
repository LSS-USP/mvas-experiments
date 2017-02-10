#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "common.h"

dotProductData * initDotProductData(int pLines, int pColumns)
{
  dotProductData * info = (dotProductData *) calloc(1, sizeof(dotProductData));
  if (!info)
  {
    printf("Error when tried to allocate dotProduct data\n");
    return NULL;
  }

  info->matrix = matrixGenerate(pLines, pColumns);
  if (!info->matrix)
  {
    printf("Error on matrix allocation\n");
    return NULL;
  }
  info->vector = matrixGenerate(pLines, 1);
  if (!info->vector)
  {
    printf("Error on vector allocation\n");
    free(info->matrix);
    return NULL;
  }
  info->finalVector = (long*)calloc(pLines, sizeof(long));
  if (!info->finalVector)
  {
    printf("Error on final vector allocation\n");
    free(info->matrix);
    free(info->vector);
    return NULL;
  }
  info->lines = pLines;
  info->columns = pColumns;
  return info;
}

int cleanDotProductData(dotProductData * pInfo)
{
  if (!pInfo)
  {
    return 0;
  }
  if (pInfo->matrix)
  {
    free(pInfo->matrix);
  }
  if (pInfo->vector)
  {
    free(pInfo->vector);
  }
  if (pInfo->finalVector)
  {
    free(pInfo->finalVector);
  }
  free(pInfo);
  return 0;
}

long * matrixGenerate(int pLines, int pColumns)
{
  if (pLines < 0 || pColumns < 0)
  {
    printf ("Invalid parameter on matrixGenerate\n");
    return NULL;
  }

  int totalSize = pLines * pColumns;
  long * matrix = (long *) calloc(totalSize, sizeof(long));
  if (!matrix)
  {
    printf("Error on calloc\n");
    return NULL;
  }

  srand((unsigned int)clock());
  for (int i = 0; i < totalSize; i++)
  {
    matrix[i] = rand() % MAX_RANGE_VALUES;
  }
  return matrix;
}

double calculateElapsedTime(clock_t pBegin, clock_t pEnd)
{
  double elapsed = (((double)pEnd - pBegin) / (CLOCKS_PER_SEC));
  return elapsed * SECONDS_TO_MILLISECONDS;
}

void writeMatrixToFile(char * pFile, long * pMatrix, int pLines, int pColumns)
{
  if (!pMatrix || pLines < 0 || pColumns < 0)
  {
    printf("Invalid parameter on writeMatrixToFile\n");
    return;
  }

  FILE * saveTo = fopen(pFile, "w+");
  if (!saveTo)
  {
    printf("Error to open file\n");
    return;
  }

  char number[BUFFER];
  for (int i = 0; i < pLines; i++)
  {
    for (int j = 0; j < pColumns; j++)
    {
      sprintf(number, "%ld", *(pMatrix + i*pLines + j));
      #ifdef _DEBUG_
      printf("%s", number);
      #endif
      fputs(number, saveTo);
      // Remove ',' on final element
      if (j < pColumns - 1)
      {
        #ifdef _DEBUG_
        printf(",");
        #endif
        fputc(',', saveTo);
      }
    }
    fputc('\n', saveTo);
    #ifdef _DEBUG_
    printf("\n");
    #endif
  }
  fclose(saveTo);
}

int option(int pOption)
{
  switch(pOption)
  {
    case CASE_10_5:
      return 10;
    case CASE_100_20:
      return 100;
    case CASE_1000_40:
      return 1000;
    case CASE_10000_80:
      return 10000;
    default:
      return 10;
  }
  return -1;
}
