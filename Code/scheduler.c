#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

#include "parse.h"
#include "scheduler.h"

int job_pid;
int state= 100;

void sig_pause(int sig){
  // raise(SIGSTOP);
  kill(job_pid,SIGSTOP);

}
void sig_handler(int sig){
  //alarm结束了
  if(sig == SIGALRM){
    //abnormal termination
    // printf("old alarm function\n" );
    raise(SIGTERM);
  }
}


//return: 1 means all job had finished, otherwise, some job still need to do.
int Check_jobs_finish(Jobs *info){
  int S = 1;
  int i;
  for(i=0;i < info->number;i++){
    if(info->job[i].state != Finish){
      S = 0;
      break;
    }
  }
  return S;
}
//return: 1 means some jobs are suspend, otherwise, some job still need to do.
int Check_jobs_exist(Jobs *info){
  int S = 0;
  int i;
  for(i=0;i < info->number;i++){
    if(info->job[i].state == Exist){
      S = 1;
      break;
    }
  }
  return S;
}

void FCFS(Jobs * info){

  FILE * Ofile = fopen (Output_file , "aw+");
  fprintf(Ofile,"Start schedlue (FCFS)\n");
  int timer = 0;
  int job_num = info->number;
  int run_order[MAX_JOBS] = {-1};
  // int job_pid[MAX_JOBS] = {0};
  int parent;

  parent = getpid();
	//sort the running order. using index in info by job_num
	__Ordering(info,run_order);

	//FCFS: start to deal with job by run_order;
	if(getpid() == parent){
		printf("start FCFS schedule\n");
    //check the job's start time. Using sleep function to let the timer equal to job's time
		int i;
    for(i=0; i < job_num; i++){
			int start = info->job[ run_order[i] ].arr_time;
			//no equal
			if(start != timer){
				if(start > timer){
					sleep( start - timer);
          timer = start;
				}
        else{
          // timer is later than start time. we don't need to sleep paret
        }
			}

      /********start to deal with job*********/
			int temp = fork();
      info->job[ run_order[i] ].pid = temp;
      //child process
			if(temp == 0){
				//execute job
        char *cmd = info->job[ run_order[i] ].cmd[0];
        info->job[ run_order[i] ].cmd[info->job[ run_order[i] ].cmd_num] = NULL;
        execvp(cmd,info->job[ run_order[i] ].cmd);
		  }
      //parent
      else{
        fprintf(Ofile,"%d sec: Job %d run\n",timer,info->job[ run_order[i] ].jid );
        //sleep the parent while job is running        
        sleep(info->job[ run_order[i] ].dur_time);
        kill(info->job[ run_order[i] ].pid,SIGINT);
      }
      /*********parent wait process********/
      // pid_t   pid;
      // int     stat;
      wait(NULL);

      timer += info->job[ run_order[i] ].dur_time;
      fprintf(Ofile,"%d sec: Finish! Job %d terminated \n",timer,info->job[ run_order[i] ].jid );      //   printf("%d sec: Finish! Job %d terminated \n",timer,info->job[ run_order[i] ].jid );
		}
	}
  fprintf(Ofile,"End\n\n\n");
}

void RR(Jobs * info,int per_time){

  signal(SIGALRM,sig_pause);
  FILE * Ofile = fopen (Output_file , "aw+"); 
  fprintf(Ofile,"Start schedlue (RR)\n");
  // signal(SIGCHLD,sig_chld);
  signal(SIGCHLD,SIG_IGN);
  int timer = 0;  //init timer
  int run_time = per_time;
  int job_num = info->number;
  int parent;
  int no_job_schedule = True;

  parent = getpid();
  //job do not need to sort 

  //check each job. If all jobs finished, stop while statement
  while(1){
    int i;
    for(i = 0; i < job_num;i++){
      //job had finished
      if(info->job[i].state == Finish){
        continue;
      }
      /******check whether it should sleep 1 second to wait all jobs**********/
      int start = info->job[i].arr_time;
      //timer is eariler than arr_time
      if(timer < start){
        //jump to next job
        no_job_schedule = True;
        continue;
      }
      else{
        //there is a job that should run. Do not need to sleep.
        no_job_schedule = False;
      }
      /*************************/

      /**********Set job's process time**************/
      //set running time
      //dur_time > running time
      if( info->job[i].dur_time > per_time ){
        info->job[i].dur_time -= per_time;
        run_time = per_time;
        //job don't finish。its state is exist
        info->job[i].state = Exist; 
      }
      ///dur_time < running time
      //说明该进程在alarm结束后要直接终止掉
      else{
        run_time = info->job[i].dur_time;
        info->job[i].dur_time = 0; //不再需要执行
        info->job[i].state = Finish; //job完成。状态为finish
      }
      /**************************/


      /************processing job  ****************/
      //job had fork before. So it must be continued
      if(info->job[i].pid != 0){
        job_pid = info->job[i].pid;
        fprintf(Ofile,"%d sec: Job %d continued \n",timer,info->job[i].jid);
        kill(info->job[i].pid,SIGCONT);
      }
      //job doesn't have its own pid(process)
      else{
          fprintf(Ofile,"%d sec: Job %d run \n",timer,info->job[i].jid );
          //Child: 创建进程去执行job
          job_pid = fork();
          info->job[i].pid = job_pid;
          //child
          if(job_pid == 0){
            /*******执行job*******/
            char *cmd = info->job[i].cmd[0];
            info->job[i].cmd[info->job[i].cmd_num] = NULL;
            execvp(cmd,info->job[i].cmd);
            /**********************/
          }
      }
      //Parent: store job's pid
      //set clock. sleep parents
      alarm(run_time);
      sleep(run_time);
      timer += run_time;

      //**********check the child's waiting state ************//
      pid_t   pid;
      int     stat;

      pid = waitpid(info->job[i].pid,&stat, WNOHANG | WUNTRACED); //WNOHANG代表不等待
      //process only have two state: normal stop or suspent!
      //暂停
      if( WIFSTOPPED(stat) ){
        state = Suspend;
      }
      //normal stop
      else{
        alarm(0);
        state = Normal_stop;
      }
      /***********************************/

      //********Print detail about scheduler ***************//
      //父进程开始输出语句。先判断这个job是不是正常退出。
      if(state == Normal_stop){
        
        // info->job[i].state = Finish;
          if(info->job[i].state == Finish){
          fprintf(Ofile,"%d  sec: Finish! Job %d terminated \n",timer,info->job[i].jid);
        }
        //process sleep 
        else if(info->job[i].state == Exist){
          fprintf(Ofile,"%d sec: Job %d suspended \n",timer,info->job[i].jid );
        }
      }
      else if(state == Suspend){
        //Finish means the process stop by forcing
        if(info->job[i].state == Finish){
          fprintf(Ofile,"%d sec: Finish! Job %d terminated \n",timer,info->job[i].jid);
          kill(info->job[i].pid,SIGKILL);
        }
        //process sleep 
        else if(info->job[i].state == Exist){
          fprintf(Ofile,"%d sec: Job %d suspended \n",timer,info->job[i].jid );
        }
      }
      /***********************************/

    }//for statement end

    //check whether job had finish
    if( Check_jobs_finish(info) == 1){
      break;
    }
    //let parent sleep 1 second ant it would make sure all process would schedule
    if(no_job_schedule == True ){
      if(Check_jobs_exist(info) != 1){
        timer++;
        sleep(1);
      }
    }
  }

  fprintf(Ofile,"End\n\n\n");
}
