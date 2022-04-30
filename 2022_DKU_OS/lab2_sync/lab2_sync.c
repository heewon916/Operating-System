/*
*   DKU Operating System Lab
*           Lab2 (Hash Queue Lock Problem)
*           Student id : 32201321
*           Student name : Heewon Kim
*
*   lab2_sync.c :
*       - lab2 main file.
*		- Thread-safe Hash Queue Lock code
*		- Coarse-grained, fine-grained lock code
*       - Must contains Hash Queue Problem function's declations.
*
*   Implement thread-safe Hash Queue for coarse-grained verison and fine-grained version.
*/

#include <aio.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "lab2_sync_types.h"

/*
 * TODO
 *  Implement function which init queue nodes for front and rear
 *  ( refer to the ./include/lab2_sync_types.h for front and rear nodes)
 */
void init_queue() {
	// You need to implement init_queue function
	pthread_mutex_init(&frontLock, NULL);
	pthread_mutex_init(&rearLock, NULL);
	front = NULL;
	rear = NULL;
}

/*
 * TODO
 *  Implement function which add new_node at next rear node
 *
 *  @param queue_node *new_node		: Node which you need to insert at queue.
 */
int first = 1;
void enqueue(queue_node *new_node) {
	// You need to implement enqueue function.		
	if(value_exist(new_node->data)){
		printf("[!] FAILED :: data ALREADY EXIST\n");
		return;
	}
	if(first){ 	// the first node inserted
		//front = new_node->prev;
		//rear = new_node->next;
		printf("[E] front = new_node\n");
		front = new_node;
		printf("[E] rear = new_node\n");
		rear = new_node;
		first = 0;
		
	} else {		// if queue was not empty
		printf("[E] rear->next = new_node\n");
		rear->next = new_node;
		printf("[E] new_node->prev = rear\n");
		new_node->prev = rear;
		printf("[E] rear = new_node\n");
		rear = new_node;
	
	}
}

/*
 * TODO
 *  Implement function which add new_node at next rear node
 *
 *  @param queue_node *new_node		: Node which you need to insert at queue in coarse-grained manner.
 */
void enqueue_cg(queue_node *new_node) {
	// You need to implement enqueue_cg function.
	pthread_mutex_lock(&frontLock);
	pthread_mutex_lock(&rearLock);
	first = 1;
	if (value_exist(new_node->data)){
		printf("[!] ERROR :: data ALREADY EXIST\n");
		return;
	}
	if(first){
		front = new_node;
		rear = new_node;
		first = 0;
	} else{
		rear->next = new_node;
		new_node->prev = rear;
		rear = new_node;
	}
	pthread_mutex_unlock(&rearLock);
	pthread_mutex_unlock(&frontLock);
}

/*
 * TODO
 *  Implement function which add new_node at next rear node
 *
 *  @param queue_node *new_node		: Node which you need to insert at queue in fine-grained manner.
 */
void enqueue_fg(queue_node *new_node) {
	// You need to implement enqueue_fg function.
	first = 1;
	if(value_exist(new_node->data)){
		printf("[!] ERROR :: data ALREADY EXIST\n");
		return;
	}
       	pthread_mutex_lock(&frontLock);
	pthread_mutex_lock(&rearLock);	
	if(first){
		front = new_node;
		rear = new_node;
		first = 0;
	} else {
		rear->next = new_node;
		new_node->prev = rear;
		rear = new_node;
	}
	pthread_mutex_unlock(&rearLock);
	pthread_mutex_unlock(&frontLock);
}

/*
 * TODO
 *  Implement function which delete del_node at location that contains target key
 *
 *  @param queue_node *del_node		: Node which you need to delete at queue.
 */
void dequeue(queue_node *del_node) {
	// You need to implement dequeue function.
	//printf("[D] del_node = front\n");
	del_node = front;
	//printf("[D] front = front->rear\n");
	front = front->next;
	//printf("[D] front->prev = NULL\n");
	front->prev = NULL;
}
/*
 * TODO
 *  Implement function which delete del_node at location that contains target key
 *
 *  @param queue_node *del_node		: Node which you need to delete at queue in coarse-grained manner.
 */
