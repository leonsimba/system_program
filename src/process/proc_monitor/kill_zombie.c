/*
 * kill_zombie.c - Verify that the kill signal can't kill
 * 				   the zombie process.
 *
 * Copyright (C) 2016 ZhiPing.He. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Author:
 * 	ZhiPing.He <zhiping.hzp@alibaba-inc.com>
 */

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <libgen.h> 	/* for basename() */
#include <stdio.h>
#include <stdlib.h>

#define CMD_SIZE 200

int main(int argc,char *argv[])
{
	char cmd[CMD_SIZE];
	pid_t childPid;

	/* disable buffering of stdout */
	setbuf(stdout,NULL);
	printf("Parent PID=%ld\n", (long)getpid());

	switch (childPid = fork()) {
		case -1:
			perror("fork failed");
			break;
		case 0:		/* child immediately exists to become zombie */
			printf("Child (PID=%ld) exiting\n", (long)getpid());
			_exit(0);
		default:	/* parent */
			/* wait the child became to a zombie */
			sleep(3);

			snprintf(cmd, CMD_SIZE, "ps | grep %s", basename(argv[0]));
			cmd[CMD_SIZE - 1] = '\0';

			/* send the kill signal to the zombie process */
			if (kill(childPid, SIGKILL) == -1) {
				perror("kill failed");
				exit(-1);
			}

			sleep(3);
			printf("After sending SIGKILL to zombie (PID=%ld):\n",(long)childPid);

			/* view zombie child again */
			system(cmd);
			exit(0);
	}
	return 0;
}
