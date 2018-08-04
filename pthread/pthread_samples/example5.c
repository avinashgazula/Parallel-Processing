
// Example
#define _MULTI_THREADED
#include <stdio.h>
#include <qp0z1170.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include "check.h"

void myAnsiSignalMapperHdlr(int sigNumber);
void *threadfunc1(void *parm);
void *threadfunc2(void *parm);

void *threadfunc1(void *parm)
{
	char *p=NULL;

	printf("Thread1: Unhandled exception (pointer fault) about to happen\n");
	*p = `!';
	printf("Thread1: After exception\n");

	return NULL;
}

void *threadfunc2(void *parm)
{
	int i1=0, i2=1, i3=0;

	printf("Thread2: Unhandled exception (divide by zero) about to happen\n");
	i1 = i2 / i3;
	printf("Thread2: After exception\n");

	return NULL;
}

void myAnsiSignalMapperHdlr(int sigNumber) {
	/* In a multithreaded environment, this is slightly difficult. We have
	to*/
	/* re-enable the ANSI C handler immediately, because that is the way it
	*/
	/* is defined. (A better alternative may be direct monitor exception
	*/
	/* handlers which are always valid in the function which they are
	*/
	/* registered, and with direct monitors, we can catch the hardware
	*/
	/* exception before it is converted to an ANSI C signal
	*/
	signal(SIGALL, myAnsiSignalMapperHdlr);

	/* Since ANSI C signals and hardware exceptions will only be handled in
	*/
	/* the same thread that caused them, we will send the POSIX signal to
	*/
	/* the calling thread (The signal will be delivered before returning from
	*/
	/* pthread_kill().
	*/
	printf("Mapping ANSI signal to POSIX signal %d\n", sigNumber);
	pthread_kill(pthread_self(), sigNumber);

	return;
}
void fpViolationHldr(int sigNumber) {
	printf("Thread 0x%.8x %.8x "
	"Handled floating point failure SIGFPE (signal %d)\n",
	pthread_getthreadid_np(), sigNumber);
	/* By definition, returning from a POSIX signal handler handles the
	signal*/
}
void segFaultHdlr(int sigNumber) {
	printf("Thread 0x%.8x %.8x "
	"Handled segmentation violation SIGSEGV (signal %d)\n",
	pthread_getthreadid_np(), sigNumber);

	/* By definition, returning from a POSIX signal handler handles the
	signal*/
}

int main(int argc, char **argv)
{
	int rc=0;
	pthread_t threadid;
	struct sigaction actions;
	void *status;

	printf("----------- Setup Signal Mapping/Handling -------------\n");
	printf("- Register ANSI C signal handler to map ALL\n"
	" ANSI C signals & hardware exceptions to POSIX signals\n");
	signal(SIGALL, myAnsiSignalMapperHdlr);

	printf("- Register normal POSIX signal handling mechanisms\n"
	" for floating point violations, and segmentation faults\n"
	"- Other signals take the default action for asynchronous
	signals\n");

	memset(&actions, 0, sizeof(actions));
	sigemptyset(&actions.sa_mask);
	actions.sa_flags = 0;
	actions.sa_handler = fpViolationHldr;
	rc = sigaction(SIGFPE,&actions,NULL);
	checkResults("sigaction for SIGFPE\n", rc);

	actions.sa_handler = segFaultHdlr;
	rc = sigaction(SIGSEGV,&actions,NULL);
	checkResults("sigaction for SIGSEGV\n", rc);

	printf("----------- Start memory fault thread -------------\n");
	printf("Create a thread\n");
	rc = pthread_create(&threadid, NULL, threadfunc1, NULL);
	checkResults("pthread_create()\n", rc);

	rc = pthread_join(threadid, &status);
	checkResults("pthread_join()\n", rc);

	printf("----------- Start divide by 0 thread -------------\n");
	printf("Create a thread\n");
	rc = pthread_create(&threadid, NULL, threadfunc2, NULL);
	checkResults("pthread_create()\n", rc);

	rc = pthread_join(threadid, &status);
	checkResults("pthread_join()\n", rc);

	printf("Main completed\n");
	return 0;
}


// Example Output
// ----------- Setup Signal Mapping/Handling -------------
// - Register ANSI C signal handler to map ALL
// ANSI C signals & hardware exceptions to POSIX signals
// - Register normal POSIX signal handling mechanisms
// for floating point violations, and segmentation faults
// - Other signals take the default action for asynchronous signals
// ----------- Start memory fault thread -------------
// Create a thread
// Thread1: Unhandled exception (pointer fault) about to happen
// Mapping ANSI signal to POSIX signal 5
// Thread 0x00000000 00000022 Handled segmentation violation SIGSEGV (signal 5)
// Thread1: After exception
// ----------- Start divide by 0 thread -------------
// Create a thread
// Thread2: Unhandled exception (divide by zero) about to happen
// Mapping ANSI signal to POSIX signal 2
// Thread 0x00000000 00000023 Handled floating point failure SIGFPE (signal 2)
// Thread2: After exception
// Main completed
// 




