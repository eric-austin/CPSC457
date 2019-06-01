#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	pid_t pid = fork();

	if (pid < 0) {
		fprintf(stderr, "fork failed");
		exit(-1);
	} else if (pid == 0) {
		execlp("/bin/ls", "ls", "-l", NULL);
		printf("This _should_ never print...\n");
	} else {
		printf("Waiting for child process %d\n", pid);
		while(wait(NULL) > 0);
		printf("Child finished.\n");
		exit(0);
	}
}
