/*
 * child_status.c - Call waitpid() to get child's wait status
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
#define _GNU_SOURCE 	/* get strsignal() declaration from <string.h> */
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

void print_wait_status(const char *msg,int status)
{
	if (WIFEXITED(status)) {
		printf("child exited, status = %d\n", WEXITSTATUS(status));
	} else if(WIFSIGNALED(status)) {
		printf("child killed by signal %d (%s)\n",
			WTERMSIG(status), strsignal(WTERMSIG(status)));
	} else if (WIFSTOPPED(status)) {
		printf("child stopped by signal %d (%s)\n",
			WSTOPSIG(status), strsignal(WSTOPSIG(status)));
	} else if(WIFCONTINUED(status)) {
		printf("child continued\n");
	} else {
		printf("what happened to this child? (status = %x)\n",
				(unsigned int)status);
	}
}

int main(int argc, char *argv[])
{
	int status;
	pid_t childPid;

	switch (fork()) {
		case -1:
			perror("fork failed");
			break;
		case 0:
			/*Child: either exits immediately with given status or loops waiting for signals*/
			printf("Child started with PID = %ld\n", (long)getpid());
			if (argc > 1) {
				exit(atoi(argv[1]));
			} else {
				for(;;)
					pause();
			}
			exit(0);
		default:
			for (;;) {
				childPid = waitpid(-1, &status, WUNTRACED | WCONTINUED);
				if (childPid == -1) {
					perror("waitpid failed");
					exit(-1);
				}

				printf("waitpid() returned:PID = %ld; status = 0x%04x (%d, %d)\n",
					(long)childPid, (unsigned int)status, status>>8, status & 0xff);
				print_wait_status(NULL,status);

				/* If child was killed or terminated, then terminated the parent */
				if (WIFEXITED(status) || WIFSIGNALED(status))
					exit(0);
			}
	}
	return 0;
}
