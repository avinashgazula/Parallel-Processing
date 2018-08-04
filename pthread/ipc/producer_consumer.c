#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

# define BufferSize 10

void *Producer();
void *Consumer();

int BufferIndex=0;
char *BUFFER;

pthread_cond_t Buffer_Not_Full=PTHREAD_COND_INITIALIZER;
pthread_cond_t Buffer_Not_Empty=PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex_var=PTHREAD_MUTEX_INITIALIZER;

int main()
{    
    pthread_t ptid,ctid;
    
    BUFFER=(char *) malloc(sizeof(char) * BufferSize);            
    
    pthread_create(&ptid,NULL,Producer,NULL);
    pthread_create(&ctid,NULL,Consumer,NULL);
    
    pthread_join(ptid,NULL);
    pthread_join(ctid,NULL);
        
    
    return 0;
}

void *Producer()
{    
    for(;;)
    {
        pthread_mutex_lock(&mutex_var);
        if(BufferIndex == BufferSize)
        {                        
            pthread_cond_wait(&Buffer_Not_Full,&mutex_var);
        }                 

        BUFFER[BufferIndex++]='@';
        printf("Produce : %d \n",BufferIndex);
        
        pthread_mutex_unlock(&mutex_var);
        pthread_cond_signal(&Buffer_Not_Empty);
        
        sleep(rand()%3);        
    }    
}

void *Consumer()
{
    for(;;)
    {
        pthread_mutex_lock(&mutex_var);
        
        if(BufferIndex == 0)
        {            
            pthread_cond_wait(&Buffer_Not_Empty,&mutex_var);
        }                
        
        printf("\t\tConsume : %d \n",BufferIndex--);        
        
        pthread_mutex_unlock(&mutex_var);        
        pthread_cond_signal(&Buffer_Not_Full);                
        
        sleep(rand()%5);
    }    
}

