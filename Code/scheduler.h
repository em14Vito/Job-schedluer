#ifndef scheduler_h
#define scheduler_h

#include "parse.h"
#define BUFFER_SIZE 2048
enum job_state {
    Abnormal_stop = 0,
    Normal_stop =1,
    Continues = 2,
    Exist = 3,
    Time_out = 4,
    //记录job是否完成的状态
    Suspend= 5,
    Finish = 6,
    True = 7,
    False = 8
};

void sig_pause(int sig);
void sig_handler(int sig);
int Check_jobs_finish(Jobs *info);
int Check_jobs_exist(Jobs *info);
void FCFS(Jobs * info);
void RR(Jobs * info,int per_time);

#endif
