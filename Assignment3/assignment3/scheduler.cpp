#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <array>
#include <queue>

using namespace std;

struct process {
  int id;
  int arrival;
  int burst;
  int remaining;
  int finished;
  int ready;
  int endtime;
  double wait;
};

int main(int argc, char ** argv) {
  //data structures
  struct process processes[30];
  //scheduler types
  string sjf = "SJF";
  string rr = "RR";
  string algo;
  int quantum = 0;
  int currrentProcess = -1;
  //handle command line arguments
  if(argc == 3) {
    algo = argv[2];
    if(strcmp((const char *)&algo, (const char *)&sjf) != 0) {
      cout << algo << " not valid, usage is scheduler [config] SJF" << endl;
      exit(-1);
    }
  } else if (argc == 4) {
    algo = argv[2];
    quantum = atoi(argv[3]);
    if(strcmp((const char *)&algo, (const char *)&rr) != 0) {
      cout << algo << " not valid, usage is scheduler [config] RR [quantum]" << endl;
      exit(-1);
    }
    if(quantum <= 1 || quantum > 100) {
      cout << "Invalid quantum, 1 < quantum <= 100\n";
      exit(-1);
    }
  } else {
    cout << "Error, usage: scheduler [config] RR quantum OR scheduler [config] SJF\n";
    exit(-1);
  }
  //open and read file
  string filename = argv[1];
  fstream configFile(filename, ios::in);
  string input = "";
  int nProcesses = 0;
  while(configFile >> input) {
    processes[nProcesses].id = nProcesses;
    processes[nProcesses].arrival = stoi(input);
    configFile >> input;
    processes[nProcesses].burst = stoi(input);
    processes[nProcesses].remaining = stoi(input);
    processes[nProcesses].finished = 0;
    processes[nProcesses].endtime = 0;
    processes[nProcesses].wait = 0;
    processes[nProcesses].ready = 0;
    nProcesses++;
  }

  if(nProcesses == 0) {
    cout << "Average waiting time = 0.000 sec.\n";
  }

  int time = 0;
  int currentQuantum = quantum;
  cout << "currentQuantum: " << currentQuantum << endl;
  int jobs_remaining = nProcesses;
  int cpu = 0;
  priority_queue <int, vector<int>, greater<int> > readyQueue;
  queue <int> RRqueue;

  cout << "Time ";
  for(int i = 0; i < nProcesses; i++) {
    cout << "P" << i << "    ";
  }
  cout << endl;
  for(int i = 0; i < 60; i++) {
    cout << "-";
  }
  cout << endl;

  if(strcmp((const char *)&algo, (const char *)&sjf) == 0) {
    while(1) {
      //exit loop if all jobs finished
      if(jobs_remaining == 0) {
        break;
      }
      //check current process on CPU if its finished
      if(currrentProcess != -1 && cpu != 0) {
        if(processes[currrentProcess].remaining == 0) {
          processes[currrentProcess].finished = 1;
          processes[currrentProcess].ready = 0;
          processes[currrentProcess].endtime = time;
          processes[currrentProcess].wait = processes[currrentProcess].endtime - processes[currrentProcess].arrival - processes[currrentProcess].burst;
          jobs_remaining--;
          cpu = 0;
          continue;
        }
      }
      //if CPU is idle or job not finished check for new processes
      for(int i = 0; i < nProcesses; i++) {
        if(processes[i].arrival == time) {
          readyQueue.push(processes[i].burst);
          processes[i].ready = 1;
        }
      }
      //if CPU is idle and RQ not empty
      if((cpu == 0) && (readyQueue.size() > 0)) {
        int next_remaining = readyQueue.top();
        readyQueue.pop();
        for(int i = 0; i < nProcesses; i++) {
          if(processes[i].remaining == next_remaining && processes[i].finished == 0) {
            currrentProcess = i;
            break;
          }
        }
        cpu = 1;
        continue;
      }
      //execute burst
      processes[currrentProcess].remaining--;
      //print stuff
      cout << time << "   ";
      for(int i = 0; i < nProcesses; i++) {
        if(processes[i].id == currrentProcess) {
          cout << ".      ";
        } else if (processes[i].ready == 1) {
          cout << "+      ";
        } else {
          cout << "       ";
        }
      }
      cout << endl;
      //increment Time
      time++;
    }
    //calc and print stats
    for(int i = 0; i < 60; i++) {
      cout << "-";
    }
    cout << endl;
    double totalWait = 0.0;
    for(int i = 0; i < nProcesses; i++) {
      cout << "P" << i << " waited " << processes[i].wait << " sec.\n";
      totalWait += processes[i].wait;
    }
    cout << "Average waiting time = " << totalWait / (double)nProcesses << " sec.\n";
  } else {
    while(1) {
      //exit loop if all jobs finished
      if(jobs_remaining == 0) {
        break;
      }
      //check current process on CPU if its finished
      if(currrentProcess != -1 && cpu != 0) {
        if(processes[currrentProcess].remaining == 0) {
          processes[currrentProcess].finished = 1;
          processes[currrentProcess].ready = 0;
          processes[currrentProcess].endtime = time;
          processes[currrentProcess].wait = processes[currrentProcess].endtime - processes[currrentProcess].arrival - processes[currrentProcess].burst;
          jobs_remaining--;
          cpu = 0;
          currentQuantum = quantum;
          continue;
        }
      }

      //if CPU is idle or job not finished check for new processes
      for(int i = 0; i < nProcesses; i++) {
        if(processes[i].arrival == time) {
          RRqueue.push(i);
          processes[i].ready = 1;
        }
      }
      //if CPU is idle and RQ not empty
      if((cpu == 0) && (RRqueue.size() > 0)) {
        currrentProcess = RRqueue.front();
        RRqueue.pop();
        cpu = 1;
      }

      //check whether quantum has expired
      if(currrentProcess != -1 && cpu != 0) {
        if(currentQuantum == 0) {
          RRqueue.push(currrentProcess);
          currentQuantum = quantum;
          cpu = 0;
          continue;
        }
      }
      //execute burst
      processes[currrentProcess].remaining--;
      //print stuff
      cout << time << "   ";
      for(int i = 0; i < nProcesses; i++) {
        if(processes[i].id == currrentProcess) {
          cout << ".      ";
        } else if (processes[i].ready == 1) {
          cout << "+      ";
        } else {
          cout << "       ";
        }
      }
      cout << endl;
      //increment Time
      time++;
      if(currrentProcess != -1) {
        currentQuantum--;
      }
    }
    //calc and print stats
    for(int i = 0; i < 60; i++) {
      cout << "-";
    }
    cout << endl;
    double totalWait = 0.0;
    for(int i = 0; i < nProcesses; i++) {
      cout << "P" << i << " waited " << processes[i].wait << " sec.\n";
      totalWait += processes[i].wait;
    }
    cout << "Average waiting time = " << totalWait / (double)nProcesses << " sec.\n";
  }


  cout << endl;

  return 0;
}
