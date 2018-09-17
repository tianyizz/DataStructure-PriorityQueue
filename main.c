/*
 *  Tianyi Zhang
 * 	ECE 2230
 * 	Nov 15th 2017
 */

#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include"sim.h"
#include"priority.h"
#include"queue.h"
#include"randsim.h"

int main(int argc, char* argv[]){
	double max[4]={0,0,0,0};
	double min[4]={10000,10000,10000,10000};
	int MAX_CUST=1000;
	int cashLane=0;
	int maxEv=1000;
	int numCash=1;
	int totalCust=0;
	int counter=0;
	bool correct=false;
	double totalShop=0,totalEnq=0,totalCheck=0;

/*
 * Parsing User input, read any flags necessary and giving
 * feedback respectively
 *
 */
	if(argc>1){
		int a=0;
		for(;a<argc;a++){
			if(argv[a+1]==NULL)break;

			if(strcmp(argv[a],"-q")==0){

				int keyEntered=0,i,size;
				   for(size=0;size<90;size++)
					   if(argv[a+1][size]=='\0')
						   break;

				     size--;
				     for(i=0;i<90;i++){
				    	 int tempnum=1,tempcounter=(size-i);
				    	 if(argv[a+1][i]!='\0'){
				    		 while(tempcounter--)tempnum*=10;
				    		 keyEntered+=(tempnum*(argv[a+1][i]-'0'));
				    	 }
				    	 else break;
				     }
				 if(keyEntered<0)
					 printf("Input Q number must be greater than 0\n");
				 else{
					 correct=true;
					 numCash=keyEntered;
					 printf("Q number override, number of Cashiers: %d\n",numCash);
				 }
			}

			if(strcmp(argv[a],"-e")==0){

				int keyEntered=0,i,size;
				char* temp=argv[a+1];
				for(size=0;size<90;size++) if(temp[size]=='\0')break;
					size--;
					for(i=0;i<90;i++){
						int tempnum=1,tempcounter=(size-i);
						if(temp[i]!='\0'){
							while(tempcounter--)tempnum*=10;
							keyEntered+=(tempnum*(temp[i]-'0'));
						}
						else break;
					}
				if(keyEntered<0)
				  printf("Input PQ must be greater than 0\n");
				else{
					correct=true;
					maxEv=keyEntered;
					printf("PQ size override, size PQ: %d\n",maxEv);

				}
			}

			if(strcmp(argv[a],"-c")==0){

			//	printf("argv[]: %s",argv[a]);


				int keyEntered=0,i,size;
				char* temp=argv[a+1];
				for(size=0;size<90;size++) if(temp[size]=='\0')break;
				size--;
				for(i=0;i<90;i++){
					int tempnum=1,tempcounter=(size-i);
					if(temp[i]!='\0'){
						while(tempcounter--)tempnum*=10;
						keyEntered+=(tempnum*(temp[i]-'0'));
					}
					else break;
				}
				if(keyEntered<0)
				   printf("Input CUST_MAX must be greater than 0\n");
				else{
					correct=true;
					MAX_CUST=keyEntered;
					printf("MAX Customer size override, size: %d\n",MAX_CUST);
				}
			}
		}
	}

	if(!correct){
		printf("Incorrect input and the correct input form should\n");
		printf("be \"-q number\" \"-e number\"  \"-c number\"\n");
		printf("Program is running on default value: \n");
		printf("Max customer: %d, Number of Qs: %d, Max PQ length: %d\n"
				,MAX_CUST, numCash,maxEv);
	}

	printf("\n");


	cashLane=MAX_CUST+1;

/*initilize an array of pointers to the queue_t struct (this is number of cashiers)*/
	queue_t** Cq=(queue_t**)calloc(numCash,sizeof(*Cq));

/* initialize queues */
	while(counter<numCash){
		Cq[counter]=queue_init(cashLane);
		if(Cq==NULL)printf("Cannot allocate memory.\n");
		counter++;
	}

/*keep record of waiting time of each lane*/
	double* cqTime=(double*)calloc(numCash,sizeof(*cqTime));


/* malloc new event and customer */
	priority_t* eq=priority_init(maxEv);
	customer_t* cust=(customer_t*)calloc(1,sizeof(*cust));
	totalCust++;

/* now schedule arrive event at t=0 and put in event queue */
	event_t* startEv=(event_t*)calloc(1,sizeof(*startEv));
	startEv->event_type=EV_ARRIVE;
	startEv->event_time=0;
	startEv->customer=cust;
	startEv->queue_number=-1;

	priority_insert(eq,startEv);

/*free things up if user inputs 0 for Qs and PQs*/
	if(priority_full(eq)||numCash==0){
		free(startEv);
		free(cust);
	}


/*Operation starts here*/
while(!priority_empty(eq)&&numCash!=0)
{
	event_t* new_ev = priority_remove(eq);

	switch (new_ev->event_type)
	{
		case (EV_ARRIVE) :{

		/*In the event case, calculate the shop time,
		 * update the event time to shop
		 * shedule an enq for the customer */
		double shoptime = TIME_SHOP();
	    totalShop+=shoptime;
		event_t* enqEv_an=(event_t*)calloc(1,sizeof(*enqEv_an));
		enqEv_an->event_type=EV_ENQUEUE;
		enqEv_an->event_time=new_ev->event_time+shoptime;
		enqEv_an->customer=new_ev->customer;
		enqEv_an->queue_number=-1;
		priority_insert(eq,enqEv_an);
		if(shoptime>max[0])max[0]=shoptime;
		if(shoptime<min[0])min[0]=shoptime;

		/*
		 * New customer initialization if
		 * not reaching the maximum
		 */
		if (MAX_CUST > totalCust)
		{
			/* malloc new event and customer */
			if(priority_full(eq)){
				printf("The specified priority queue length is not enough for operation.\n");
				printf("Program terminated. (Current max size of PQ: %d)\n",maxEv);

				int a=0;
				free(new_ev);
				free(cqTime);

				while(!priority_empty(eq)){
					event_t* t=priority_remove(eq);
					if(t->customer!=NULL)free(t->customer);
					free(t);
				}

				for(;a<numCash;a++){
					while(queue_size(Cq[a]))
						free(queue_remove(Cq[a]));
					queue_finalize(Cq[a]);
				}

				free(Cq);

				priority_finalize(eq);

				return 0;
			}

			/*Schedule the start event for the customer*/
			customer_t* an_cust=(customer_t*)calloc(1,sizeof(*an_cust));
			totalCust++;
			event_t* startEv_an=(event_t*)calloc(1,sizeof(*startEv_an));
			double ariTime=TIME_ARRIVE();
			startEv_an->event_type=EV_ARRIVE;
			startEv_an->event_time=ariTime;
			startEv_an->customer=an_cust;
			startEv_an->queue_number=-1;
			an_cust->arrival_time = ariTime;
			priority_insert(eq,startEv_an);
		}
		free(new_ev);

		break;
		}

		case (EV_ENQUEUE) :{
	    new_ev->customer->enqueue_time=new_ev->event_time;
		int tempMin=cashLane+1;
		int minQ=0;

		/*Look for the shortest lane*/
		for(counter=0;counter<numCash;counter++){

			if(tempMin>queue_size(Cq[counter])){
				minQ=counter;
				tempMin=queue_size(Cq[counter]);
			}
		}

		/*shedule a checkout event for the customer
		 * update the event time with the current
		 * waiting time of that respective lane
		 */
		event_t* ckEv=(event_t*)calloc(1,sizeof(*ckEv));
		ckEv->event_type=EV_CHECKOUT;
		ckEv->event_time=new_ev->event_time+cqTime[minQ];
		ckEv->customer=NULL;
		ckEv->queue_number=minQ;

		/*
		 * check for size of the Q, if filled (should not)
		 * return with error message and free up the memory
		 */
		if(cashLane>queue_size(Cq[minQ])){
		  queue_insert(Cq[minQ],new_ev->customer);
		}

		else{
			printf("The specified queue length is not enough for operation.\n");
			printf("Program terminated. (Current max size of Q: %d)\n",cashLane);

			free(new_ev->customer);
			free(ckEv);
			free(new_ev);
			free(cqTime);

			while(!priority_empty(eq)){
				event_t* t=priority_remove(eq);
				if(t->customer!=NULL)free(t->customer);
				free(t);
			}

			int a=0;
			for(;a<numCash;a++){
				while(queue_size(Cq[a]))
				free(queue_remove(Cq[a]));
				queue_finalize(Cq[a]);
			}

			free(Cq);

			priority_finalize(eq);

			return 0;
		}

		/*
		 * if the Q is empty, no one waiting
		 * then keep the event time as same
		 * of the enq time (no waiting)
		 * insert the new event
		 */
		if(tempMin==0)
			ckEv->event_time=new_ev->event_time;

		double enqtime=ckEv->event_time-new_ev->event_time;
		if(enqtime>max[1])max[1]=enqtime;
		if(enqtime<min[1])min[1]=enqtime;

		priority_insert(eq,ckEv);

		free(new_ev);
		break;
		}

		case (EV_CHECKOUT) :{
		double checkTime=TIME_CHECKOUT();
		customer_t* temp=queue_peek(Cq[new_ev->queue_number]);

		/*
		 * Peek the top customer and update the total
		 * waiting time of the wait lane set the checkout
		 * time in the event and schedule a leave event
		 */
		if(temp!=NULL){
			temp->checkout_time=new_ev->event_time;
			cqTime[new_ev->queue_number]+=checkTime;
			totalCheck+=checkTime;

			event_t* lvEv=(event_t*)calloc(1,sizeof(*lvEv));
			lvEv->event_type=EV_LEAVE;
			lvEv->event_time=temp->checkout_time+checkTime;
			lvEv->customer=NULL;
			lvEv->queue_number=new_ev->queue_number;
			if(checkTime>max[2])max[2]=checkTime;
			if(checkTime<min[2])min[2]=checkTime;
			priority_insert(eq,lvEv);
		}


		free(new_ev);

		break;
		}
		case (EV_LEAVE):{
		customer_t* tempLv=queue_remove(Cq[new_ev->queue_number]);

		/*
		 * free the customer as leaving
		 */

		if(tempLv!=NULL){
			tempLv->leave_time=new_ev->event_time;
			cqTime[new_ev->queue_number]-=(tempLv->leave_time-tempLv->checkout_time);
			totalEnq+=(tempLv->checkout_time-tempLv->enqueue_time);
			double tempTotal=tempLv->leave_time-tempLv->arrival_time;
			if(tempTotal>max[3])max[3]=tempTotal;
			if(tempTotal<min[3])min[3]=tempTotal;

			free(tempLv);
		}

		/*
		 * schedule a new checkout event if Q is empty
		 */

		if(queue_size(Cq[new_ev->queue_number])!=0){
			event_t* ckEv=(event_t*)calloc(1,sizeof(*ckEv));
			ckEv->event_type=EV_CHECKOUT;
			ckEv->event_time=new_ev->event_time;
			ckEv->customer=NULL;
			ckEv->queue_number=new_ev->queue_number;
			priority_insert(eq,ckEv);
		}

		free(new_ev);
		break;
		}
		default :
			break;
		}

	}

	printf("Total number of customer: %d\n",totalCust);
	printf("\tshop time\twait time\tcheckout time\toverall time\n");
	printf("avg:\t%f\t%f\t%f\t%f\t\n",totalShop/totalCust,totalEnq/totalCust,totalCheck/totalCust,(totalShop+totalEnq+totalCheck)/totalCust);
	printf("max:\t%f\t%f\t%f\t%f\t\n",max[0],max[1],max[2],max[3]);
	printf("min:\t%f\t%f\t%f\t%f\t\n",min[0],min[1],min[2],min[3]);

	/*free up the memories*/
	int i=0;
	free(cqTime);

	for(;i<numCash;i++)
		queue_finalize(Cq[i]);

	free(Cq);

	priority_finalize(eq);

	return 0;
	}


