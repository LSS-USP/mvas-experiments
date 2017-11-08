/*
 * This file has the data structures and function prototype of the simulation
 */

#ifndef _ACTION_H
#define _ACTION_H

#include <unistd.h>

/*
 * Upper limit for child processes
 */
#define LIMIT 100

/*
 * Data structure responsible for centralize the data info for handling the
 * simulation
 *
 * @signal_info sigaction struct wherein we configure signal handlers
 * @children Total of children processes that could be used to handle request
 * @children_list Keep track of all child process
 */
struct action_info {
	struct sigaction signal_info;
	int children;
	pid_t *children_list;
};

/*
 * Function responsible for get the request and select a child process to
 * handle it.
 *
 * @sig Received signal
 */
void handle_signal(int sig);

/*
 * Initialize simulation data structure by installing handle_signal to handle
 * a set of signals
 */
int initialize_simulation(struct sigaction *s);

/*
 * Initialize each child processes responsible for handling a request and save
 * the pid on parent data structure
 */
int start_server(struct action_info *a);

/*
 * Main child process. Here we have the request handling
 * @note If you want to test any new technology of processes isolation, here
 * is the place!
 */
pid_t main_child(void);

#endif
