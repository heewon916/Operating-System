/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32201321
*	    Student name : Heewon Kim
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm function'definition.
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
 * you need to implement FCFS, RR, SPN, HRRN, MLFQ scheduler. 
 */
// set up processes by User's INPUT: especially arrival Time and service Time
int process_num;

void mk_process(PROCESS* ptr){ // ptr: proc_list will be sended
	printf("[INPUT] number of process:: ");
	scanf("%d", &process_num);

	PROCESS* tmp_proc = (PROCESS*)malloc(sizeof(PROCESS)*process_num);

	// insert processes in process_list: implemented in lab1_sched_test.c
	for(int i=0; i<process_num; i++){
		tmp_proc[i].name = 'A'+i;
		tmp_proc[i].id = i;
		tmp_proc[i].state = READY;
		printf("--PROCESS %c--\n", tmp_proc[i].name);
		printf("\t[INPUT] Arrived Time ::"); scanf("%d", &tmp_proc[i].arriveTime);
	       	printf("\t[INPUT] Service Time ::"); scanf("%d", &tmp_proc[i].serviceTime);
		*(ptr+i) = tmp_proc[i];
	}
	free(tmp_proc);
}

// calculate service Time of each process
int calc_serviceTime(PROCESS proc_list[]){
	int i=0, j=0;
	int sum_time = 0;
	PROCESS* tmp_proc; // to get service Time from every process
	PROCESS tmp;

	// move proc_list info. to tmp_proc pointer
	tmp_proc = (PROCESS*)malloc(sizeof(PROCESS)*process_num);
	for(i; i<process_num; i++){
		tmp_proc[i] = proc_list[i];
	}
	// sort process by arriveTime
	for(i = 0; i< process_num; i++){
		for (j = i; j < process_num; j++){
			if (tmp_proc[i].arriveTime > tmp_proc[j].arriveTime){
				tmp = tmp_proc[i];
				tmp_proc[i] = tmp_proc[j];
				tmp_proc[j] = tmp;
			}
		}
	}
	// add to sum_time for each process's service time: 최종으로 끝나는 서비스 시간을 구함 
	sum_time = tmp_proc[0].arriveTime; //가장 처음 도착한 잡의 도착 시간에다가 
	// 모든 프로세스의 서비스 받아야 하는 시간을 더해줌 
	for(i=0; i<process_num-1; i++){ 
		sum_time += tmp_proc[i].serviceTime;
		// 만약에 내 뒤에 있는 잡의 도착 시간이 전체 서비스 시간보다 크면 
		if (tmp_proc[i+1].arriveTime > sum_time){
			sum_time += tmp_proc[i+1].arriveTime - sum_time;
		}
	}
	sum_time += tmp_proc[process_num-1].serviceTime;

	// free malloc memeory
	free(tmp_proc);
	return sum_time;
}

