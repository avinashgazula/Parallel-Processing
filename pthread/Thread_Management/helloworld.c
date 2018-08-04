#include <pthread.h>
#include <stdio.h>

#define MAX_THREADS	50

void * PrintHello(void * data)
{
    printf("Hello from thread %d - I was created in iteration %d !\n", pthread_self(), data);
    pthread_exit(NULL);
}

int main(int argc, char * argv[])
{
    int rc;
    pthread_t thread_id[MAX_THREADS];
    int i, n;

    n = atoi(argv[1]);
    if(n > MAX_THREADS) n = MAX_THREADS;

    for(i = 0; i < n; i++)
    {
        rc = pthread_create(&thread_id[i], NULL, PrintHello, (void*)i);
        if(rc)
        {
             printf("\n ERROR: return code from pthread_create is %d \n", rc);
             exit(1);
        }
        printf("\n I am thread %d. Created new thread (%d) in iteration %d ...\n", 
                pthread_self(), thread_id[i], i);
        if(i % 5 == 0) sleep(1);
    }

    pthread_exit(NULL);
}
