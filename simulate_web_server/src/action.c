#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <mvas/vas.h>

#include "action.h"
#include "data_measurement.h"
#include "mvas_isolation.h"

static pid_t *children_list;
static int children;

void handle_signal(int sig)
{
	int index = 0, i = 0, ret = 0;

	srand(time(NULL));
	index = rand() % children;
#ifdef _VERBOSE_
	printf("handle_signal: catch: %d\n", sig);
#endif
	if (sig == SIGTERM || sig == SIGINT) {
#ifdef _VERBOSE_
		printf("SIGTERM RECEIVED: BYEBYE\n");
#endif
		for (i = 0; i < children; i++) {
			ret = kill(children_list[i], SIGTERM);
			if (ret < 0)
				perror("SIGTERM to child");
		}
		exit(1);
	}

	// Wake up a process
	if (sig == SIGUSR1) {
#ifdef _VERBOSE_
		printf("WAKE UP CHILD (PID %d): %d\n", getpid(),
			children_list[index]);
#endif
		if (children_list[index] > 0)
			kill(children_list[index], SIGCONT);
	}

	if (sig == SIGCONT) {
		printf("Just for child");
	}
}

int initialize_simulation(struct sigaction *s)
{
	int ret = 0;

	s->sa_handler = handle_signal;
	s->sa_flags = 0;
	sigemptyset(&s->sa_mask);

	ret = sigaction(SIGINT, s, NULL);
	if (ret == -1)
		return ret;

	ret = sigaction(SIGUSR1, s, NULL);
	if (ret == -1)
		return ret;

	ret = sigaction(SIGTERM, s, NULL);
	if (ret == -1)
		return ret;

	ret = sigaction(SIGCONT, s, NULL);
	if (ret == -1)
		return ret;

	return ret;
}

int start_server(struct action_info *a)
{
	int i = 0;

	if (a->children < 1 || a->children > LIMIT) {
		perror("invalid children limit");
		exit(1);
	}

	a->children_list = calloc(a->children, sizeof(pid_t));
	if (!a->children_list)
		return -1;

	children = a->children;
	children_list = a->children_list;

	for (i = 0; i < a->children; i++) {
		a->children_list[i] = main_child();
	}

	signal(SIGCONT, SIG_DFL);

	return 0;
}

static void do_something(void *data, unsigned int size)
{
	printf("+++> do_something> %s\n", (char*)data);
#ifdef _MVAS_
	int rc = 0;
	mode_t mode = S_IRWXU | S_IRWXG;
	vasid_t vas_id = 0;

	printf("+++> do_something: MVAS\n");

	// Turn on VAS
	vas_id = create_isolate_vas((char*)data, mode);
	if (vas_id < 0) {
		printf("Error to create isolate vas\n");
	} else {
		rc = vas_attach(0, vas_id, mode);
		if (rc >= 0) {
			rc = vas_switch(vas_id);
			if (rc < 0)
				printf("Error to switch vas\n");
		}
	}

	printf("==== Heyyyyy, I am inside the mvas ===");

	// Turn of VAS
	if (vas_id >= 0) {
		rc = vas_switch(0);
		if (rc < 0)
			printf("Error to switch back vas\n");

		rc = vas_detach(0, vas_id);
		if (rc < 0)
			printf("Error to detach vas\n");

		rc = vas_delete(vas_id);
		if (rc < 0)
			printf("Error to delete vas\n");
	}
#endif
}

pid_t main_child(void)
{
	pid_t pid;
	struct measurement_data m;
	char buf[256];

	pid = fork();
	if (!pid) {
		signal(SIGINT, SIG_DFL);
		signal(SIGUSR1, SIG_DFL);
		signal(SIGTERM, SIG_DFL);
		sprintf(buf, "%d", getpid());
#ifdef _VERBOSE_
		printf("Children created and stopped\n");
#endif
		//Request processing
		while(1) {
			pause();
			get_begin_time(&m);
#ifdef _VERBOSE_
			printf("--> Child %d will handle the request\n", getpid());
#endif
			do_something(buf, sizeof(buf));
			get_end_time(&m);
			calculate_elapsed_time(&m);
			//dump_elapsed_time(&m, "...");
		}
		exit(0);
	}

	return pid;
}
