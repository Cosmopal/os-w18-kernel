this is the Readme for the assignment 2 part 2
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

	- srt_tasks array:
	 stores at max 'max_srt_tasks'
	 number of srt tasks.

	- max_srt_tasks:
	defines maximum number
	of srt tasks that can run

	- srt_tasks_list_head:
	head node of linked list of
	srt task structs

	- srt_task:
	a struct which is a node in 
	the linked list, and has info
	about srt_requirement of the
	task, and the task struct of
	the task.


Changes to task_struct:
	task_struct has extra fields:
	- SRT_FLAG:
	1 if the task has srt requirements
	0 otherwise

	- srt_task_struct:
	pointer to the srt_task struct
	holding info about srt requirements

	- srt_req:
	srt requirement of the task


System Call:
	A system call rtnice is defined
	in kernel/sys.c.

	sys.c also intialises the
	list head of the linked list of
	srt tasks, and also the max_srt_tasks
	variable.


Changes to fair.c:
	pick_next_task_fair is changed 
	to first check for any tasks with
	srt requirements.


=> Details of Implementation

	System call:
	
	It takes input the pid of the task
	and the srt requirement of that task
	It then sets the srt requirement of that
	task by updating the srt_task struct 
	associated with the task_struct of that
	task.

	It them, if necessary, adds the srt_struct
	into the array and/or the linked list holding
	information about all the tasks - 
	srt_tasks and
	srt_tasks_list_head.

	It handles the following errors:
	
	- Task Doesn't exist
		ESRCH

	- Maximum number of srt tasks already
	reached
		EAGAIN

	- timeslice requirement < 1
	or > 5 seconds.
		EINVAL



	
	
	Fair.c	
	the pick_next_task_fair in fair.c
	is called when the scheduler needs to 
	pick a new task. This is if the current runnig
	task has cfs_sched_class as its scheduler
	class, which we assume is true for all processes.


	It checks the array/linked list for processes
	having soft real time requirements. If there
	exist such processes, it returns the process
	which is closest to completing its srt 
	requirement. Else, is goes on with its regular
	operation.
	
	It compares the sum_exec_runtime in the se 
	(sched_entity) field of the srt task's 
	task_struct.

	If there is a task in the list/array, which
	is finished, then it removes it from the list
	and decrements num_srt_tasks.
