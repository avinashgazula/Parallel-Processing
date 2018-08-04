#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define MAXCHAIRS	4

void * barber(void *);
int checkAndSleep(void);
void serviceCustomer();
void * customer(void *);
void wakeUpBarber();
void customerWait(int chair);
int checkVacancyAndWait(void);

int	nextCustomerNo = 1;
int	first = 0, last = 0;
int	waitingCustomerCount = 0;

pthread_mutex_t lck=PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t custCond[MAXCHAIRS]={
	PTHREAD_COND_INITIALIZER,
	PTHREAD_COND_INITIALIZER,
	PTHREAD_COND_INITIALIZER,
	PTHREAD_COND_INITIALIZER
	};
pthread_cond_t brbrCond=PTHREAD_COND_INITIALIZER;

int custNos[MAXCHAIRS];	

int main(){

	int i, temp;
	pthread_t brbr, cust[MAXCHAIRS+1];

	// Create barber thread
	temp=pthread_create(&brbr, NULL, barber, NULL);
	if(temp<0){
		printf("unable to create barber thread\n");
		return(0);	
	}
	// Create  MAXCHAIRS + 1 customer threads.
	for (i=0;  i<MAXCHAIRS+1; i++){
		temp=pthread_create(&cust[i], NULL, customer, NULL);
		if(temp<0){
			printf("unable to create customer[%d] thread\n",i);
			return(0);	
		}	
	}
	// Do nothing here
	for ( ; ; );
	
	return 0;
}

void * barber(void *no){
	while (1){
		// Check for any customer waiting 
		// no custmer is there then sleep
		checkAndSleep();

		// Customer is available or new customer has wakeup
		serviceCustomer();
	}
}

int checkAndSleep(void){
	// Check if any customer is waiting in the chair
	pthread_mutex_lock(&lck);
	if (waitingCustomerCount == 0)
		pthread_cond_wait(&brbrCond, &lck);

	// Mutex is relased after gaining access to the next waiting customer.
}

void serviceCustomer(){
	int custNo;
	int chair;

	// The mutex is already in locked state.
	chair = first;
	first = (first + 1) % MAXCHAIRS;
	custNo = custNos[chair];
	waitingCustomerCount--;

	// Inform the custmenr being serviced
	pthread_cond_signal(&custCond[chair]);
	pthread_mutex_unlock(&lck);

	// Service takes time
	printf("Barber servicing %d customer\n",custNo);
	sleep(2);
}

void wakeUpBarber(){
	pthread_cond_signal(&brbrCond);
}

void * customer(void *no){

	int custno;
	while (1){
		// check for vacant seat and wait.
		custno = checkVacancyAndWait();

		if (custno != 0){
			// Getting serviced
			printf("Customer %d getting serviced ...\n",custno);
			sleep(1);
		}
		else{
			printf("No chair is vacant and customer leaving ...\n");
			sleep((time(NULL) % 5) + 1);
		}
	}
}

void customerWait(int chair){
	pthread_cond_wait(&custCond[chair], &lck);	
}

// Check for empty chair. wait if available, else leave
int checkVacancyAndWait(void){
	int chair;
	int custNo;
	
	pthread_mutex_lock(&lck);
	if (waitingCustomerCount == MAXCHAIRS){
		pthread_mutex_unlock(&lck);	//no chair available
		return 0;
	}
	else{
		chair = last;
		custNo = custNos[chair] = nextCustomerNo++;
		last = (chair + 1) % MAXCHAIRS;
		waitingCustomerCount++;

		// If this is the first customer then wakeup the barber
		if (waitingCustomerCount == 1)
			wakeUpBarber();
	
		// Wait for service	
		customerWait(chair);
		pthread_mutex_unlock(&lck);
		return custNo;
	}
}

