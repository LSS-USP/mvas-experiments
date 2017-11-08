#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

#include "action.h"
#include "data_measurement.h"

static int byebye = 0;
static pid_t *children_list;
static int children;

void handle_signal(int sig)
{
	int index = 0;

	srand(time(NULL));
	index = rand() % children;

	printf("handle_signal catch: %d\n", sig);
	
	if (sig == SIGTERM) {
		byebye = 1;
		printf("SIGTERM RECEIVED AND BYEBYE\n");
		exit(1);
	}

	// Wake up a process
	if (sig == SIGUSR1) {
		printf("Hey, I toke my decision-> WAKE UP CHILD: %d\n",
			children_list[index]);
		if (children_list[index] > 0)
			kill(children_list[index], SIGUSR1);
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

	return ret;
}

int initialize_children(struct action_info *a)
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

	return 0;
}

pid_t main_child(void)
{
	pid_t pid;

	pid = fork();
	if (!pid){
		//Child should wait
		printf("Children created and stopped\n");
		while(1) {
			pause();
			printf("Child received any signal to continue\n");
			if (byebye)
				break;
		}
		exit(0);
	}

	return pid;
}
