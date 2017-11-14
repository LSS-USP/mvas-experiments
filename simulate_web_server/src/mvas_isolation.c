#include <stdio.h>
#include <mvas/vas.h>

#include "mvas_isolation.h"

int create_isolate_vas(const char *vas_name, mode_t permission)
{
	int rc = 0;
#ifdef _MVAS_
	vasid_t vas_id = vas_find(vas_name);

	// If VAS already exists, we want to delete it to have a clean VAS
	if (vas_id >= 0) {
		rc = vas_delete(vas_id);
		if (rc < 0) {
			perror("vas_delete");
			return -1; // Something wrong
		}
	}

	vas_id = vas_create(vas_name, permission);
	if (vas_id < 0) {
		perror("vas_create");
		return -1;
	}
	rc = vas_id;
#endif
	return rc;
}

int delete_vas(int vas_id)
{
	int rc = 0;
#ifdef _MVAS_
	rc = vas_delete(vas_id);
	if (rc < 0)
  		return -1; // Something wrong
#endif
	return rc;
}
