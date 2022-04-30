/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32201321
*	    Student name : Heewon Kim 
*
*   lab1_sched_types.h :
*       - lab1 header file.
*       - must contains scueduler algorithm function's declations.
*
*/

#ifndef _LAB1_HEADER_H
#define _LAB1_HEADER_H

#define RUN 1
#define READY 0
#define MAX_QUEUE_SIZE 10
#define MAX_PROC_SIZE 30

typedef struct process{
	char name;
	int id;
	int arriveTime;
	int serviceTime;
	int runTime;
	int state;
} PROCESS;

typedef struct procQueue{
	int head;
	int tail;
	int time_slice;
	PROCESS* Q;
} PROC_QUEUE;


// proc_list[]: existing process' list 
// pop funtion: implement differently on every scheduling algorithms
// push function: implement all same 

// For all: general 
int calc_serviceTime(PROCESS proc_list[]);
void mk_process(PROCESS* ptr);
int calc_queue_size(PROC_QUEUE* queue);
void proc_end(PROCESS* proc, int idx);
void show_scheduled(int** arr, PROCESS proc_list[]);
void push_proc(PROCESS new_proc, PROC_QUEUE* queue);

// For each scheduling: 
void FIFO(PROCESS proc_list[]);
void FIFO_pop(PROC_QUEUE* single_q_ptr, int t, int** array);
void RR(PROCESS proc_list[], int time_quantum);
void RR_POP(PROC_QUEUE* ptr, PROCESS proc_list[], int time, int** array, int* insert_flag);
void SPN(PROCESS* proc_list);
void SPN_POP(PROC_QUEUE* sq_ptr, int time, int** arr);
void MLFQ(PROCESS proc_list[], int MLFQ_cnt);
int MLFQ_POP(PROC_QUEUE* q_ptr, PROCESS proc_list[], int q_idx, int time, int** result_arr, int MLFQ_cnt);

#endif /* LAB1_HEADER_H*/



