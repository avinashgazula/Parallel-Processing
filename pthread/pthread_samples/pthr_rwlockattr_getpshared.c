
// pthread_rwlockattr_getpshared()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/shm.h>
#include "check.h"

typedef struct {
	int protectedResource;
	pthread_rwlock_t rwlock;
} shared_data_t;

extern char **environ;
shared_data_t *sharedMem=NULL;
pid_t childPid=0;
int childStatus=-99;
int shmid=0;

/* Change this path to be the path to where you create this example program
*/
#define MYPATH "/QSYS.LIB/QP0WTEST.LIB/TPRWLSH0.PGM"
#define NTHREADSTHISJOB 2
#define NTHREADSTOTAL 4

void parentSetup(void);
void childSetup(void);
void parentCleanup(void);
void childCleanup(void);

void *childReaderThreadFunc(void *parm)
{
	int rc;
	int retries = 5;

	while (retries--) {
		rc = pthread_rwlock_rdlock(&sharedMem->rwlock);
		checkResults("pthread_rwlock_rdlock()\n", rc);

		/* Under protection of the shared read lock, read the resource */
		printf("CHILD READER - current protectedResource = %d\n",
		sharedMem->protectedResource);
		sleep(1);

		printf("CHILD READER - unlock\n");
		rc = pthread_rwlock_unlock(&sharedMem->rwlock);
		checkResults("pthread_rwlock_unlock()\n", rc);
	}

	return NULL;
}

void *parentWriterThreadFunc(void *parm)
{
	int rc;

	rc = pthread_rwlock_wrlock(&sharedMem->rwlock);
	checkResults("pthread_rwlock_rdlock()\n", rc);

	/* Under protection of the exclusive write lock, write the resource */
	++sharedMem->protectedResource;
	printf("PARENT WRITER - current protectedResource = %d\n",
	sharedMem->protectedResource);
	sleep(5);

	printf("PARENT WRITER - unlock\n");
	rc = pthread_rwlock_unlock(&sharedMem->rwlock);
	checkResults("pthread_rwlock_unlock()\n", rc);

	return NULL;
}

