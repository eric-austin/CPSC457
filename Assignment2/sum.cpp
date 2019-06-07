#include <iostream>
#include <thread>
#include <fstream>
#include <array>

using namespace std;

int partialSums[1000000];

void doSum(int numbers[], int id, int start, int end) {
  for(int i = start; i < end; i++) {
    partialSums[id] += numbers[i];
  }
}

int main(int argc, char ** argv) {
  //get command line args
  string fileName = argv[1];
  int numOfThreads = atoi(argv[2]);
  //set up input file stream
  fstream numberFile(fileName, ios::in);
  //variables needed
  thread threads[numOfThreads];
  long long total = 0;
  int numbers[1000000];
  int numberCount = 0;
  string currentNumber;
  //iterate through input file grabbing one number per line
  while(getline(numberFile, currentNumber)) {
    numbers[numberCount] = stoi(currentNumber);
    numberCount++;
  }
  //calculate the workload for each thread
  int group1Size = numberCount % numOfThreads;
  int group2Size = numOfThreads - group1Size;
  int group1PerThread = numberCount / numOfThreads + 1;
  int group2PerThread = numberCount / numOfThreads;
  int group1Total = group1Size * group1PerThread;
  int group2Total = group2Size * group2PerThread;
  //get the group 1 threads going
  for(int i = 0; i < group1Size; i++) {
    int start = i * group1PerThread;
    int end = start + group1PerThread;
    threads[i] = thread(doSum, numbers, i, start, end);
  }
  //get the group 2 threads going
  for(int i = group1Size; i < numOfThreads; i++){
    int start = group1Total + (i - group1Size) * group2PerThread;
    int end = start + group2PerThread;
    threads[i] = thread(doSum, numbers, i, start, end);
  }
  //join these bad boys so main thread waits for them all to finish
  for(int i = 0; i < numOfThreads; i++) {
    threads[i].join();
  }
  //print results
  for(int i = 0; i < numOfThreads; i++) {
    int threadID = i + 1;
    cout << "Thread " << threadID << ": " << partialSums[i] << endl;
    total += partialSums[i];
  }
  cout << "Sum = " << total << endl;

}
