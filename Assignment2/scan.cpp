//Eric Austin
//30037742
//CPSC 457 Assignment 2
//May 24, 2019
//Code skeleton taken from assignment hints/description
// given at https://sites.google.com/site/2019scpsc457/assignments/assignment-2

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <regex>
#include <tuple>

#define MAX_FNAME_SIZE 512
#define MAX_FILES 1024

using namespace std;

//function for sorting
bool mySortFunction (tuple<char *, long> i, tuple<char *, int> j) {
  return (get<1>(i) >= get<1>(j));
}

int main(int argc, char ** argv) {
  //handle command line arguments
  if(argc != 3) {
    cerr << "I take two command line arguments, file extension and number\n";
    exit(-1);
  }
  //capture command line arg values
  string fileExt = argv[1];
  int numToOutput = atoi(argv[2]);
  int lenFileExt = fileExt.length();
  //open myFind
  FILE * fp = popen("./myFind", "r");
  if(fp == NULL) {
    cerr << "popen failed";
    exit(-1);
  }
  //read in all filenames that have specified file extension
  char buffer[MAX_FNAME_SIZE];
  int nFiles = 0;
  char * files[MAX_FILES];
  string pattern = "(.*)" + (("." + fileExt) + "\n");
  regex re(pattern);
  while(fgets(buffer, MAX_FNAME_SIZE, fp)) {
    int len = strlen(buffer) - 1;
    string buffString(buffer);
    if(regex_match(buffString, re)) {
      files[nFiles] = strndup(buffer, len);
      nFiles++;
    }
  }
  //close myFind
  fclose(fp);
  //data structure for filenames and sizes
  tuple<char *, int> filesWithSizes[nFiles];
  //add files with sizes to data structure for sorting
  long long totalSize = 0;
  struct stat st;
  for(int i = 0; i < nFiles; i++) {
    if(0 != stat(files[i], &st)) {
      cerr << "stat failed\n";
      exit(-1);
    }
    filesWithSizes[i] = make_tuple (files[i], st.st_size);
  }

  //time to sort
  sort(filesWithSizes, filesWithSizes + nFiles, mySortFunction);
  //max output is user input or total found
  int numToPrint = min(nFiles, numToOutput);
  //print out number of results provided by user and calc total size
  for(int i = 0; i < numToPrint; i++) {
    totalSize += get<1>(filesWithSizes[i]);
    cout << get<0>(filesWithSizes[i]) << " " << get<1>(filesWithSizes[i]) << endl;
  }
  printf("Total size: %ld\n", totalSize);
  //clean up
  for(int i = 0; i < nFiles; i++) {
    free(files[i]);
  }
  //return success
  return 0;
}