// calculate how many processes need to be scheduled
int calc_queue_size(PROC_QUEUE* queue){
	PROCESS* ptr = queue->Q;
	int cnt = 0;
	int i = 0;
	for(; i< MAX_QUEUE_SIZE; i++){
		if (ptr[i].serviceTime > 0) 
			cnt++;
	}
	return cnt;
}
// set the process in the proc_list on the state end 
void proc_end(PROCESS* proc, int idx){
	proc[idx].id = -1;
	proc[idx].arriveTime = 0;
	proc[idx].serviceTime = 0;
	proc[idx].name = ' ';
	proc[idx].runTime = 0;
	proc[idx].state = READY;
}
// show the graphic how the processes are scheduling --> can't understand
void show_scheduled(int** arr, PROCESS proc_list[]){
	int i=0, k=0;
	int h = process_num;
	int w = calc_serviceTime(proc_list);
	for (; i<h; i++){
		printf("%c ", proc_list[i].name);
		for(int j=0; j<w; j++){
			if (arr[i][j] == 0) printf("- ");
			else printf("O ");
		}
		printf("\n");
	}
}
// insert process into the ready_queue
void push_proc(PROCESS new_proc, PROC_QUEUE* queue){
	int head = queue->head;
	int tail = queue->tail;

	if ((tail+1) % MAX_QUEUE_SIZE == head){
		printf("[ERROR] :: Queue is full\n");
		return;
	}
	new_proc.runTime = 0;
	queue->Q[tail] = new_proc;
	queue->tail = (tail+1) % MAX_QUEUE_SIZE;
}
// Do FIFO scheduling
void FIFO(PROCESS proc_list[]) {
	int i=0, j=0;
	int time = 0;
	int row = process_num;
	int col = calc_serviceTime(proc_list);

	int** result_arr = (int**)malloc(sizeof(int*)*row);
	for(; i<row; i++){
		result_arr[i] = (int*)malloc(sizeof(int)*col);
	}
	for(i=0; i<row; i++){
		for(j=0; j<col; j++){
			result_arr[i][j] = 0;
		}
	}

	PROCESS* ready_queue = (PROCESS*)malloc(sizeof(PROCESS)*MAX_QUEUE_SIZE);
	PROC_QUEUE single_q = {0, };
	single_q.head = 0;
	single_q.tail = 0;
	single_q.Q = ready_queue;
	
	for(int i=0; i<MAX_QUEUE_SIZE; i++)
		proc_end(ready_queue, i);

	for(i=0; i<process_num; i++){
		proc_list[i].state = READY;
	}

	// start FIFO 
	for(; time < calc_serviceTime(proc_list); time++){
		for(i=0; i<process_num; i++){
			if (proc_list[i].state == READY && proc_list[i].arriveTime <= time){
				push_proc(proc_list[i], &single_q);
				proc_list[i].state = RUN;
			}
		}
		// if queue size is not 0: if some job is left
		if(calc_queue_size(&single_q))
			FIFO_pop(&single_q,  time, result_arr);
	}
	show_scheduled(result_arr, proc_list);

	for(i=0; i<row; i++) 
		free(result_arr[i]);
	free(ready_queue);
	
}
void FIFO_pop(PROC_QUEUE* single_q_ptr, int t, int** array){
	int i=0;
	int head = single_q_ptr->head;
	int tail = single_q_ptr->tail;

	PROCESS* q_ptr = single_q_ptr->Q;
	int pid = q_ptr[head].id;

	q_ptr[head].serviceTime--;
	if (q_ptr[head].serviceTime == 0){
		array[pid][t] = 1;
		proc_end(q_ptr, head);
		single_q_ptr->head = (head+1) % MAX_QUEUE_SIZE;
	}
	array[pid][t] = 1;
}
//Do RR scheduling
void RR(PROCESS proc_list[], int time_quantum)
{
	int total_ST = calc_serviceTime(proc_list);
	int i, j, time;
	int insert_flag = 0;

	//create result array
	int** result_arr = (int**)malloc(sizeof(int*) * process_num);
	for (i = 0; i < process_num; i++) result_arr[i] = (int*)malloc(sizeof(int) * total_ST);

	for (i = 0; i < process_num; i++) for (j = 0; j < total_ST; j++) result_arr[i][j] = 0;

	for (i = 0; i < process_num; i++) proc_list[i].state = READY;

	PROC_QUEUE single_q = { 0, }; //RR uses one queue
	PROCESS* queue = (PROCESS*)malloc(sizeof(PROCESS) * MAX_QUEUE_SIZE);
	single_q.head = 0;
	single_q.tail = 0;
	single_q.Q = queue;
	single_q.time_slice = time_quantum;

	for (i = 0; i < MAX_QUEUE_SIZE; i++) proc_end(queue, i);

	//Doing simulation
	for (time = 0; time < calc_serviceTime(proc_list); time++) {
		for (j = 0; j < process_num; j++) {
			if (proc_list[j].arriveTime == time) { //check by process arrival time
				if(!insert_flag) push_proc(proc_list[j], &single_q);
			}
		}
		insert_flag = 0;
		if (calc_queue_size(&single_q)) RR_POP(&single_q, proc_list, time, result_arr, &insert_flag);
		else continue;
	}

	show_scheduled(result_arr, proc_list);

	for (i = 0; i < process_num; i++) free(result_arr[i]);
	free(queue);
}

//pop rr's queue by param of singleQ pointer
void RR_POP(PROC_QUEUE* sq_ptr, PROCESS proc_list[], int time, int** result_arr, int* flag) {
	int i = 0;
	int head = sq_ptr->head;
	int tail = sq_ptr->tail;
	int time_quantum = sq_ptr->time_slice;
	PROCESS* queue_pointer = sq_ptr->Q;
	int pid = queue_pointer[head].id;
	int run_time = queue_pointer[head].runTime;

	if (head == tail) printf("ERROR :: Queue is empty"); //error exception for empty queue

	queue_pointer[head].serviceTime--; //if process run - reduce svt, plus runT
	queue_pointer[head].runTime++;

	if (queue_pointer[head].serviceTime == 0) { //process end
		result_arr[pid][time] = 1;
		proc_end(queue_pointer, head);
		sq_ptr->head = (head + 1) % MAX_QUEUE_SIZE;
	}
	else {
		if (run_time < time_quantum - 1) { //if time quantum is enough
			result_arr[pid][time] = 1;
			return;
		}
		for (; i < process_num; i++) {
			if (proc_list[i].arriveTime == time + 1) { //check by arrival time
				push_proc(proc_list[i], sq_ptr);
				*flag = 1;
			}
		}
		push_proc(queue_pointer[head], sq_ptr);
		result_arr[pid][time] = 1;
		proc_end(queue_pointer, head);
		sq_ptr->head = (head + 1) % MAX_QUEUE_SIZE;
	}
}

