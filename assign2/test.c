#include <stdio.h>
#include <sys/syscall.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>


void long_exec(){
	struct timespec tstart = {0,0};
	struct timespec tend = {0,0};
	printf("getting times\n");
	clock_gettime(CLOCK_REALTIME,&tstart);
	int ans =0;
	int i =0,j=0,k=0;
	printf("starting task\n");
	for (i = 0; i< 1000; i++){
	//printf("iteratig i\n");
		for (j = 0; j<1000;j++){
			for (k = 0; k<1000;k ++){
				if (k%2 == 0){
					i++;
				}
				else {	
					i--;
				}
			}
		}
	}
	clock_gettime(CLOCK_REALTIME,&tend);
	printf("The long computation took about %.f5 milliseconds\n",
		(double)tend.tv_sec*1000 + 1.0e-6*tend.tv_nsec - 
		(double)tstart.tv_sec*1000 + 1.0e-6*tstart.tv_nsec);
}

int main(){
	printf("Starting long exec\n");
	long_exec();
	printf("Ending long exec\n");
	
	printf("Starting srt long exec\n");
	pid_t p = getpid();
	printf("Pid of this task = %d",p);
	int e = syscall(323,p,4000000000);
	if (e<0){
		perror("Error running system call: \n");
	}
	else{
		long_exec();
		printf("Ended srt long exec");
	}
} 
