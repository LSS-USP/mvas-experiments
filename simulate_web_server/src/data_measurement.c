#include <stdio.h>
#include <time.h>
#include <string.h>

#include "data_measurement.h"

void get_begin_time(struct measurement_data *m)
{
	clock_gettime(CLOCK_MONOTONIC_RAW, &m->beginMonotonicRaw);
	m->beginClock = clock();
}

void get_end_time(struct measurement_data *m)
{
	clock_gettime(CLOCK_MONOTONIC_RAW, &m->endMonotonicRaw);
	m->endClock = clock();
}

void calculate_elapsed_time(struct measurement_data *m)
{
  m->elapsedClock = (((double)m->endClock -
                              m->beginClock) / (CLOCKS_PER_SEC)) *
                              SECONDS_TO_MILLISECONDS;
  m->elapsedMonotonicRaw =
          (m->endMonotonicRaw.tv_sec -
           m->beginMonotonicRaw.tv_sec ) +
          (m->endMonotonicRaw.tv_nsec -
          m->beginMonotonicRaw.tv_nsec)
          / BILLION;
}

void dump_elapsed_time(const struct measurement_data *m, char *p)
{
	char saveTo[DATA_TIME_BUFFER];
	char output[DATA_TIME_BUFFER];
#ifdef _VERBOSE_
	printf("Elapsed time (MONOTONIC): %lf (s)\n", m->elapsedMonotonicRaw);
	printf("Elapsed time (CLOCK): %lf (ms)\n", m->elapsedClock);
#endif
	strcpy(saveTo, p);
	strcat(saveTo, "_elapsedTime.txt");

	FILE * elapsedTimeFile = fopen(saveTo, "a+");
	if (!elapsedTimeFile) {
	  printf("Error to write information file\n");
	  return;
	}
	sprintf(output, "Elapsed time (MONOTONIC): %lf (s)\n",
	        m->elapsedMonotonicRaw);
	fputs(output, elapsedTimeFile);
	sprintf(output, "Elapsed time (CLOCK): %lf (ms)\n",
	        m->elapsedClock);
	fputs(output, elapsedTimeFile);
	fclose(elapsedTimeFile);
}
