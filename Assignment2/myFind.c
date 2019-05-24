#include <stdio.h>
#include <dirent.h>
#include <string.h>

void visitDir(const char *path) {
  DIR *directory = opendir(path);
  struct dirent* element = NULL;

  if(!directory) return;

  while(element = readdir(directory)) {
    printf("%s\n", element->d_name);
    if(element->d_type == DT_DIR) {
      if(strcmp(element->d_name, ".") && strcmp(element->d_name, "..")){
        char buffer[1000];
        sprintf(buffer, "%s/%s", path, element->d_name);
        visitDir(buffer);
      }
    }
  }
}
int main() {
  visitDir(".");
  return 0;
}