void dequeue_cg(queue_node *del_node) {
	// You need to implement dequeue_cg function.
	pthread_mutex_lock(&frontLock);
	if(front == NULL){
		printf("[!] WARNING :: queue is EMPTY\n");
	       	return;	
	}
	del_node = front;
	front = front->next;
	front->prev = NULL;
	pthread_mutex_unlock(&frontLock);
}

/*
 * TODO
 *  Implement function which delete del_node at location that contains target key
 *
 *  @param queue_node *del_node		: Node which you need to delete at queue in fine-grained manner.
 */
void dequeue_fg(queue_node *del_node) {
	// You need to implement dequeue_fg function.
	if(front == NULL){
		printf("[!] WARNING :: queue is EMPTY\n");
		return ;
	}
	pthread_mutex_lock(&frontLock);
	del_node = front;
	front = front->next;
	front->prev = NULL;
	pthread_mutex_unlock(&frontLock);
}

/*
 * TODO
 *  Implement function which init hashlist(same as hashtable) node.
 */
void init_hlist_node() {
	// You need to implement init_hlist_node function.
	pthread_mutex_init(&hlistLock, NULL);
	int i;
	for(i=0; i<HASH_SIZE; i++){
		hashlist[i]->next->next = NULL; 	// point the next node inserted
		hashlist[i]->next->q_loc = NULL;
		hashlist[i]->q_loc->next = NULL; 	// point the location of hashlist's   
		hashlist[i]->q_loc->prev = NULL;
		hashlist[i]->q_loc->data = 0;
		printf("[.] init_hlist_node() :: %d completed..\n", i);
	}
}
/*
 * TODO
 *  Implement function which calculate hash value with modulo operation.
 */
int hash(int val) {
	// You need to implement hash function.
	return val % HASH_SIZE;
}
/*
 * TODO
 *  Implement function which insert the result of finding the location 
 *  of the bucket using value to the entry and hashtable
 *
 *  @param hlist_node *hashtable		: A pointer variable containing the bucket
 *  @param int val				: Data to be stored in the queue node
 */
void hash_queue_add(hlist_node *hashtable, int val) {
	// You need to implement hash_queue_add function.
	printf("[HQ] START\n");
	// make new queue_node
	queue_node* new_Q_node;
	new_Q_node = (queue_node*)malloc(sizeof(queue_node));
	new_Q_node->data = val;
	new_Q_node->prev = new_Q_node->next = NULL;
	// make new hlist_node
	hlist_node* new_H_node = (hlist_node*)malloc(sizeof(hlist_node));
	new_H_node->next = NULL;

	printf("[HQ] enqueue(new_node)\n");
	enqueue(new_Q_node);
	
	int key = hash(val);
	printf("[HQ] set hashtable[key].q_loc\n");
	if(hashtable[key].next == NULL){ 	// first inserted key 
		hashtable[key].next = new_H_node;
		hashtable[key].q_loc = rear;
		new_H_node->q_loc = rear;
	} else{
		hlist_node* tmp;
		tmp = &hashtable[key];
		while(tmp){
			tmp = tmp->next;
		} // get out when the node is the last one
		tmp->next = new_H_node;
		new_H_node->q_loc = rear;
	}
	free(new_Q_node);
	free(new_H_node);
}
/*
 * TODO
 *  Implement function which insert the resilt of finding the location 
 *  of the bucket using value to the entry and hashtable
 *
 *  @param hlist_node *hashtable		: A pointer variable containing the bucket
 *  @param int val				: Data to be stored in the queue node
 */
