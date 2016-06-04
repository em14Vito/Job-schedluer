#include "parse.h"

//init job struct
void init_jobs(Jobs* info){
  info->number = defult_num;
  int i;
  for(i = 0; i< MAX_JOBS; i++){
    info->job[i].pid = defult_num;
    info->job[i].jid = defult_num;
    info->job[i].state = defult_num;
    info->job[i].arr_time = defult_num;
    info->job[i].dur_time = defult_num;
    info->job[i].cmd_num = defult_num;
    info->job[i].priority = defult_num;
    info->job[i].cmd[0] = NULL;
  }
}

//parsing
Jobs* __Parse(FILE * file){

   int job_index = 0, cmd_index = 0 ;
   char buffer[MAX_COM_VAR];
   Jobs* all_jobs = (Jobs*)malloc(sizeof(Jobs));
   init_jobs(all_jobs);
   
   
   while( ! feof (file) )
   {
      if ( fgets (buffer , MAX_COM_VAR , file) == NULL ) break;
      //take the token into all_jobs
      else{

        //除去\n
        if(buffer[strlen(buffer)-1] == '\n')
          buffer[strlen(buffer)-1] = '\0';

        /*****get the first string, first string is Arrival time*****/
        char * pch;
        pch = strtok (buffer,"\t");
        if(!pch) break;
        all_jobs->job[job_index].arr_time = atoi(pch);
        /*********************/

        /******get the comand and its arg.******/
        pch = strtok (NULL, "\t");
        if(!pch) break;
        char* temp_total = pch;
        /*********************/

        /******get the last string: duration time.******/
        pch = strtok(NULL, "\t");
        if(!pch) break;
        all_jobs->job[job_index].dur_time = atoi(pch);
        /*********************/

        /******record job id************/
        all_jobs->job[job_index].jid = job_index+1;
        /*********************/        

        /*****parse the total commands string****/
        char* temp_cmd = strtok(temp_total," ");
        while(1){
          
          //copy the pch(command or arg) into job
          all_jobs->job[job_index].cmd[cmd_index] = (char*)malloc( sizeof(char*) );
          strcpy(all_jobs->job[job_index].cmd[cmd_index], temp_cmd);
          all_jobs->job[job_index].cmd_num = ++cmd_index;
          temp_cmd = strtok(NULL," ");
          if(!temp_cmd) break;
        }
        /*********************/


        all_jobs->number = ++job_index;
        // printf("job number is %d\n",all_jobs->number);
        cmd_index = 0;
      }
      
      
   }
   fclose (file);
   
   return all_jobs;
  
}

//Set the jobs priority(Only use in FCFS).If arrival eariler, priority is heigher.
void __Sort_jobs(Jobs * A){
  int _priority = 0;
  int i;
  for(i=0; i < A->number; i++){
    int j;
    for(j=0; j < A->number; j++){
      //job[i] is earlier job[j]
      if(A->job[i].arr_time <= A->job[j].arr_time){
          _priority++;
      }
    }
    A->job[i].priority = _priority;
    _priority = 0;
  }
}

//Using job's priority to get the execute order. It would store in arr pointer.
void __Ordering(Jobs * info, int *arr){
  int run_order = 0;
  int i;
  for(i=0; i < info->number; i++){
    //number - proirotiy = when the job should be run
    run_order = info->number - info->job[i].priority;
    arr[run_order] = i;
  }
}