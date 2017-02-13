#include <stdio.h>
#include <string.h>

#include "common.h"
#include "dataTime.h"

void dumpEstimatePi(const piData * pInfo, const char * pBaseName)
{
  char bufferName[BUFFER];
  printf("Approximation of PI: %lf\n", pInfo->pi);
  printf("Total of terms: %ld\n", pInfo->terms);

  strcpy(bufferName, pBaseName);
  strcat(bufferName, "_pi.csv");
  FILE * piFile = fopen(bufferName, "a+");
  if (!piFile)
  {
    printf("Cannot open %s file \n", bufferName);
    return;
  }
  char piInfo[BUFFER];
  sprintf(piInfo, "%ld,%lf\n", pInfo->terms, pInfo->pi);
  fputs(piInfo, piFile);
  fclose(piFile);
}
