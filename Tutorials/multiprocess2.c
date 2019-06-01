#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	printf("Before ls.\n");
	system("/bin/ls");
	printf("After ls.\n");
}
