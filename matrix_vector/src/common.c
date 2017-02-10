#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "common.h"

void getBeginTime(dataTime * pDataTime)
{
  clock_gettime(CLOCK_MONOTONIC_RAW, &pDataTime->beginMonotonicRaw);
  pDataTime->beginClock = clock();
}

void getEndTime(dataTime * pDataTime)
{
  clock_gettime(CLOCK_MONOTONIC_RAW, &pDataTime->endMonotonicRaw);
  pDataTime->endClock = clock();
}

void calculateElapsedTime(dataTime * pDataTime)
{
  pDataTime->elapsedClock = (((double)pDataTime->endClock -
                              pDataTime->beginClock) / (CLOCKS_PER_SEC)) *
                              SECONDS_TO_MILLISECONDS;
  pDataTime->elapsedMonotonicRaw =
          (pDataTime->endMonotonicRaw.tv_sec -
           pDataTime->beginMonotonicRaw.tv_sec ) +
          (pDataTime->endMonotonicRaw.tv_nsec -
          pDataTime->beginMonotonicRaw.tv_nsec)
          / BILLION;
}

void dumpElapsedTime(const dataTime * pDataTime, char * pPrefix)
{
  printf("Elapsed time (MONOTONIC): %lf (s)\n", pDataTime->elapsedMonotonicRaw);
  printf("Elapsed time (CLOCK): %lf (ms)\n", pDataTime->elapsedClock);
  char saveTo[BUFFER];
  char output[BUFFER];
  strcpy(saveTo, pPrefix);
  strcat(saveTo, "_elapsedTime.txt");

  FILE * elapsedTimeFile = fopen(saveTo, "a+");
  if (!elapsedTimeFile)
  {
    printf("Error to write information file\n");
    return;
  }
  sprintf(output, "Elapsed time (MONOTONIC): %lf (s)\n",
          pDataTime->elapsedMonotonicRaw);
  fputs(output, elapsedTimeFile);
  sprintf(output, "Elapsed time (CLOCK): %lf (ms)\n",
          pDataTime->elapsedClock);
  fputs(output, elapsedTimeFile);
  fclose(elapsedTimeFile);
}

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

int optionMatrixSize(int pOption)
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

int optionTotalThreads(int pOption)
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