// Do SPN: non-preemptive
void SPN(PROCESS* proc_list) {
	int i=0, j=0, time = 0; 
	int total_ST = calc_serviceTime(proc_list);
	
	int** result_arr = (int**)malloc(sizeof(int*)*process_num);
	for(; i< process_num; i++){
		result_arr[i] = (int*)malloc(sizeof(int)*total_ST);
	}
	for(i=0; i<process_num; i++){
		for(; j<total_ST; j++){
			result_arr[i][j] = 0;
		}
	}
	PROCESS* ready_queue = (PROCESS*)malloc(sizeof(PROCESS)*MAX_QUEUE_SIZE);
	PROC_QUEUE single_q = {0, };
	single_q.head = 0;
	single_q.tail = 0;
	single_q.Q = ready_queue;
	
	for(i=0; i<MAX_QUEUE_SIZE; i++){
		proc_end(ready_queue, i);
	}
	for(i=0; i<process_num; i++){
		proc_list[i].state = READY;
	}

	//printf("total_ST = %d\n",total_ST); 
	for(time=0; time<total_ST; time++){
		for(i=0; i<process_num; i++){
			if(proc_list[i].state == READY && proc_list[i].arriveTime <= time){
				push_proc(proc_list[i], &single_q);
				proc_list[i].state = RUN;
			}
		}
		//printf("proc_list[%d].arriveTime = %d\n", time, proc_list[time].arriveTime);
		//printf("\nQ_size = %d\n",calc_queue_size(&single_q)); 
		if(calc_queue_size(&single_q)){
			SPN_POP(&single_q, time, result_arr);
		}
	}
	show_scheduled(result_arr, proc_list);
	free(ready_queue);
	for(i=0; i<process_num; i++){
		free(result_arr[i]);
	}

}
void SPN_POP(PROC_QUEUE* sq_ptr, int time, int** arr){
	int i=0; 
	int head = sq_ptr->head;
	int tail = sq_ptr->tail;

	PROCESS* q_ptr = sq_ptr->Q;
	int pid = q_ptr[head].id;
	//printf("\n--%c--\n", q_ptr[head].name);

	q_ptr[head].serviceTime--;
	arr[pid][time] = 1;
	if (q_ptr[head].serviceTime == 0){
		//arr[pid][time] = 1;
		proc_end(q_ptr, head);
		sq_ptr->head = (head+1) % MAX_QUEUE_SIZE;
	}
	/*
	PROCESS* tmp;
	for(i=0; i<calc_queue_size(q_ptr)-1; i++){
		index = i;
		for(j=i+1; j<calc_queue_size(q_ptr);j++){
			if(q_ptr[index].serviceTime > q_ptr[i].serviceTime){
				tmp = q_ptr[index];
				q_ptr[index] = q_ptr[i];
				q_ptr[i] = tmp;
			}	
		}
	}
	*/

}

// Do MLFQ
void MLFQ(PROCESS proc_list[], int MLFQ_cnt) {
	int total_ST = calc_serviceTime(proc_list);
	int i = 0,j = 0;
	int q_cnt = 0;
	int q_idx, proc_idx, time;
	int end_flag = 0;
	int cont_flag = 0;
	int power_val;

	int** result_arr = (int**)malloc(sizeof(int*) * process_num);
	for (i; i < process_num; i++) 
		result_arr[i] = (int*)malloc(sizeof(int) * total_ST);
	for (i = 0; i < process_num; i++) {
		for (j = 0; j < total_ST; j++) 
			result_arr[i][j] = 0;
	}
	for (i = 0; i < process_num; i++) {
		proc_list[i].state = READY;
	}

	PROC_QUEUE* mlfq  = (PROC_QUEUE*)malloc(sizeof(PROC_QUEUE) * MLFQ_cnt);
	
	for (q_idx = 0; q_idx < MLFQ_cnt; q_idx++) {
		power_val = 1;
		mlfq[q_idx].head = 0;
		mlfq[q_idx].tail = 0;
		mlfq[q_idx].time_slice = 1;
		for(j=0; j<q_idx; j++){ //is same with pow(2, q_idx);
			power_val *= 2;
		}
		mlfq[q_idx].time_slice = power_val;
		mlfq[q_idx].Q = (PROCESS*)malloc(sizeof(PROCESS) * MAX_QUEUE_SIZE);
		for (i = 0; i < MAX_QUEUE_SIZE; i++)	
			proc_end(mlfq[q_idx].Q, i);
	}

	//Doing simulation
	for (time = 0; time < total_ST; time++) {
		cont_flag = 0;
		end_flag = 0;
		for (j = 0; j < process_num; j++) {
			if ((proc_list[j].state == READY) && (proc_list[j].arriveTime <= time)) { 
				proc_list[j].state = RUN;
				push_proc(proc_list[j], &mlfq[0]);
			}
		}
		for (q_idx = 0; q_idx < MLFQ_cnt; q_idx++) { 
			for (proc_idx = 0; proc_idx < calc_queue_size(&mlfq[q_idx]); ) {
				cont_flag = MLFQ_POP(mlfq, proc_list, q_idx, time, result_arr, MLFQ_cnt); 
				if (cont_flag == 1) { 
					time++;
					for (j = 0; j < process_num; j++) { 
						if ((proc_list[j].state == READY) && (proc_list[j].arriveTime <= time)) { 
							proc_list[j].state = RUN;
							push_proc(proc_list[j], &mlfq[0]);
						}
					}
					continue;
				}
				else {
					end_flag = 1;
					break;
				}
			}
			if (end_flag) 
				break;
		}
	}

	show_scheduled(result_arr, proc_list);

	for (i = 0; i < process_num; i++) free(result_arr[i]);
	free(mlfq);
}

