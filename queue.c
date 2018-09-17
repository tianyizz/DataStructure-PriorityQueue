/*
 *  Tianyi Zhang
 * 	ECE 2230
 * 	Nov 15th 2017
 */
#include "sim.h"
#include "queue.h"
#include "list.h"
#include <stdlib.h>


struct queue_s{
	int size;
	int currentSize;
	list newLI;
};


/* create and initialize a new queue
must be able to hold at least size items
return pointer to the new queue, NULL if error */
queue_t *queue_init(int size){
	if(size<0)return NULL;
	queue_t* newQ=(queue_t*)calloc(1,sizeof(*newQ));
	if(newQ==NULL) return NULL;
	newQ->size=size;
	newQ->currentSize=0;
	newQ->newLI=list_init();
	if(newQ->newLI==NULL) return NULL;

	return newQ;
}

/* insert an item into the queue
return 0 if successful, -1 otherwise */
int queue_insert(queue_t *q, customer_t *c){
	if(q->size==0)return -1;
	list_first(q->newLI);
	customer_t* temp=list_append(q->newLI,c);
	if(temp==NULL)return -1;
	q->size--;
	q->currentSize++;
	return 0;
}
/* return the next item in the queue but do not remove it
return NULL if the queue is empty or on error */
customer_t *queue_peek(queue_t *q){
	if(q==NULL||q->newLI==NULL)return NULL;
	return list_first(q->newLI);
}
/* remove the next item from the queue
and return it, return NULL if there is an error */
customer_t *queue_remove(queue_t *q){
	if(q->currentSize==0)return NULL;
	list_first(q->newLI);
	q->currentSize--;
	q->size++;
	return list_remove(q->newLI);
}
/* return the number of items in the queue
You can see if queue is empty with this */
int queue_size(queue_t *q){
	if(q==NULL) return 0;
	return q->currentSize;
}
/* return nono-zero if the queue is full
This may be trivial using a linked implementation */
int queue_full(queue_t *q){
	if(q->size==0) return 1;
	return 0;
}
/* free all resourced used by the queue then free
the queue itself */
void queue_finalize(queue_t *q){
	if(q!=NULL&&q->newLI!=NULL)
	list_finalize(q->newLI);

	if(q!=NULL)
	free(q);
}
