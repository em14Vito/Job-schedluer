#Manual
##User Manual:
1. Using makefile. it is easy to use this scheduler by using make
command in terminal. also, you can clean things that you make before.
2. For one execution of the scheduler, it must follow one scheduling policy only. To invoke the scheduler, the following arguments are required:<br />
`$ ./scheduler [input filename] [policy]`<br />
Note:<br />
‘$’ is the shell prompt;<br />
“./scheduler” is the path to the scheduler program;<br />
“[input filename]” is the path to the job description file;<br />
“[policy]” is the scheduling policy which should take the following values:<br />
* (i) “FCFS” (which means the first come first serve policy);
* (ii) “RR 2” (which means the round robin policy) and “2” means the time quota is “2”.
(Note: don’t type the quotes.)  <br />
After invoking the scheduler, it creates a set of jobs that are described in “Job.txt”, which will be described in Section 3.3. Then, the jobs will be scheduled to execute according to the given policy.  <br />
<br />
After the jobs are terminated, the scheduler should print out the statistic information into a file “JobOut.txt”.<br />
(Note: If file JobOut.txt doesn’t exist, It would be created. Otherwise, data that producing by you repeat execute this program would be append to JobOut.txt)
3. The job description file is a plain-text file named “Job.txt”, which has the following format:<br />
Note:<br />
url
* There are at most 8 jobs stated in a job description file.
* One line represents one job, i.e., every job description is separated by one newline character  
“\n”.* A line of job description must contain three non-empty fields: “Arrival Time”,<br />  
“Command”, and “Duration”. Every two fields are separated by exactly one tab character.
* The “Arrival Time” of a job is measured relative to the time that the scheduler starts executing<br />
and is measured in terms of seconds. The smallest value is 0 and the largest is the<br />  
maximum value defined as “int MAX=10000” in the your header files.<br />
- (i) When the scheduler has just been started, the time that is relative to the time that the  
scheduler starts executing is zero.- (ii) If “Arrival Time” of a job is i, it means the job should start executing i seconds  
after the scheduler starts.  
- (iii) The “Arrival Time” means that the time that the job arrives at the system.
Whether it should be started or not depends on the scheduling policy.
* The “Command” is a command string with the following constraints:<br />
- (i) A command has a maximum length of 255 characters.
- (ii) There will be no leading or trailing space characters in any commands.
- (iii) A command is composed of a series of tokens (or words). The first token is the<br />
        program name and the successive tokens are the program arguments.<br />
        Every two tokens are separated by exactly one space character.
4. Scheduling policy are FCFS and RR
5. Suppose the job description file “Job.txt” is  
[]url
The following is the example of the scheduling report:  
url
