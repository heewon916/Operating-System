/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32201321
*	    Student name : Heewon Kim
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm test code.
*
*/

#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "lab1_sched_types.h"

/*
 * you need to implement scheduler simlator test code.
 *
 */

int main(int argc, char *argv[]){
	PROCESS proc_list[MAX_PROC_SIZE];
	mk_process(proc_list);
	printf("\nFIFO\n");
	FIFO(proc_list);
	printf("\nRR(q=1)\n");
	RR(proc_list,1);
	printf("\nRR(q=4)\n");
	RR(proc_list, 4);
	printf("\nSPN\n");
	SPN(proc_list);
	printf("\nMLFQ(q=1)\n");
	MLFQ(proc_list, 1);
	printf("\nMLFQ(q=4)\n");
	MLFQ(proc_list, 4);
	return 0;

}

