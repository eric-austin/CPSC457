#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE * fp = popen("/bin/ls -l", "r");
	if (fp == NULL) {
		fprintf(stderr, "popen failed.\n");
		exit(-1);
	}

	char buff[4096];
	while (fgets(buff, sizeof(buff), fp) != NULL)
		printf("%s", buff);

	pclose(fp);
}
