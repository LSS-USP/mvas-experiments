#ifndef __COMMON__
#define __COMMON__

enum
{
  CASE_10_5,
  CASE_100_20,
  CASE_1000_40,
  CASE_10000_80
};

extern long * matrixGenerate (int pLines, int pColumns);
extern double calculateElapsedTime (clock_t pBegin, clock_t pEnd);
extern void writeMatrixToFile (char * pFileName, long * pMatrix,
                               int pLines, int pColumns);

#endif
