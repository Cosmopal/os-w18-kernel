This is the Readme for the assignment 2 part 2
 Modifying CFS Scheduler submission, by
Group ID - 
Palash Aggrawal, 2016064
Satyam Verma, 2016087

Contents:
	How to run
	Output
	About the patch
	Details of implementation
		System call
		Fair.c


=> How to run:
	-- Compile Kernel and install

	-- Go in the tests folder, and make 
		$ cd tests
		$ make
	
	-- run ./test


=> Output:
	The program test runs a long task first as normal CFS task
	and then as a soft real time task
	The time taken in both implementations in printed out.


=> About the Patch:

SRT information constants:
	Some constants are defined in 
	the include/sched.h file to store
	information about SRT tasks

	- srt_task_pid	
	  pid of the current task with
	  srt priority.

	- srt_task_req
	  srt requirement of this task

	- SRT_TASK_IS_FLAG
	  whether there is a task that
	  has any srt requirement.




System Call:
	A system call rtnice is defined
	in kernel/sys.c.


Changes to fair.c:
	pick_next_task_fair is changed 
	to first check for any tasks with
	srt requirements.


=> Details of Implementation

	System call:
	
	It takes input the pid of the task
	and the srt requirement of that task.
	It validates the input and then sets
	the global variables approriately.
	It then sets the srt requirement of that
	task by updating the srt_task struct 
	associated with the task_struct of that
	task.

	It handles the following errors:
	
	- Task Doesn't exist
		ESRCH

	- Maximum number of srt tasks already
	reached
		EAGAIN

	- timeslice requirement < 1
	or > 100 seconds.
		EINVAL



	
	
	Fair.c	
	the pick_next_task_fair in fair.c
	is called when the scheduler needs to 
	pick a new task. This is if the current runnig
	task has cfs_sched_class as its scheduler
	class, which we assume is true for all processes.

	First, the flag for srt requirements is checked.
	If it is true, then that task is checked for its
	requirements - whether it has already got them, 
	by comparing the sum_exec_runtime of that tasks's
	sched entity with the srt_req_ flag.
	If it is already finished, then the global
	flags are reset. If checks pass, then the variable
	p is set to that task.

	If a valid task was found in the above checks, then
	the normal code of CFS proceeds normally, but in the 
	end, the task which cfs was going to return is replaced
	with the srt task. this is to ensure proper enqueue/dequeue
	previous task and run queues.
