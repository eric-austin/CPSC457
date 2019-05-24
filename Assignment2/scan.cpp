#include <iostream>

using namespace std;

int main() {
  //initialise file pointer for reading from piped process
  FILE *fp;
  //initialise character buffer for reading lines (ie. filepaths)
  char buffer[512];
  //start new process and pipe to file pointer
  fp = popen("./myFind", "r");
  while(fgets(buffer, 512, fp)) {
    cout << buffer;
  }
  //close input iostream
  pclose(fp);
  return 0;
}
