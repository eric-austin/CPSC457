#include <unistd.h>
#include <stdio.h>

int main() {
	printf("Hello\n");
	fork();
	printf("world.\n");
}
