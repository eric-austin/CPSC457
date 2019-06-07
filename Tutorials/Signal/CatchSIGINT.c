#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sig_handler(int signo) {
	if(signo == SIGINT)
		printf("OUCH! - I got signal\n");
}

int main(void) {
	if(signal(SIGINT, sig_handler) == SIG_ERR)
		printf("\ncan't catch SIGINT\n");

	while(1) {
		printf("Hi\n");
		sleep(1);
	}

	return 0;
}
