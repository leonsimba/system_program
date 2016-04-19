/*
 * wait_all_children.c - Parent wait all chilren termiated
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
#include <sys/wait.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define BUF_SIZE    1000

char *currTime(const char *format)
{
	static char buf[BUF_SIZE];
	time_t t;
	size_t s;
	struct tm *tm;

	t = time(NULL);
	tm = localtime(&t);
	if (tm == NULL)
		return NULL;

	s = strftime(buf, BUF_SIZE, (format != NULL) ? format : "%c", tm);
	return (s==0) ? NULL : buf;
}

int main(int argc,char *argv[])
{
	int numDead;
	pid_t childPid;
	int j;

	if (argc<2 || strcmp(argv[1],"--help") == 0) {
		printf("%s sleep-time1 sleep-time2 ...\n",argv[0]);
		return -1;
	}

	/* disable buffering of stdout */
	setbuf(stdout,NULL);

	/* foreach argument create one child */
	for (j = 1; j < argc; j++) {
		switch (fork()) {
			case -1:
				perror("fork failed");
				exit(-1);
			case 0:
				printf("[%s] Child %d start with PID %ld,sleeping %s seconds\n",
					currTime("%T"), j, (long)getpid(), argv[j]);
				sleep(atoi(argv[j]));
				_exit(0);
			default:
				break;
		}
	}

	sleep(1);
	numDead = 0;
	/* parents waits until all children exit */
	for(;;){
		childPid = wait(NULL);
		if (childPid == -1) {
			if (errno == ECHILD) {
				printf("No more children - bye!\n");
				exit(0);
			} else {
				perror("wait failed");
				exit(-1);
			}
		}
		numDead++;
		printf("[%s] wait() returned child PID %ld (numDead=%d)\n",
			currTime("%T"), (long)childPid, numDead);
	}
	return 0;
}
