#include <unistd.h>
#include <stdio.h>

int main() {
	fprintf(stderr, "A\n");
	fork();
	fprintf(stderr, "B\n");
	fork();
	fprintf(stderr, "C\n");
}
