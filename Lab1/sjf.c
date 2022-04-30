#include <stdio.h>
#include <stdlib.h>

#define SIZE 5
#define MAX_Q_SIZE 10

typedef struct proc_info{
	int p_name;
	int arrivalTime;
	int burstTime;
} PROC_INFO;

typedef struct P_QUEUE{	
	int head;
	int tail;
	PROC_INFO* Q;
} P_QUEUE;

void swap(int i, int idx, PROC_INFO* arr){
	PROC_INFO tmp = arr[i];
	arr[i] = arr[idx];
	arr[idx] = tmp;

}
void push(PROC_INFO new_p, P_QUEUE* queue){
	queue->tail = (queue->tail+1) % MAX_Q_SIZE; 
	queue->Q[queue->tail] = new_p;
}
void sort_q(P_QUEUE* queue){
	PROC_INFO* Q = queue->Q;
	int index;
	for(int i=0; i<MAX_Q_SIZE-1; i++){
		index = i;
		for(int j=i+1; j<MAX_Q_SIZE; j++){
			if(Q[i].burstTime < Q[index].burstTime){
				index = j;
			}
		}
		swap(i, index, queue->Q);
	}
}
int is_empty(P_QUEUE* queue){
	if (queue->head == queue->tail){
		return 1;
	}
	else return 0;
}
PROC_INFO* pop(P_QUEUE* queue){
	PROC_INFO tmp;
	if(!is_empty(queue)){
		tmp = queue->Q[queue->head];
		queue->head = (queue->head+1) % MAX_Q_SIZE;
	}
	return &tmp;
}
void SJF (PROC_INFO arr[]){
	int index = 0; // used to sort the process according to arrivalTime
	for(int i=0; i<SIZE -1; i++){
		index = i;
		for(int j=i+1; j < SIZE; j++){
			if(arr[j].arrivalTime < arr[index].arrivalTime){
				index = j;
			}
		}
		swap(i, index, arr);
	}

	// ctime stores the current run time
	int ctime = arr[0].arrivalTime;

	PROC_INFO* pq = (PROC_INFO*)malloc(sizeof(PROC_INFO)*MAX_Q_SIZE);
	P_QUEUE* ready_queue;
	ready_queue->head = 0; 
	ready_queue->tail = 0;
	ready_queue->Q = pq;
	
	int temp = arr[0].arrivalTime;

	push(arr[0], ready_queue); 
	arr[0].arrivalTime = -1;

	printf("p_name\tarrival time\tburst time\n");

	PROC_INFO* tmp;
	while(!is_empty(ready_queue)){
		tmp = pop(ready_queue);
		printf("%c\t%d\t%d\t", tmp->p_name, tmp->arrivalTime, tmp->burstTime);

		ctime += tmp->burstTime;

		for(int i=0; i<SIZE; i++){
			if(arr[i].arrivalTime <= ctime && arr[i].arrivalTime != -1){
				push(arr[i], ready_queue);
				arr[i].arrivalTime = -1;
			}	
		}
	}
}
int main(){
	PROC_INFO arr[SIZE];
	arr[0].p_name = 'A';
	arr[0].arrivalTime = 0;
	arr[0].burstTime = 3;
	arr[1].p_name = 'B';
	arr[1].arrivalTime = 2;
	arr[1].burstTime = 6;
	arr[2].p_name = 'C';
	arr[2].arrivalTime = 4;
	arr[2].burstTime = 4;
	arr[3].p_name = 'D';
	arr[3].arrivalTime = 6;
	arr[3].burstTime = 5;
	arr[4].p_name = 'E';
	arr[4].arrivalTime = 8;
	arr[4].burstTime = 2;

	SJF(arr);
	return 0;
}

