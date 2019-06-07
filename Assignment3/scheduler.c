#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char ** argv) {
  if(argc == 3) {
    char algo[3] = "   ";
    algo = argv[2];
    if(!(strcmp(algo, "SJF"))) {
      printf("Usage: scheduler [config file] SJF, %s not valid\n", &algo);
    }
  }
}
