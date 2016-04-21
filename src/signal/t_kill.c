#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int main(int argc,char *argv[])
{
	int ret,sig;
	long pid;

	if (argc != 3) {
		printf("Usage: %s sig-num pid\n", argv[0]);
		exit(-1);
	}

	sig = atoi(argv[1]);
	pid = atol(argv[2]);
	ret = kill(pid,sig);
	printf("ret=%d\n", ret);

	if (sig != 0) {
		if(ret == -1){
			perror("kill failed");
			exit(-1);
		}
	} else {
		if (ret == 0) {
			printf("Process exists and we can send it a signal\n");
		} else {
			if (errno == EPERM) {
				printf("Process exists, don't have permission to send a signal to it\n");
			} else if(errno == ESRCH) {
				printf("Process does not exist\n");
			} else {
				perror("kill failed");
				exit(-1);
			}
		}
	}
	return 0;
}
