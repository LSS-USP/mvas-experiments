#ifndef _MVAS_ISOLATION_H
#define _MVAS_ISOLATION_H

int create_isolate_vas(const char *vas_name, mode_t permission);

int delete_vas(int vas_id);

#endif
