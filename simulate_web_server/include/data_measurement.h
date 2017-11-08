#ifndef _DATA_TIME
#define _DATA_TIME

#include <time.h>

#define DATA_TIME_BUFFER 4096
#define SECONDS_TO_MILLISECONDS 1000
#define BILLION 1E9

struct measurement_data
{
	struct timespec beginMonotonicRaw;
	struct timespec endMonotonicRaw;
	clock_t beginClock;
	clock_t endClock;
	double elapsedMonotonicRaw;
	double elapsedClock;
};

/*
 * Fill out informations about start measure in dataTime structure
 *
 * @param pDataTime Data time information
 */
void get_begin_time(struct measurement_data *m);

/*
 * With this function, all informations related to stop is saved.
 *
 * @param pDataTime data structure with time information
 * @see dataTime
 */
void get_end_time(struct measurement_data *m);

/*
 * Show and save information collected on dataTime structure
 *
 * @param pDataTime Parameter with time information
 * @param pPrefix
 */
void dump_elapsed_time(const struct measurement_data* m, char *p);

/*
 * Based on dataTime informations, calculate the elapsed time
 *
 * @param pDataTime data structure with all time information
 * @see dataTime
 */
void calculate_elapsed_time(struct measurement_data *m);

#endif
