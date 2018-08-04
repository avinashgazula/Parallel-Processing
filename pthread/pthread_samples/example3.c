
// Example
#define _MULTI_THREADED
#include <stdio.h>
#include <qp0z1170.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include "check.h"

void abortTheProcessWhenAnExceptionOccurs(int sigNumber);
void *threadfunc1(void *parm);

void *threadfunc1(void *parm)
{
	char *p=NULL;

	printf("Thread1: Unhandled exception (pointer fault) about to happen\n");
	*p = `!';
	printf("Thread1: After exception\n");

	return NULL;
}

void abortTheProcessWhenAnExceptionOccurs(int sigNumber) {
	/* In a multithreaded environment this is a little difficult. We have to
	*/
	/* re-enable the ANSI C handler immediately, because that is the way it
	*/
	/* is defined. (A better alternative may be direct monitor exception
	*/
	/* handlers that are always valid in the function which they are
	*/
	/* registered, and with direct monitors, we can catch the hardware
	*/
	/* exception before it is converted to an ANSI C
	signal */
	signal(SIGALL, abortTheProcessWhenAnExceptionOccurs);

	/* Since ANSI C signals and hardware exceptions are only handled in
	*/
	/* the same thread that caused them, we send the Posix signal to
	*/
	/* the calling thread (The signal is delivered before returning from
	*/
	/* pthread_kill().
	*/
	printf("Mapping ANSI signal %d to posix signal SIGABRT. "
	"Aborting the process\n", sigNumber);

	/* If we want to do some debug processing, we can put it here.
	*/
	pthread_kill(pthread_self(), SIGABRT);

	return;
}

int main(int argc, char **argv)
{
	int rc=0;
	pthread_t threadid;
	void *status;

	printf("----------- Setup Signal Mapping/Handling -------------\n");
	printf("- Register ANSI C signal handler to map ALL\n"
	" ANSI C signals & hardware exceptions to Posix signals\n");
	/* If we want to do debug, or determine what when wrong a little more
	easily,
	*/
	/* we could use the abortTheProcessWhenAnExceptionOccurs function to
	delay
	the thread, or */
	/* dump failure data of some
	sort.
	*/
	signal(SIGALL, abortTheProcessWhenAnExceptionOccurs);

	printf("----------- Start memory fault thread -------------\n");
	printf("Create a thread\n");
	rc = pthread_create(&threadid, NULL, threadfunc1, NULL);
	checkResults("pthread_create()\n", rc);

	rc = pthread_join(threadid, &status);
	checkResults("pthread_join()\n", rc);

	printf("Main completed\n");
	return 0;
}






// Output:
// ----------- Setup Signal Mapping/Handling -------------
// - Register ANSI C signal handler to map ALL
// ANSI C signals & hardware exceptions to Posix signals
// ----------- Start memory fault thread -------------
// Create a thread
// Thread1: Unhandled exception (pointer fault) about to happen
// Mapping ANSI signal 5 to posix signal SIGABRT. Aborting the process
// Unhandled exceptions terminate the thread (not the process)




