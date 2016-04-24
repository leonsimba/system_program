#define _GNU_SOURCE		/*Get strsignal() declaration frm <string.h>*/
#include <string.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

/*set to 1 once "env" buffer has been initialized by [sig]setjmp*/
static volatile sig_atomic_t canJump = 0;
#ifdef USE_SIGSETJMP
static sigjmp_buf senv;
#else
static jmp_buf env;
#endif

void print_sigmask(FILE *of, char *msg)
{
	int sig,cnt=0;
	sigset_t currMask;

	if (msg != NULL)
		fprintf(of, "%s",msg);

	if (sigprocmask(SIG_BLOCK,NULL,&currMask) == -1)
		return;

	for (sig = 1; sig<NSIG; sig++) {
		if (sigismember(&currMask,sig)) {
			cnt++;
			fprintf(of,"%d (%s)\n", sig, strsignal(sig));
		}
	}
	if(cnt == 0)
		fprintf(of, "<empty signal set>\n");
}

static void handler(int sig)
{
	printf("Received signal %d (%s)\n", sig, strsignal(sig));
	printf("signal mask is:\n");
	print_sigmask(stdout,NULL);

	if (!canJump) {
		printf("'env' buffer not yet set,doing a simple return");
		return;
	}

#ifdef USE_SIGSETJMP
	siglongjmp(senv,1);
#else
	longjmp(env,1);
#endif
}

int main(int argc,char *argv[])
{
	struct sigaction sa;

	print_sigmask(stdout,"Signal mask at startup: \n");
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handler;

	if (sigaction(SIGINT,&sa,NULL) == -1) {
		perror("sigaction failed");
		exit(-1);
	}

#ifdef USE_SIGSETJMP
	printf("Calling sigsetjmp to set a jump-point\n");
	if (sigsetjmp(senv,1) == 0) {
#else
	printf("Calling setjmp to set a jump-point\n");
	if (setjmp(env) == 0) {
#endif
		canJump = 1;
	} else {
		print_sigmask(stdout,"After jump from handler,signal mask is: \n");
	}

	for (;;)
		pause();
}
