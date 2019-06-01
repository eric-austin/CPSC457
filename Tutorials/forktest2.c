#include <unistd.h>
#include <stdio.h>

int main() {
	pid_t pid = fork();
	printf("fork returned %d.\n", pid);
}