void hash_queue_add_cg(hlist_node *hashtable, int val) {
	// You need to implement hash_queue_add_cg function.
	pthread_mutex_lock(&hlistLock);
	queue_node* new_node;
	new_node = (queue_node*)malloc(sizeof(queue_node));
	if(new_node == NULL){
		printf("[!] ERROR :: malloc FAILED\n");
		return;
	}
	new_node->data = val;
	new_node->prev = new_node->next = NULL;
	enqueue(new_node);
	int key = hash(val);
	printf("[HQ_CQ] set hashtable[key].q_loc\n"); 
	if(hashtable[key].q_loc == NULL){
		hashtable[key].q_loc = rear;
	} else {
		queue_node* tmp;
		tmp = hashtable[key].q_loc;
		while(tmp->next != NULL){
			tmp = tmp->next;
		}
		tmp->next = rear;
	}
	free(new_node);
	pthread_mutex_unlock(&hlistLock);
}

/*
 * TODO
 *  Implement function which insert the resilt of finding the location 
 *  of the bucket using value to the entry and hashtable
 *
 *  @param hlist_node *hashtable		: A pointer variable containing the bucket
 *  @param int val				: Data to be stored in the queue node
 */
void hash_queue_add_fg(hlist_node *hashtable, int val) {
	// You need to implement hash_queue_add_fg function.
	queue_node* new_node = (queue_node*)malloc(sizeof(queue_node));
	if(new_node == NULL){
		printf("[!] ERROR :: malloc FAILED\n");
		return ;
	}
	new_node->data = val;
	new_node->prev = new_node->next = NULL;
	enqueue(new_node);
	int key = hash(val);
	printf("[HQ_FQ] set hashtable[key].q_loc\n");
	pthread_mutex_lock(&hlistLock);
	if(hashtable[key].q_loc == NULL) {
		hashtable[key].q_loc = rear;
	} else {
		queue_node* tmp;
		tmp = front;
		while(tmp->next == rear){
			tmp = tmp->next;
		}
		tmp->next =rear;
	}
	free(new_node);
	pthread_mutex_unlock(&hlistLock);
}		

/*
 * TODO
 *  Implement function which check if the data(value) to be stored is in the hashtable
 *
 *  @param int val				: variable needed to check if data exists
 *  @return					: status (success or fail)
 */
int value_exist(int val) {
	// You need to implement value_exist function.
	int key = hash(val);
	queue_node* tmp = front;
	while(front != rear){
		if(tmp->data == key){
			return 1;
		}
	}
	return 0;
}

/*
 * TODO
 *  Implement function which find the bucket location using target
 */
void hash_queue_insert_by_target() {
	// You need to implement hash_queue_insert_by_target function.
	hash_queue_add(*hashlist, target);	
}

/*
 * TODO
 *  Implement function which find the bucket location using target
 */
void hash_queue_insert_by_target_cg() {
	// You need to implement hash_queue_insert_by_target_cg function.
	hash_queue_add_cg(*hashlist, target);
}

/*
 * TODO
 *  Implement function which find the bucket location using target
 */
void hash_queue_insert_by_target_fg() {
	// You need to implement hash_queue_insert_by_target_fg function.
	hash_queue_add_fg(*hashlist, target);
}

/*
 * TODO
 *  Implement function which find the bucket location and stored data
 *  using target and delete node that contains target
 */
void hash_queue_delete_by_target() {
	// You need to implement hash_queue_delete_by_target function.
	queue_node* tmp = front;
	while(tmp == rear){
		if(target == tmp->data){
			dequeue(tmp);
			return;
		}
	}
	printf("[!] ERROR :: target NOT EXIST\n");
}

/*
 * TODO
 *  Implement function which find the bucket location and stored data
 *  using target and delete node that contains target
 */
void hash_queue_delete_by_target_cg() {
	// You need to implement hash_queue_delete_by_target_cg function.
	
	queue_node* tmp = front;
	while(tmp == rear){
		if(target == tmp->data){
			dequeue(tmp);
			return;
		}
	}
	printf("[!] ERROR :: target NOT EXIST\n");
}

/*
 * TODO
 *  Implement function which find the bucket location and stored data
 *  using target and delete node that contains target
 */
void hash_queue_delete_by_target_fg() {
	// You need to implement hash_queue_delete_by_target_fg function.
	queue_node* tmp = front;
	while(tmp == rear){
		if(target == tmp->data){
			dequeue(tmp);
			return ;
		}
	}
	printf("[!] ERROR :: target NOT EXIST\n");
}
