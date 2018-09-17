/*
 *  Tianyi Zhang
 * 	ECE 2230
 * 	Dec 5th 2017
 */
#include"sim.h"
#include"priority.h"
#include"queue.h"
#include"randsim.h"
#include<stdlib.h>
#include<stdio.h>

int main(){
	int a,b;
	int count,limit=50;
	customer_t *c=(customer_t*)calloc(limit,sizeof(*c));
	event_t *ev=(event_t*)calloc(limit,sizeof(*ev));

	for(count=0;count<limit;count+=2){
		ev[count].event_time=90-count;
		ev[count].queue_number=count;
	}

	for(count=1;count<limit;count+=2){
			ev[count].event_time=90-count;
			ev[count].queue_number=count;
		}

	for(count=0;count<limit;count++)
			c[count].arrival_time=count;


	queue_t* t=queue_init(limit-1);
	priority_t* pt=priority_init(limit-1);

	printf("initialized!\n");

	for(count=0;count<limit;count++){
	a=priority_insert(pt,ev+count);
	if(a<0)printf("fail to insert pt.\n");
	else printf("inserted %d!\n",count);
	if(priority_full(pt))break;

	}



	printf("all inserted!\n");

	for(count=0;count<limit;count++){
		b=queue_insert(t,c+count);
		if(b<0)printf("fail to insert t.\n");
		else printf("inserted %d\n",count);
		if(queue_full(t))break;

		}

	for(count=0;count<limit;count++){
	b=queue_insert(t,c+count);
	if(b<0)printf("fail to insert t.\n");
	else printf("inserted %d\n",count);
	if(queue_full(t))break;

	}

	for(count=0;count<limit;count++){
	event_t* temp=priority_remove(pt);
	if(temp!=NULL)printf("%d remove: %d\n",count,temp->queue_number);
	else printf("remove is empty\n");
	//free(temp);
	if(priority_empty(pt))break;
	}

	for(count=0;count<limit;count++){
		customer_t* temp=queue_remove(t);
		if(temp!=NULL)printf("%d remove t : %f\n",count,temp->arrival_time);
		else printf("Nothing to return t.\n");
		if(!queue_size(t))break;

	}

	if(queue_peek(t)==NULL)printf("empty peek\n");

	queue_finalize(t);
	priority_finalize(pt);

		free(c);
		free(ev);


	return 0;
}
