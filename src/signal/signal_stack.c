#define _GNU_SOURCE	/*Get strsignal() declaration from <string.h>*/
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static void sigsegv_handler(int sig)
{
	int x;

	printf("Caught signal %d (%s)\n", sig, strsignal(sig));
	printf("Top of handler stack near %10p\n", (void *)&x);
	fflush(NULL);
	_exit(0);	/*Can't return after SIGSEGV*/
}

static void overflowStack(int callNum)
{
	char a[100000];		/*Make this stack frame large*/

	printf("Call %4d - top of stack near %10p\n",callNum,&a[0]);
	overflowStack(callNum+1);
}

int main(int argc,char *argv[])
{
	stack_t sigstack;
	struct  sigaction sa;
	int j;

	printf("Top of standard stack is near %10p\n",(void *)&j);

	/* Allocate alternate stack and inform kernel of its existence */
	sigstack.ss_sp = malloc(SIGSTKSZ);
	if (sigstack.ss_sp == NULL) {
		perror("malloc failed");
		exit(-1);
	}
	sigstack.ss_size = SIGSTKSZ;
	sigstack.ss_flags = 0;
	if (sigaltstack(&sigstack,NULL) == -1) {
		perror("sigaltstack failed");
		exit(-1);
	}
	printf("Alternate stack is at   %10p-%p\n",sigstack.ss_sp,(char *)sbrk(0)-1);

	/* Register a handler for SIGSEGV*/
	sa.sa_handler = sigsegv_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_ONSTACK;
	if (sigaction(SIGSEGV,&sa,NULL) == -1) {
		perror("sigaction failed");
		exit(-1);
	}
	overflowStack(1);

	return 0;
}
