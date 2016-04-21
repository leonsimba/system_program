/*
 * t_raise.c - An example for rasie()
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
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

void sig_handler(int signum)
{
	printf("handler signal signum = %d\n", signum);
}

int main()
{
	signal(SIGUSR1, sig_handler);
	raise(SIGUSR1);
	printf("execute after sig_handler\n");
	return 0;
}
