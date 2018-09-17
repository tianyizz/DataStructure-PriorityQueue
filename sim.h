#ifndef SIM_H
#define SIM_H

#define EV_ARRIVE	1							/*	this	is	when	shopper	arrives	and	starts	shopping	*/

#define EV_ENQUEUE	2			/*	this	is	when	she	stops	shopping	and	gets	in	line	*/

#define EV_CHECKOUT	3	/*	this	is	when	she	gets	to	the	cashier	*/

#define EV_LEAVE	4										/*	this	is	when	she	leaves	the	store	*/


typedef struct customer_s
{
double arrival_time;
double enqueue_time;
double checkout_time;
double leave_time;
} customer_t;


typedef struct event_s
{
int event_type;
/* type of event – see below */
double event_time;
/* sim time when event occurs */
customer_t *customer;/* customer related to this event */
int queue_number;
/* queue related to this event */
} event_t;

#endif
