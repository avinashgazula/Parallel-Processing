#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define READERS 5
#define WRITERS 2

int active=0;
int pendingReaders=0;
int pendingWriters=0;

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t canRead=PTHREAD_COND_INITIALIZER;
pthread_cond_t canWrite=PTHREAD_COND_INITIALIZER;

pthread_t reader[READERS];
pthread_t writer[WRITERS];

void *reading();
void readerEnter();
void readerLeaves();
void *writing();
void writerEnter();
void writerLeaves();

int main(){

	int i,returnValue;
//	int arr[10]={0,1,2,3,4,5,6,7,8,9};

	for(i=0; i < READERS; i++){
		returnValue = pthread_create(&reader[i],NULL,reading, (void *)&i);
	 	if(returnValue!=0){
			printf("Error in creating reader no: %d \n",i);
			return 0;
		}
	}

	for(i=0; i < WRITERS; i++){
		returnValue = pthread_create(&reader[i],NULL,writing, (void *)&i);
	 	if(returnValue!=0){
			printf("Error in creating writer no: %d \n",i);
			return 0;
		}
	}

	for (i=0; i < READERS; i++)
		pthread_join (reader[i], NULL);
	for (i=0; i < WRITERS; i++)
	 	pthread_join (writer[i], NULL);	

return 0;
}

void * reading(void *no){
	int n=*(int *)no;
	while(1){
		readerEnter();
		printf("reader %d is reading .. \n",n);
		sleep(1);
		readerLeaves();

		sleep(rand()%3);
	}
}

void readerEnter(){
	pthread_mutex_lock(&mutex);
	pendingReaders++;
	while(active < 0)
		pthread_cond_wait(&canRead, &mutex);
	if(pendingWriters > 0)
		pthread_cond_wait(&canRead, &mutex);
	active++;
	pendingReaders--;
	pthread_mutex_unlock(&mutex);
}

void readerLeaves(){
	pthread_mutex_lock(&mutex);
	active--;
	if(active == 0){
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&canWrite);	
	}
	pthread_mutex_unlock(&mutex);
}

void *writing(void *no){
	int n=*(int *)no;

	while(1){
	
		writerEnter();
	
		printf("....writer %d is writing ...\n",n);
		sleep(1);	
	
		writerLeaves();
		sleep(rand()%2);
	}
}

void writerEnter(){
	
	pthread_mutex_lock(&mutex);
	pendingWriters++;
	
	while(active)
		pthread_cond_wait(&canWrite, &mutex);
	
	pendingWriters--;
	active = -1;
	pthread_mutex_unlock(&mutex);
}

void writerLeaves(){
	
	pthread_mutex_lock(&mutex);
	active = 0;
	
	if(pendingReaders != 0){
		pthread_mutex_unlock(&mutex);
		pthread_cond_broadcast(&canRead);
	}
	else{
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&canWrite);
	}
}