int MLFQ_POP(PROC_QUEUE* q_ptr, PROCESS proc_list[], int q_idx, int time, int** result_arr, int MLFQ_cnt) {
	int i,j;
	int proc_cnt = 0;
	int flag = 0;
	int head = q_ptr[q_idx].head;
	int tail = q_ptr[q_idx].tail;
	int run_time = q_ptr[q_idx].Q[head].runTime;
	int pid = q_ptr[q_idx].Q[head].id;

	if (head == tail) {
		printf("ERROR :: Queue is empty\n"); 
		return -1; 
	}

	// stay 
	if (run_time < q_ptr[q_idx].time_slice - 1) { 
		q_ptr[q_idx].Q[head].serviceTime--;
		// process finish
		if (q_ptr[q_idx].Q[head].serviceTime == 0) { 
			q_ptr[q_idx].head = (head + 1) % MAX_QUEUE_SIZE;
			proc_end(q_ptr[q_idx].Q, head);
			result_arr[pid][time] = 1;
			return 0;
		}
		else {
			q_ptr[q_idx].Q[head].runTime = run_time + 1;
			result_arr[pid][time] = 1;
			return 1;
		}
	}
	else { //move to lower level
		q_ptr[q_idx].Q[head].serviceTime--;
		// process end
		if (q_ptr[q_idx].Q[head].serviceTime == 0) { 
			q_ptr[q_idx].head = (head + 1) % MAX_QUEUE_SIZE;
			proc_end(q_ptr[q_idx].Q, head);
			result_arr[pid][time] = 1;
			return 0;
		}
		else {
			for (i = 0; i < MLFQ_cnt; i++) 
				proc_cnt += calc_queue_size(&q_ptr[i]); 

			if (proc_cnt == 1) { //if only one process exist 
				// stay in current queue
				for (j = 0; j < process_num; j++) {
					// if new process arrive:: insert new one first
					if (proc_list[j].arriveTime == time + 1) { 
						flag = 1;
						push_proc(proc_list[j], &q_ptr[0]);
						proc_list[j].state = RUN;
					}
				}
				if (flag) {
					if (q_idx == MLFQ_cnt - 1) {
						result_arr[pid][time] = 1;
						push_proc(q_ptr[q_idx].Q[head], &q_ptr[q_idx]);
						proc_end(q_ptr[q_idx].Q, head);
						q_ptr[q_idx].head = (head + 1) % MAX_QUEUE_SIZE;
						return 0;
					}
					push_proc(q_ptr[q_idx].Q[head], &q_ptr[q_idx + 1]);
					q_ptr[q_idx].head = (head + 1) % MAX_QUEUE_SIZE;
					proc_end(q_ptr[q_idx].Q, head);
					result_arr[pid][time] = 1;
					return 0;
				}
				else { 
					result_arr[pid][time] = 1;
					return 0;
				}
			}
			// exception 
			if (q_idx == MLFQ_cnt - 1) { 
				for (j = 0; j < process_num; j++) {
					// insert new process first
					if (proc_list[j].arriveTime == time + 1) { 
						push_proc(proc_list[j], &q_ptr[0]);
						proc_list[j].state = RUN;
					}
				}
				// do RR scheduling
				push_proc(q_ptr[q_idx].Q[head], &q_ptr[q_idx]); 
			}
			else {
				push_proc(q_ptr[q_idx].Q[head], &q_ptr[q_idx + 1]);
			}
			q_ptr[q_idx].head = (head + 1) % MAX_QUEUE_SIZE;
			proc_end(q_ptr[q_idx].Q, head);
			result_arr[pid][time] = 1;
			return 0;
		}
	}
}

