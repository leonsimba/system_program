/*
 * signal_int_quit.c - An example for signal()
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

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static void sig_handler(int signum)
{
	static int count = 0;

	if (signum == SIGINT) {
		count++;
		printf("Caught SIGINT (%d)\n", count);
	} else if (signum == SIGQUIT) {
		printf("Caught SIGQUI\n");
		exit(0);
	}
}

int main(int argc,char *argv[])
{
	/* register signal handler for SIGINIT
	 * Ctrl + c can send SIGINT signal
	 */
	if (signal(SIGINT, sig_handler) == SIG_ERR) {
		perror("signal failed");
		exit(-1);
	}

	/* register signal handler for SIGINIT
	 * Ctrl + \ can send SIGQUIT signal
	 */
	if (signal(SIGQUIT, sig_handler) == SIG_ERR) {
		perror("signal failed");
		exit(-1);
	}
	for(;;)
		pause();
	return 0;
}