int main(int argc, char **argv)
{
	int rc=0;
	int i;
	pthread_t threadid[NTHREADSTHISJOB];
	int parentJob=0;
	void *status=NULL;

	/* If we run this from the QSHELL interpreter on the AS/400, we want
	*/
	/* it to be line buffered even if we run it in batch so the output
	between*/
	/* parent and child is intermixed.
	*/
	setvbuf(stdout,NULL,_IOLBF,4096);

	/* Determine if we are running in the parent or child */
	if (argc != 1 && argc != 2) {
		printf("Incorrect usage\n");
		printf("Pass no parameters to run as the parent testcase\n");
		printf("Pass one parameter `ASCHILD' to run as the child testcase\n");
		exit(1);
	}

	if (argc == 1) {
		parentJob = 1;
	}
	else {
		if (strcmp(argv[1], "ASCHILD")) {
			printf("Incorrect usage\n");
			printf("Pass no parameters to run as the parent testcase\n");
			printf("Pass one parameter `ASCHILD' to run as the child
			testcase\n");
			exit(1);
		}

		parentJob = 0;
	}

	/* PARENT
	*****************************************************************/
	if (parentJob) {
		printf("PARENT - Enter Testcase - %s\n", argv[0]);
		parentSetup();

		printf("PARENT - Create %d threads\n", NTHREADSTHISJOB);
		for (i=0; i<NTHREADSTHISJOB; ++i) {
			rc = pthread_create(&threadid[i], NULL, parentWriterThreadFunc,
			NULL);
			checkResults("pthread_create()\n", rc);
		}

		for (i=0; i<NTHREADSTHISJOB; ++i) {
			rc = pthread_join(threadid[i], NULL);
			checkResults("pthread_create()\n", rc);

			if (status != NULL) {
				printf("PARENT - Got a bad status from a thread, "
				"%.8x %.8x %.8x %.8x\n", status);
				exit(1);
			}
		}

		parentCleanup();
		printf("PARENT - Main completed\n");
		exit(0);
	}

	/* CHILD
	*****************************************************************/
	{
		printf("CHILD - Enter Testcase - %s\n", argv[0]);
		childSetup();

		printf("CHILD - Create %d threads\n", NTHREADSTHISJOB);
		for (i=0; i<NTHREADSTHISJOB; ++i) {
			rc = pthread_create(&threadid[i], NULL, childReaderThreadFunc,
			NULL);
			checkResults("pthread_create()\n", rc);
		}

		/* The parent will wake up all of these threads using the */
		/* pshared condition variable. We will just join to them... */
		printf("CHILD - Joining to all threads\n");
		for (i=0; i<NTHREADSTHISJOB; ++i) {
			rc = pthread_join(threadid[i], &status);
			checkResults("pthread_join()\n", rc);

			if (status != NULL) {
				printf("CHILD - Got a bad status from a thread, "
				"%.8x %.8x %.8x %.8x\n", status);
				exit(1);
			}
		}

		/* After all the threads are awake, the parent will destroy */
		/* the read/write lock. Do not use it anymore */
		childCleanup();
		printf("CHILD - Main completed\n");
	}

	return 0;
}

/***************************************************************/
/* This function initializes the shared memory for the job, */
/* sets up the environment variable indicating where the shared*/
/* memory is, and spawns the child job. */
/* */
/* It creates and initializes the shared memory segment, and */
/* It initializes the following global variables in this */
/* job. */
/* sharedMem */
/* childPid */
/* shmid */
/* */
/* If any of this setup/initialization fails, it will exit(1) */
/* and terminate the test. */
/* */
/***************************************************************/
void parentSetup(void)
{
	int rc;

	/***************************************************************/
	/* Create shared memory for shared_data_t above */
	/* attach the shared memory */
	/* set the static/global sharedMem pointer to it */
	/***************************************************************/
	printf("PARENT - Create the shared memory segment\n");
	rc = shmget(IPC_PRIVATE, sizeof(shared_data_t), 0666);

	if (rc == -1) {
		printf("PARENT - Failed to create a shared memory segment\n");
		exit(1);
	}

	shmid = rc;
	printf("PARENT - Attach the shared memory\n");
	sharedMem = shmat(shmid, NULL, 0);

	if (sharedMem == NULL) {
		shmctl(shmid, IPC_RMID, NULL);
		printf("PARENT - Failed to attach shared memory\n");
		exit(1);
	}

	/***************************************************************/
	/* Initialize the read/write lock and other shared memory data */
	/***************************************************************/

	{
		pthread_rwlockattr_t rwlattr;
		printf("PARENT - Init shared memory and read/write lock\n");
		memset(sharedMem, 0, sizeof(shared_data_t));

		/* Process Shared Read/Write lock */
		rc = pthread_rwlockattr_init(&rwlattr);
		checkResults("pthread_rwlockattr_init()\n", rc);

		rc = pthread_rwlockattr_setpshared(&rwlattr, PTHREAD_PROCESS_SHARED);
		checkResults("pthread_rwlockattr_setpshared()\n", rc);

		rc = pthread_rwlock_init(&sharedMem->rwlock, &rwlattr);
		checkResults("pthread_rwlock_init()\n", rc);
	}

	/**************************************************************/
	/* Set and environment variable so that the child can inherit */
	/* it and know the shared memory ID */
	/**************************************************************/
	{
		char shmIdEnvVar[128];

		sprintf(shmIdEnvVar, "TPRWLSH0_SHMID=%d\n", shmid);
		rc = putenv(shmIdEnvVar);

		if (rc) {
			printf("PARENT - Failed to store env var %s, errno=%d\n",
			shmIdEnvVar, errno);
			exit(1);
		}

		printf("PARENT - Stored shared memory id of %d\n", shmid);
	}

	/**************************************************/
	/* Spawn the child job */
	/**************************************************/
	{
		inheritance_t in;
		char *av[3] = {NULL, NULL, NULL};

		/* Allow thread creation in the spawned child */
		memset(&in, 0, sizeof(in));
		in.flags = SPAWN_SETTHREAD_NP;

		/* Set up the arguments to pass to spawn based on the */
		/* arguments passed in */
		av[0] = MYPATH;
		av[1] = "ASCHILD";
		av[2] = NULL;

		/* Spawn the child that was specified, inheriting all */
		/* of the environment variables. */
		childPid = spawn(MYPATH, 0, NULL, &in, av, environ);
		if (childPid == -1) {
			/* spawn failure */
			printf("PARENT - spawn() failed, errno=%d\n", errno);
			exit(1);
		}

		printf("PARENT - spawn() success, [PID=%d]\n", childPid);
	}

	return;
}

/***************************************************************/
/* This function attaches the shared memory for the child job, */
/* It uses the environment variable indicating where the shared*/
/* memory is. */
/* */
/* If any of this setup/initialization fails, it will exit(1) */
/* and terminate the test. */
/* */
/* It initializes the following global variables: */
/* sharedMem */
/* shmid */
/***************************************************************/
void childSetup(void)
{
	int rc;

	printf("CHILD - Child setup\n");

	/**************************************************************/
	/* Set and environment variable so that the child can inherit */
	/* it and know the shared memory ID */
	/**************************************************************/
	{
		char *shmIdEnvVar;

		shmIdEnvVar = getenv("TPRWLSH0_SHMID");
		if (shmIdEnvVar == NULL) {
			printf("CHILD - Failed to get env var \"TPRWLSH0_SHMID\",
			errno=%d\n",
			errno);
			exit(1);
		}

		shmid = atoi(shmIdEnvVar);
		printf("CHILD - Got shared memory id of %d\n", shmid);
	}

	/***************************************************************/
	/* Create shared memory for shared_data_t above */
	/* attach the shared memory */
	/* set the static/global sharedMem pointer to it */
	/***************************************************************/

	printf("CHILD - Attach the shared memory\n");
	sharedMem = shmat(shmid, NULL, 0);

	if (sharedMem == NULL) {
		shmctl(shmid, IPC_RMID, NULL);
		printf("CHILD - Failed to attach shared memory\n");
		exit(1);
	}

	return;
}

/***************************************************************/
/* wait for child to complete and get return code. */
/* Destroy read/write lock in shared memory */
/* detach and remove shared memory */
/* set the child's return code in global storage */
/* */
/* If this function fails, it will call exit(1) */
/* */
/* This function sets the following global variables: */
/* sharedMem */
/* childStatus */
/* shmid */
/***************************************************************/
void parentCleanup(void)
{
	int status=0;
	int rc;
	int waitedPid=0;

	/* Even though there is no thread left in the child using the */
	/* contents of the shared memory, before we destroy the */
	/* read/write lock in that shared memory, we will wait for the */
	/* child job to complete, we know for 100% certainty that no */
	/* threads in the child are using it then, because the child */
	/* is terminated. */
	printf("PARENT - Parent cleanup\n");

	/* Wait for the child to complete */
	waitedPid = waitpid(childPid,&status,0);

	if (rc == -1) {
		printf("PARENT - waitpid failed, errno=%d\n", errno);
		exit(1);
	}

	childStatus = status;

	/* Cleanup resources */
	rc = pthread_rwlock_destroy(&sharedMem->rwlock);
	checkResults("pthread_rwlock_destroy()\n", rc);

	/* Detach/Remove shared memory */
	rc = shmdt(sharedMem);
	if (rc) {
		printf("PARENT - Failed to detach shared memory, errno=%d\n", errno);
		exit(1);
	}

	rc = shmctl(shmid, IPC_RMID, NULL);
	if (rc) {
		printf("PARENT - Failed to remove shared memory id=%d, errno=%d\n",
		shmid, errno);
		exit(1);
	}

	shmid = 0;
	return;
}

/***************************************************************/
/* Detach the shared memory */
/* At this point, there is no serialization, so the contents */
/* of the shared memory should not be used. */
/* */
/* If this function fails, it will call exit(1) */
/* */
/* This function sets the following global variables: */
/* sharedMem */
/***************************************************************/
void childCleanup(void)
{
	int rc;

	printf("CHILD - Child cleanup\n");
	rc = shmdt(sharedMem);

	sharedMem = NULL;
	if (rc) {
		printf("CHILD - Failed to detach shared memory, errno=%d\n", errno);
		exit(1);
	}

	return;
}


// Output:
// This example was run under the OS/400 QShell Interpreter. In the QShell Interpreter, a program gets descriptors 0, 1,
// 2 as the standard files, the parent and child I/O is directed to the console. When run in the QShell Interpreter, the
// output shows the intermixed output from both parent and child processes, and gives a feeling for the time sequence of
// operations occurring in each job.
// The QShell interpreter allows you to run multithreaded programs as if they were interactive. See the QShell
// documentation for a description of the QIBM_MULTI_THREADED shell variable which allows you to start
// multithreaded programs.
// The QShell Interpreter is option 30 of Base OS/400.
// PARENT - Enter Testcase - 
// PARENT - Create the shared memory segment
// PARENT - Attach the shared memory
// PARENT - Init shared memory and read/write lock
// PARENT - Stored shared memory id of 7
// PARENT - spawn() success, [PID=584]
// PARENT - Create 2 threads
// PARENT WRITER - current protectedResource = 1
// CHILD - Enter Testcase - 
// CHILD - Child setup
// CHILD - Got shared memory id of 7
// CHILD - Attach the shared memory
// CHILD - Create 2 threads
// CHILD - Joining to all threads
// PARENT WRITER - unlock
// PARENT WRITER - current protectedResource = 2
// PARENT WRITER - unlock
// CHILD READER - current protectedResource = 2
// CHILD READER - current protectedResource = 2
// PARENT - Parent cleanup
// CHILD READER - unlock
// CHILD READER - current protectedResource = 2
// CHILD READER - unlock
// CHILD READER - current protectedResource = 2
// CHILD READER - unlock
// CHILD READER - current protectedResource = 2
// CHILD READER - unlock
// CHILD READER - current protectedResource = 2
// CHILD READER - unlock
// CHILD READER - current protectedResource = 2
// CHILD READER - unlock
// CHILD READER - current protectedResource = 2
// CHILD READER - unlock
// CHILD READER - current protectedResource = 2
// CHILD READER - unlock
// CHILD READER - current protectedResource = 2
// CHILD READER - unlock
// CHILD READER - unlock
// CHILD - Child cleanup
// CHILD - Main completed
// PARENT - Main completed









