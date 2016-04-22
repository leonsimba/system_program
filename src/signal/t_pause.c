/*
 * t_pause.c - An example for pause()
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

void sig_handler(int signum)
{
	printf("Handle SIGINT\n");
}

int main()
{
	if (signal(SIGINT,sig_handler) == SIG_ERR) {
		perror("signal failed");
		exit(-1);
	}
	pause();
	printf("After pause\n");

	return 0;
}
