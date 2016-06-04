#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "parse.h"
#include "scheduler.h"

void zhengli(Jobs *a){
  a->number = 1;
}

int main(int arg,char* argv[])
{
  /******check input argment********/
  if(arg < 3 || arg > 5){
      printf("Please check parameter and run it again\n");
      return 0;
  }

  char* Filename = argv[1];
  char* Policy = argv[2];
  int quota;
  if(strcmp(Policy,"RR") == 0){
    if(arg == 4)
      quota = atoi(argv[3]);
    else{
      printf("Lossing quota time for RR scheduler.Please run it again\n");
      return 0;
    }
  }
  /**************************/

  //get the data from file and store into Jobs
  FILE* pFile;
  Jobs* all_jobs = (Jobs*)malloc(sizeof(Jobs));
  pFile = fopen (Filename , "r");
  if (pFile == NULL) perror ("Opening file error");
  else  all_jobs = __Parse(pFile);

  //calculate which time should be do first (using its arrival time)
  __Sort_jobs(all_jobs);

  printf("Start\n");

  /***********Do scheduler *******************/
  if(strcmp(Policy,"FCFS") == 0){
    FCFS(all_jobs);
  }
  else if(strcmp(Policy,"RR") == 0){
    RR(all_jobs,quota);
  
  }
  printf("Finish\n");
  return 0;
}
