#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

#include "action.h"

static void help_info(void)
{
	printf("You should provide the total of children:\n");
	printf("Usage: simulate_web_server <value>\n");
	exit(1);
}

int main(int argc, char ** argv)
{
	struct action_info action;
	int ret = 0, i = 0;

	action.children = -1;

	if (argc < 2)
		help_info();

	action.children = atoi(argv[1]);

	if (action.children < 1)
		action.children = 1;

	ret = initialize_simulation(&action.signal_info);
	if (ret == -1) {
		perror("initialize_simulation()");
		exit(1);
	}

	ret = start_server(&action);
	if (ret == -1) {
		perror("initialize_children()");
		exit(1);
	}

	printf("Processes available for handling requests:\n");
	for(i = 0; i < action.children; i++)
		printf("%d: %d\n", i, action.children_list[i]);
	printf("READY\n");
	for(;;) {
		// App main loop
		pause();
	}

	return 0;
}
