/**************************************************
 * *  You are not allowed to modify the code  
 * *  Usage: ./while1 
 * *  while1: the executable code
 * ***************************************************/
 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>

int main(void){
	
	while(1){
		sleep(1);
		if(fork() == 0){
			execlp("ls","ls","NULL");
		}

	}
	return 0;
}

