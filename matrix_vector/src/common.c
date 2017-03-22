#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "dataTime.h"

void dumpDotProductData(dotProductData * pInfo, char * pBaseName)
{
  char bufferName[BUFFER];

  #ifdef _DEBUG_
  printf("Matrix\n");
  #endif
  strcpy(bufferName, pBaseName);
  strcat(bufferName, "_matrix.csv");
  writeMatrixToFile(bufferName, pInfo->matrix, pInfo->lines, pInfo->columns);

  #ifdef _DEBUG_
  printf("Vector\n");
  #endif

  strcpy(bufferName, pBaseName);
  strcat(bufferName, "_vector.csv");
  writeMatrixToFile(bufferName, pInfo->vector, 1, pInfo->columns);

  #ifdef _DEBUG_
  printf("Result\n");
  #endif

  strcpy(bufferName, pBaseName);
  strcat(bufferName, "_result.csv");
  writeMatrixToFile(bufferName, pInfo->finalVector, 1, pInfo->columns);
}

dotProductData * initDotProductData(const int pLines, const int pColumns)
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
    #ifdef _DEBUG_
    printf("Free matrix\n");
    #endif
    free(pInfo->matrix);
  }

  if (pInfo->vector)
  {
    #ifdef _DEBUG_
    printf("Free vector\n");
    #endif
    free(pInfo->vector);
  }

  // XXX: WHY THIS BREAK THE CODE?
  // if (pInfo->finalVector)
  // {
  //   #ifdef _DEBUG_
  //   printf("Free final vector\n");
  //   #endif
  //   free(pInfo->finalVector);
  // }
  #ifdef _DEBUG_
  printf("Free information\n");
  #endif
  free(pInfo);
  return 0;
}

long * matrixGenerate(const int pLines, const int pColumns)
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

void writeMatrixToFile(const char * pFile, long * pMatrix, const int pLines,
                       const int pColumns)
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

int optionMatrixSize(const int pOption)
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

int optionTotalThreads(const int pOption)
{
  switch(pOption)
  {
    case CASE_10_5:
      return 5;
    case CASE_100_20:
      return 20;
    case CASE_1000_40:
      return 40;
    case CASE_10000_80:
      return 80;
    default:
      return 10;
  }
  return -1;
}

void printTestConfiguration(const int pOption)
{
  printf("CONFIGURATION:\n");
  switch(pOption)
  {
    case CASE_10_5:
      printf("Matrix size: 10x10\nTotal of threads: 5\n");
      break;
    case CASE_100_20:
      printf("Matrix size: 100x100\nTotal of threads: 20\n");
      break;
    case CASE_1000_40:
      printf("Matrix size: 1000x1000\nTotal of threads: 40\n");
      break;
    case CASE_10000_80:
      printf("Matrix size: 10000x10000\nTotal of threads: 80\n");
    default:
      printf("(Default)\nMatrix size: 10x10\nTotal of threads: 10\n");
      break;
  }
}
