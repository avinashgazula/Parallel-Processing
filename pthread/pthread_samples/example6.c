
// Example
#define _MULTI_THREADED
#include <stdio.h>
#include <stdlib.h>
#include <qp0z1170.h>
#include <time.h>
#include <signal.h>
#include <except.h>
#include <qusec.h> /* System API error Code structure */
#include <qmh.h> /* Message Hanlder common defs */
#include <qmhchgem.h> /* Change exception message */
#include <pthread.h>
#include "check.h"

void myHardwareExceptionMapper(_INTRPT_Hndlr_Parms_T *exception);
void *threadfunc1(void *parm);
void *threadfunc2(void *parm);

void *threadfunc1(void *parm)
{
	char *p=NULL;

	/* Watch for all ESCAPE type exceptions. Other types may be used for
	*/
	/* job log messages or C++ exceptions or other control flow in the
	process*/
	/* Adjust the message type as required by your application.
	*/

#pragma exception_handler (myHardwareExceptionMapper, 0, _C1_ALL,
_C2_MH_ESCAPE)
	printf("Thread1: Unhandled exception (pointer fault) about to happen\n");
	*p = `!';
	printf("Thread1: After exception\n");
#pragma disable_handler

	return NULL;
}
void *threadfunc2(void *parm)
{
	int i1=0, i2=1, i3=0;

	/* Watch for all ESCAPE type exceptions. Others types may be used for
	*/
	/* job log messages or C++ exceptions or other control flow in the
	process*/
	/* Adjust the message type as required by your application.
	*/

#pragma exception_handler (myHardwareExceptionMapper, 0, _C1_ALL,
_C2_MH_ESCAPE)
	printf("Thread2: Unhandled exception (divide by zero) about to happen\n");
	i1 = i2 / i3;
	printf("Thread2: After exception\n");
#pragma disable_handler

	return NULL;
}

void myHardwareExceptionMapper(_INTRPT_Hndlr_Parms_T *exInfo) {
	int sigNumber;
	Qus_EC_t errorCode = {0}; /* system API error structure
	*/

	printf("Handling system exception\n");
	/* The exception information is available inside the exInfo structure
	*/
	/* for this example, we are going to handle all exceptions and then map
	*/
	/* them to an \Qappropriate' signal number. We are allowed to decide the
	*/
	/* signal mapping however is appropriate for our application.
	*/

	if (!memcmp(exInfo->Msg_Id, "MCH3601", 7)) {
		sigNumber = SIGSEGV;
	}
	else if (!memcmp(exInfo->Msg_Id, "MCH1211", 7)) {
		sigNumber = SIGFPE;
	}
	else {
		printf("Unexpected exception! Not Handling!\n");
		abort();
	}

	/* Even if the exception is \Qexpected', we are going to handle it and try
	*/
	/* to deliver it as a POSIX signal. Note that we SHOULD NOT HANDLE
	*/
	/* exceptions that are unexpected to us. Most code cannot tolerate
	*/
	/* getting back into it once the exception occured, and we could get
	into*/
	/* a nice exception loop.
	*/
	/* See the system API reference for a description of QMHCHGEM
	*/
	QMHCHGEM(&exInfo->Target, 0, &exInfo->Msg_Ref_Key, QMH_MOD_HANDLE,
	(char *)NULL, 0, &errorCode);

	if (errorCode.Bytes_Available != 0) {
		printf("Failed to handle exception. Error Code = %7.7s\n",
		errorCode.Exception_Id);
		return;
	}

	printf("Mapping Exception %7.7s to POSIX signal %d\n",
	exInfo->Msg_Id ,sigNumber);

	/* At this point the exception is handled. If the POSIX signal handler
	*/
	/* returns, then the signal will be handled, and all will be complete
	*/
	pthread_kill(pthread_self(), sigNumber);

	return;
}

void fpViolationHldr(int sigNumber) {
	printf("Thread 0x%.8x %.8x "
	"Handled floating point failure SIGFPE (signal %d)\n",
	pthread_getthreadid_np(), sigNumber);
	/* By definition, return from a POSIX signal handler handles the signal
	*/
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
	printf("- The threads will register AS/400 Exception handler to map\n"
	" hardware exceptions to POSIX signals\n");
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


// Output
// ----------- Setup Signal Mapping/Handling -------------
// - The threads will register AS/400 Exception handler to map
// hardware exceptions to POSIX signals
// - Register normal POSIX signal handling mechanisms
// for floating point violations, and segmentation faults
// - Other signals take the default action for asynchronous signals
// ----------- Start memory fault thread -------------
// Create a thread
// Thread1: Unhandled exception (pointer fault) about to happen
// Handling system exception
// Mapping Exception MCH3601 to POSIX signal 5
// Thread 0x00000000 00000024 Handled segmentation violation SIGSEGV (signal 5)
// Thread1: After exception
// ----------- Start divide by 0 thread -------------
// Create a thread
// Thread2: Unhandled exception (divide by zero) about to happen
// Handling system exception
// Mapping Exception MCH1211 to POSIX signal 2
// Thread 0x00000000 00000025 Handled floating point failure SIGFPE (signal 2)
// Thread2: After exception
// Main completed



