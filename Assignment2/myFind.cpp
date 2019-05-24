#include <dirent.h>
#include <iostream>
#include <cstring>

using namespace std;

//function to recursively visit directories starting from current
//working directory and print out all file names to std out
void visitDirectory(const char *path) {
  //initialise directory pointer to NULL
  DIR *dirPtr = NULL;
  //initialise struct dirent to be used for each directory entry opened to NULL
  struct dirent* currentEntry = NULL;
  //set directory pointer to directory opened with path passed to function
  dirPtr = opendir(path);
  //if the directory pointer is not an address (ie it's NULL) then return
  if(!dirPtr) {
    cout << "error opening directory\n";
    return;
  }
  //loop through contents of the directory
  while((currentEntry = readdir(dirPtr))) {
    //first create the full path for entry
    char fullPath[512];
    sprintf(fullPath, "%s/%s", path, currentEntry->d_name);
    //then check whether current entry is directory for recursion
    if(currentEntry->d_type == DT_DIR) {
      //make sure we don't recurse (infinitely) via . or ..
      if(strcmp(currentEntry->d_name, ".") && strcmp(currentEntry->d_name, "..")) {
        visitDirectory(fullPath);
      }
      //if not a directory, simply print out the full path
    } else {
      cout << fullPath << endl;
    }
  }
  //close directory
  closedir(dirPtr);

}

int main() {
  visitDirectory(".");
  return 0;
}
