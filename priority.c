/*
 *  Tianyi Zhang
 * 	ECE 2230
 * 	Nov 15th 2017
 */

#include "sim.h"
#include "priority.h"
#include <stdlib.h>
#include<stdio.h>


struct priority_s{
	int count;
	int size;
	event_t** events;
};
/* create and initialize a new priority queue
must be able to hold at least size items
return pointer to the new priority queue, NULL if error */
priority_t *priority_init(int size){
	if(size<0)return NULL;
	priority_t* t = (priority_t*)calloc(1,sizeof(*t));
	if(t==NULL)return NULL;
	t->size=size;
	t->count=0;
	if(!(size>0))t->events=NULL;
	t->events=(event_t**)calloc(size,sizeof(event_t*));
	return t;
}

/* insert an item into the priority queue
return 0 if successful, -1 otherwise */
int priority_insert(priority_t *q, event_t *ev){

	//printf("IN insert\n");
	if(q==NULL||q->count==q->size)return -1;
    int child = q->count;
    int parent = (child-1)/2;  // round down

	//printf("IN insert\n");

    while(child>0 && (ev->event_time)<(q->events[parent]->event_time) ) {
       q->events[child] = q->events[parent];
       child = parent;
       parent = (child-1)/2;
    }

	//printf("IN insert %d\n",child);

    q->events[child] = ev;

    //printf("no\n");
    q->count++;

	//printf("IN insert\n");

    return 0;
}

/* remove the highest priority item from the queue
and return it, return NULL if there is an error */
event_t *priority_remove(priority_t *q){
	if(q==NULL||q->count==0)return NULL;
	q->count--;
	event_t* reValue=q->events[0];

	int temp = q->count;
	double newVal = q->events[temp]->event_time;
	//printf("in pt que print %d\n",reValue->queue_number);

	event_t* newTemp=q->events[temp];
	int cur = 0;
	int left = 1;
	int right = left+1;
	int min;
	while((left< q->count && newVal>q->events[left]->event_time)||
		(right< q->count && newVal>q->events[right]->event_time)){
	   min = left;
	   if(right<(q->count) && (q->events[right]->event_time<q->events[left]->event_time))
		   min = right;
	   q->events[cur] = q->events[min];
	   cur = min;
	   left = 2*min+1;
	   right = left+1;
	}

	q->events[cur]=newTemp;

	//printf("in pt que print %d\n",reValue->queue_number);


	return reValue;
}

/* return non-zero if the priority queue us empty */
int priority_empty(priority_t *q){
	if(q==NULL||q->count==0) return 1;
	return 0;
}
/* return nono-zero if the priority queue is full
This may be trivial using a linked implementation */
int priority_full(priority_t *q){
	if(q==NULL||q->count!=q->size) return 0;
	return 1;
}
/* free all resourced used by the priority queue then free
the queue itself */
void priority_finalize(priority_t *q){
	if(q!=NULL){
		if(q->events!=NULL){
			free(q->events);
			free(q);
		}
	}
}
