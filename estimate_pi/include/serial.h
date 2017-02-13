#ifndef __SERIAL__
#define __SERIAL__

#include "dataTime.h"
#include "common.h"

/**
* @param pInfo Data structure with required information to calculate p
* @return Return dataTime structure
*/
extern dataTime estimatePiSerial(piData * pInfo);

#endif
