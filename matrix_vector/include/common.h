#ifndef __COMMON__
#define __COMMON__

#define MAX_RANGE_VALUES 700
#define SECONDS_TO_MILLISECONDS 1000
#define BUFFER 4096

enum
{
  CASE_10_5,
  CASE_100_20,
  CASE_1000_40,
  CASE_10000_80
};

typedef struct _dotProductData
{
  long * matrix;
  long * vector;
  long * finalVector;
  int lines;
  int columns;
} dotProductData;

/**
* Initialize dotProductData. It allocates matrix, vector, final vector and some
* extra informations
* @param pLines Total of lines
* @param pColumns Total of columns
* @return Return a reference to allocated data structure, otherwise return NULL
*/
extern dotProductData * initDotProductData(int pLines, int pColumns);

/**
* Clean all previously allocated data
* @param pInfo Reference to dotProductData allocated by initDotProductData
* @return Return 0 in case of success, otherwise return a negative number
*/
extern int cleanDotProductData(dotProductData * pInfo);

/**
* Generate matrix based o number of lines and column
* @param pLines Number of lines
* @param pColumns Number of columns
* @return Return allocated matrix in memory, or null if something is wrong
*/
extern long * matrixGenerate (int pLines, int pColumns);

/**
* Receives two values generated by clock() function, and return the total
* elapsed time in milliseconds
* @param pBegin Initial clock time captured
* @param pEnd Final clock time captured
* @return Return elapsed time in milliseconds
*/
extern double calculateElapsedTime (clock_t pBegin, clock_t pEnd);

/**
* Write matrix to file
* @param pFile File name to save matrix
* @param pMatrix Reference to matrix
* @param pLines Matrix lines
* @param pColumns Matrix column
*/
extern void writeMatrixToFile (char * pFile, long * pMatrix,
                               int pLines, int pColumns);

/**
* Option of matrix calculation
*
*/
extern int option(int pOption);

#endif
