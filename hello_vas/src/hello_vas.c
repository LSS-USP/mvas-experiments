#include <linux/vas.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <vas.h>
#include <segment.h>

#define TOTAL_MVAS 3
#define BUFFERSIZE 4096
#define BASE_ADDRESS 0xC0DE000
#define BASE_SIZE 0x1000

typedef struct _vasAndSegment
{
  vasid_t vas;
  segid_t segment;
  char * vasName;
  char * segmentName;
  void * commonArea;
  unsigned long start;
  unsigned long size;
  mode_t permissions;
} vasAndSegment;

int vasWithSegment(vasAndSegment * pVasSegment)
{
  vasid_t vasId = 0;
  segid_t segmentId = 0;

  vasId = vas_find(pVasSegment->vasName);
  if (vasId < 0)
  {
   // Step 1: Create vas
   vasId = vas_create(pVasSegment->vasName, segmentMode);
   if (vasId < 0)
   {
     printf("Error on vas creation: %s\n", strerror(errno));
     return -1;
   }
  }
  else
  {
    printf("VAS already created\n");
  }
  pVasSegment->vas = vasId;

  segmentId = segment_find(pVasSegment->segmentName);
  if (segmentId < 0)
  {
    // Step 2: Create segment
    segmentId = segment_create_sz(pVasSegment->segmentName, pVasSegment->start,
                                  pVasSegment->size, segmentMode);
    if (segmentId < 0)
    {
      printf("Error on segment creation: %s\n", strerror(errno));
      vas_delete(pVasSegment->vas);
      return -1;
    }
  }
  else
  {
    printf("Segment already created\n");
  }

  // Step 3: Attach segment to vas
  int status = 0;
  status = segment_attach(pVasSegment->vas, pVasSegment->segment, O_WRONLY);
  if (status < 0)
  {
    printf("Error on segment attach: %s\n", strerror(errno));
    return -1;
  }

  return 0;
}

int playWithMvas(pid_t pPID, vasAndSegment * pVasSegment, int pValue)
{
  int status = vas_attach(pPID, pVasSegment->vas, S_IRWXU | S_IRWXG);
  if (status < 0)
  {
    printf("Cannot attach. Error: %s\n", strerror(errno));
    return -1;
  }

  printf("TRY TO SWITCH\n");
  status = vas_switch(pVasSegment->vas);
  if (status < 0)
  {
    printf("Cannot attach vas. Error: %s\n", strerror(errno));
    return -1;
  }

  printf("TRY TO MALLOC\n");
  pVasSegment->commonArea = malloc(sizeof(int));
  if (!pVasSegment->commonArea)
  {
    perror("malloc");
    return -1;
  }
  *((int*)pVasSegment->commonArea) = pValue;
  printf("This is VAS number: %d\n", * ((int*)pVasSegment->commonArea));

  status = vas_detach(pPID, pVasSegment->vas);
  if (status < 0)
  {
    printf("Probem with detach. Error: %s\n", strerror(errno));
  }

  return 0;
}

int cleanMvas(vasAndSegment * pVasSegment, pid_t pPID)
{
  int status = segment_detach(pVasSegment->vas, pVasSegment->segment);
  if (status < 0)
  {
    printf("Error to detach segment (%s)\n", pVasSegment->segmentName);
  }

  status = segment_delete(pVasSegment->segment);
  if (status < 0)
  {
    printf("Error to delete segment (%s)\n", pVasSegment->segmentName);
  }

  status = vas_delete(pVasSegment->vas);
  if (status < 0)
  {
    printf("Cannot delete vas. Error: %s\n", strerror(errno));
  }

  status = vas_delete(pVasSegment->vas);
  if (status < 0)
  {
    printf("Problem to delete. Error: %s\n", strerror(errno));
  }
  return 0;
}

int main(int argc, char * argv[])
{
  char vasName[BUFFERSIZE], segmentName[BUFFERSIZE];
  vasAndSegment mvas[TOTAL_MVAS];

  pid_t taskPid = getpid();
  unsigned long start = BASE_ADDRESS;
  unsigned long size = BASE_SIZE;
  mode_t mode = S_IRWXU | S_IRGRP;
  // Step 1: Create VAS
  printf("Initialization\n");
  for (int i = 0; i < TOTAL_MVAS; i++)
  {
    sprintf(vasName, "VAS%d", i);
    sprintf(segmentName, "SEGMENT%d", i);
    mvas[i].vasName = (char*)malloc(strlen(vasName) * sizeof(char));
    mvas[i].segmentName = (char*)malloc(strlen(segmentName) * sizeof(char));
    strcpy(mvas[i].vasName, vasName);
    strcpy(mvas[i].segmentName, segmentName);
    mvas[i].start = start;
    mvas[i].size = size;
    mvas[i].permissions = mode;
    vasWithSegment(&mvas[i]);
    start = start + size + 1;
  }

  // Step 2: Attach VAS to process
  for (int i = 0; i < TOTAL_MVAS; i++)
  {
    printf("====== VAS: %d ======\n", i);
    printf("TRY TO ATTACH\n");
    playWithMvas(taskPid, &mvas[i], i);
  }

  // Step 3: Clean everything
  for (int i = 0; i < TOTAL_MVAS; i++)
  {
    cleanMvas(&mvas[i], taskPid);
  }

  return 0;
}
