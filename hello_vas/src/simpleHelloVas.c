#include <linux/vas.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <mvas/vas.h>
#include <mvas/segment.h>

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
  int commonArea;
  unsigned long start;
  unsigned long size;
  mode_t permissions;
} vasAndSegment;

int createVAS(vasAndSegment * pVasSegment)
{
  vasid_t vasId = 0;

  vasId = vas_find(pVasSegment->vasName);
  if (vasId < 0)
  {
    // Step 1: Create vas
    vasId = vas_create(pVasSegment->vasName, pVasSegment->permissions);
    if (vasId < 0)
    {
      printf("Error on vas creation: %s\n", strerror(errno));
      return -1;
    }
  }
  else
  {
    printf("VAS already exists. Continue...\n");
  }

  pVasSegment->vas = vasId;

  return 0;
}

int createSegment(vasAndSegment * pVasSegment)
{
  segid_t segmentId = 0;

  segmentId = segment_find(pVasSegment->segmentName);
  if (segmentId < 0)
  {
    // Step 2: Create segment
    segmentId = segment_create_sz(pVasSegment->segmentName, pVasSegment->start,
                                  pVasSegment->size, pVasSegment->permissions);
    if (segmentId < 0)
    {
      printf("Error on segment creation: %s\n", strerror(errno));
      return -1;
    }
  }
  else
  {
    printf("Segment already created. Continue...\n");
  }

  pVasSegment->segment = segmentId;

  return 0;
}

int attachSegmentToVAS(vasAndSegment * pVasSegment)
{
  int status = 0;
  status = segment_attach(pVasSegment->vas, pVasSegment->segment, O_RDWR);
  if (status < 0)
  {
    printf("Error on segment attach: %s\n", strerror(errno));
    return -1;
  }
  return 0;
}

int playWithMvas(pid_t pPID, vasAndSegment * pVasSegment, int pValue)
{
  int status = vas_attach(pPID, pVasSegment->vas, S_IRWXU);
  if (status < 0)
  {
    printf("Cannot attach. Error: %s\n", strerror(errno));
    return -1;
  }

  printf("Try to switch\n");
  status = vas_switch(pVasSegment->vas);
  if (status < 0)
  {
    printf("Cannot attach vas. Error: %s\n", strerror(errno));
    return -1;
  }

  printf("Try to save data on new VAS :)\n");
  pVasSegment->commonArea = 42;
  if (!pVasSegment->commonArea)
  {
    perror("common area");
    return -1;
  }

  printf("This is the value on VAS: %d\n", pVasSegment->commonArea);

  status = vas_switch(0);
  if (status < 0)
  {
    printf("Cannot attach vas. Error: %s\n", strerror(errno));
    return -1;
  }

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
    printf("Error: %s\n", strerror(errno));
  }

  status = segment_delete(pVasSegment->segment);
  if (status < 0)
  {
    printf("Error to delete segment (%s)\n", pVasSegment->segmentName);
    printf("Error: %s\n", strerror(errno));
  }

  status = vas_delete(pVasSegment->vas);
  if (status < 0)
  {
    printf("Cannot delete vas. Error: %s\n", strerror(errno));
    printf("Error: %s\n", strerror(errno));
  }
  return 0;
}

int main(int argc, char * argv[])
{
  vasAndSegment mvas;
  pid_t taskPid = getpid();
  unsigned long start = BASE_ADDRESS;
  unsigned long size = BASE_SIZE;
  mode_t mode = S_IRWXU | S_IRGRP;

  // Step 1: Create VAS
  printf("Initialization:  Get basic information\n");
  mvas.vasName = "VAS0";
  mvas.segmentName = "SEGMENT0";
  mvas.start = start;
  mvas.size = size;
  mvas.permissions = mode;
  printf("\tCreate VAS\n");
  if (createVAS(&mvas) < 0)
  {
    return -1;
  }

  printf("\tCreate Segment\n");
  if (createSegment(&mvas) < 0)
  {
    return -1;
  }

  printf("\tCreate attach segment to VAS\n");
  if (attachSegmentToVAS(&mvas) < 0)
  {
    return -1;
  }

  printf("Try to attach\n");
  if (playWithMvas(taskPid, &mvas, 1) < 0)
  {
    return -1;
  }

  printf("Clean everything\n");
  cleanMvas(&mvas, taskPid);

  return 0;
}
