#include <unistd.h>
#include <stdio.h>

int main() {
	for(int i = 0; i < 4; i++)
		fork();

	printf("X");
}
