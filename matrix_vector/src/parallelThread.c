#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#include "common.h"

// Matrix data
static long * gMatrix = NULL;
static long * gVector = NULL;
static long * gFinalVector = NULL;
static int gTotalThreads = 0;
static int gMatrixLines = 0;
static int gColumns = 0;
static int gLines = 0;

void * matrixVectorThread(void * pId)
{
  long localId = *((long*)pId);
  int localToProcess = gMatrixLines / gTotalThreads;
  int startLine = localId * localToProcess;
  int endLine = (localId + 1) * localToProcess - 1;

  for (int line = startLine; line <= endLine; line++)
  {
    gFinalVector[line] = 0.0;
    for (int column = 0; column < gColumns; column++)
    {
      gFinalVector[line] += *((gMatrix + line*gLines) + column) * gVector[column];
    }
  }

  return NULL;
}
