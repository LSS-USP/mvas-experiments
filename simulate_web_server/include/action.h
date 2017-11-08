#ifndef _ACTION_H
#define _ACTION_H

#include <unistd.h>

#define LIMIT 100

struct action_info {
	struct sigaction signal_info;
	int children;
	pid_t *children_list;
};

void handle_signal(int sig);
void handle_request(int sig);

int initialize_simulation(struct sigaction *s);
int initialize_children(struct action_info *a);

pid_t main_child(void);

#endif
