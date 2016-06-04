//
//  parse.h
//  Scheduler
//
//  Created by vi on 16/4/29.
//  Copyright © 2016年 vi. All rights reserved.
//

// #ifndef parse_c
#ifndef parse_h
#define parse_h

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define defult_num 0
#define MAX_COM_NUM 100
#define MAX_COM_VAR 256
#define MAX_JOBS 8
#define Output_file "JobOut.txt"    //output file name

typedef struct T1
{
    int pid;                // job PID
    int jid;                // job ID 
    int state;              // TSTP, CONT, etc
    char *cmd[MAX_COM_NUM];  // command line
    int arr_time;           // arrival time
    int dur_time;           // duration time
    int cmd_num;
    int priority;           //larger number means heigher prority. Depends on arrival time.

}Job;

typedef struct T2
{
    Job job[MAX_JOBS];   // The job array   
    int number; 

}Jobs;


void init_jobs(Jobs* info);
Jobs* __Parse(FILE * file);
void __Sort_jobs(Jobs * A);
void __Ordering(Jobs * info, int *arr);


#endif /* parse_h */

